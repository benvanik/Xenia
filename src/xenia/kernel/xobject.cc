/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2013 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "xenia/kernel/xobject.h"

#include <vector>

#include "xenia/base/byte_stream.h"
#include "xenia/base/clock.h"
#include "xenia/kernel/kernel_state.h"
#include "xenia/kernel/notify_listener.h"
#include "xenia/kernel/xboxkrnl/xboxkrnl_private.h"
#include "xenia/kernel/xenumerator.h"
#include "xenia/kernel/xevent.h"
#include "xenia/kernel/xfile.h"
#include "xenia/kernel/xmodule.h"
#include "xenia/kernel/xmutant.h"
#include "xenia/kernel/xsemaphore.h"
#include "xenia/kernel/xthread.h"

namespace xe {
namespace kernel {

XObject::XObject(KernelState* kernel_state, Type type)
    : kernel_state_(kernel_state),
      pointer_ref_count_(1),
      type_(type),
      guest_object_ptr_(0),
      allocated_guest_object_(false) {
  handles_.reserve(10);
  if (kernel_state) {
    kernel_state->object_table()->AddHandle(this, &handles_[0]);
  }
}

XObject::~XObject() {
  assert_zero(pointer_ref_count_);

  if (allocated_guest_object_) {
    uint32_t ptr = guest_object_ptr_ - sizeof(X_OBJECT_HEADER);
    auto header = memory()->TranslateVirtual<X_OBJECT_HEADER*>(ptr);

    // Free the object creation info
    if (header->object_type_ptr) {
      memory()->SystemHeapFree(header->object_type_ptr);
    }

    memory()->SystemHeapFree(ptr);
  }
}

Emulator* XObject::emulator() const { return kernel_state_->emulator_; }
KernelState* XObject::kernel_state() const { return kernel_state_; }
Memory* XObject::memory() const { return kernel_state_->memory(); }

XObject::Type XObject::type() { return type_; }

void XObject::RetainHandle() {
  kernel_state_->object_table()->RetainHandle(handles_[0]);
}

bool XObject::ReleaseHandle() {
  // FIXME: Return true when handle is actually released.
  return kernel_state_->object_table()->ReleaseHandle(handles_[0]) ==
         X_STATUS_SUCCESS;
}

void XObject::Retain() { ++pointer_ref_count_; }

void XObject::Release() {
  if (--pointer_ref_count_ == 0) {
    delete this;
  }
}

X_STATUS XObject::Delete() {
  if (kernel_state_ == nullptr) {
    // Fake return value for api-scanner
    return X_STATUS_SUCCESS;
  } else {
    if (!name_.empty()) {
      kernel_state_->object_table()->RemoveNameMapping(name_);
    }
    return kernel_state_->object_table()->RemoveHandle(handles_[0]);
  }
}

bool XObject::SaveObject(ByteStream* stream) {
  stream->Write<uint32_t>(allocated_guest_object_);
  stream->Write<uint32_t>(guest_object_ptr_);

  stream->Write(uint32_t(handles_.size()));
  stream->Write(&handles_[0], handles_.size() * sizeof(X_HANDLE));

  return true;
}

bool XObject::RestoreObject(ByteStream* stream) {
  allocated_guest_object_ = stream->Read<uint32_t>() > 0;
  guest_object_ptr_ = stream->Read<uint32_t>();

  handles_.resize(stream->Read<uint32_t>());
  stream->Read(&handles_[0], handles_.size() * sizeof(X_HANDLE));

  // Restore our pointer to our handles in the object table.
  for (size_t i = 0; i < handles_.size(); i++) {
    kernel_state_->object_table()->RestoreHandle(handles_[i], this);
  }

  return true;
}

object_ref<XObject> XObject::Restore(KernelState* kernel_state, Type type,
                                     ByteStream* stream) {
  switch (type) {
    case kTypeEnumerator:
      break;
    case kTypeEvent:
      return XEvent::Restore(kernel_state, stream);
    case kTypeFile:
      break;
    case kTypeIOCompletion:
      break;
    case kTypeModule:
      return XModule::Restore(kernel_state, stream);
    case kTypeMutant:
      break;
    case kTypeNotifyListener:
      return NotifyListener::Restore(kernel_state, stream);
    case kTypeSemaphore:
      break;
    case kTypeSession:
      break;
    case kTypeSocket:
      break;
    case kTypeThread:
      return XThread::Restore(kernel_state, stream);
    case kTypeTimer:
      break;
  }

  assert_always("No restore handler exists for this object!");
  return nullptr;
}

void XObject::SetAttributes(uint32_t obj_attributes_ptr) {
  if (!obj_attributes_ptr) {
    return;
  }

  auto name = X_ANSI_STRING::to_string_indirect(memory()->virtual_membase(),
                                                obj_attributes_ptr + 4);
  if (!name.empty()) {
    name_ = std::move(name);
    kernel_state_->object_table()->AddNameMapping(name_, handles_[0]);
  }
}

uint32_t XObject::TimeoutTicksToMs(int64_t timeout_ticks) {
  if (timeout_ticks > 0) {
    // Absolute time, based on January 1, 1601.
    // TODO(benvanik): convert time to relative time.
    assert_always();
    return 0;
  } else if (timeout_ticks < 0) {
    // Relative time.
    return (uint32_t)(-timeout_ticks / 10000);  // Ticks -> MS
  } else {
    return 0;
  }
}

X_STATUS XObject::Wait(uint32_t wait_reason, uint32_t processor_mode,
                       uint32_t alertable, uint64_t* opt_timeout) {
  auto wait_handle = GetWaitHandle();
  if (!wait_handle) {
    // Object doesn't support waiting.
    return X_STATUS_SUCCESS;
  }

  auto timeout_ms =
      opt_timeout ? std::chrono::milliseconds(Clock::ScaleGuestDurationMillis(
                        TimeoutTicksToMs(*opt_timeout)))
                  : std::chrono::milliseconds::max();

  auto result =
      xe::threading::Wait(wait_handle, alertable ? true : false, timeout_ms);
  switch (result) {
    case xe::threading::WaitResult::kSuccess:
      return X_STATUS_SUCCESS;
    case xe::threading::WaitResult::kUserCallback:
      // Or X_STATUS_ALERTED?
      return X_STATUS_USER_APC;
    case xe::threading::WaitResult::kTimeout:
      xe::threading::MaybeYield();
      return X_STATUS_TIMEOUT;
    default:
    case xe::threading::WaitResult::kAbandoned:
    case xe::threading::WaitResult::kFailed:
      return X_STATUS_ABANDONED_WAIT_0;
  }
}

X_STATUS XObject::SignalAndWait(XObject* signal_object, XObject* wait_object,
                                uint32_t wait_reason, uint32_t processor_mode,
                                uint32_t alertable, uint64_t* opt_timeout) {
  auto timeout_ms =
      opt_timeout ? std::chrono::milliseconds(Clock::ScaleGuestDurationMillis(
                        TimeoutTicksToMs(*opt_timeout)))
                  : std::chrono::milliseconds::max();

  auto result = xe::threading::SignalAndWait(
      signal_object->GetWaitHandle(), wait_object->GetWaitHandle(),
      alertable ? true : false, timeout_ms);
  switch (result) {
    case xe::threading::WaitResult::kSuccess:
      return X_STATUS_SUCCESS;
    case xe::threading::WaitResult::kUserCallback:
      // Or X_STATUS_ALERTED?
      return X_STATUS_USER_APC;
    case xe::threading::WaitResult::kTimeout:
      xe::threading::MaybeYield();
      return X_STATUS_TIMEOUT;
    default:
    case xe::threading::WaitResult::kAbandoned:
    case xe::threading::WaitResult::kFailed:
      return X_STATUS_ABANDONED_WAIT_0;
  }
}

X_STATUS XObject::WaitMultiple(uint32_t count, XObject** objects,
                               uint32_t wait_type, uint32_t wait_reason,
                               uint32_t processor_mode, uint32_t alertable,
                               uint64_t* opt_timeout) {
  std::vector<xe::threading::WaitHandle*> wait_handles(count);
  for (size_t i = 0; i < count; ++i) {
    wait_handles[i] = objects[i]->GetWaitHandle();
    assert_not_null(wait_handles[i]);
  }

  auto timeout_ms =
      opt_timeout ? std::chrono::milliseconds(Clock::ScaleGuestDurationMillis(
                        TimeoutTicksToMs(*opt_timeout)))
                  : std::chrono::milliseconds::max();

  if (wait_type) {
    auto result = xe::threading::WaitAny(std::move(wait_handles),
                                         alertable ? true : false, timeout_ms);
    switch (result.first) {
      case xe::threading::WaitResult::kSuccess:
        return X_STATUS(result.second);
      case xe::threading::WaitResult::kUserCallback:
        // Or X_STATUS_ALERTED?
        return X_STATUS_USER_APC;
      case xe::threading::WaitResult::kTimeout:
        xe::threading::MaybeYield();
        return X_STATUS_TIMEOUT;
      default:
      case xe::threading::WaitResult::kAbandoned:
        return X_STATUS(X_STATUS_ABANDONED_WAIT_0 + result.second);
      case xe::threading::WaitResult::kFailed:
        return X_STATUS_UNSUCCESSFUL;
    }
  } else {
    auto result = xe::threading::WaitAll(std::move(wait_handles),
                                         alertable ? true : false, timeout_ms);
    switch (result) {
      case xe::threading::WaitResult::kSuccess:
        return X_STATUS_SUCCESS;
      case xe::threading::WaitResult::kUserCallback:
        // Or X_STATUS_ALERTED?
        return X_STATUS_USER_APC;
      case xe::threading::WaitResult::kTimeout:
        xe::threading::MaybeYield();
        return X_STATUS_TIMEOUT;
      default:
      case xe::threading::WaitResult::kAbandoned:
      case xe::threading::WaitResult::kFailed:
        return X_STATUS_ABANDONED_WAIT_0;
    }
  }
}

uint8_t* XObject::CreateNative(uint32_t size) {
  auto global_lock = xe::global_critical_region::AcquireDirect();

  uint32_t total_size = size + sizeof(X_OBJECT_HEADER);

  auto mem = memory()->SystemHeapAlloc(total_size);
  if (!mem) {
    // Out of memory!
    return nullptr;
  }

  allocated_guest_object_ = true;
  memory()->Zero(mem, total_size);
  SetNativePointer(mem + sizeof(X_OBJECT_HEADER), true);

  auto header = memory()->TranslateVirtual<X_OBJECT_HEADER*>(mem);

  auto object_type = memory()->SystemHeapAlloc(sizeof(X_OBJECT_TYPE));
  if (object_type) {
    // Set it up in the header.
    // Some kernel method is accessing this struct and dereferencing a member
    // @ offset 0x14
    header->object_type_ptr = object_type;
  }

  return memory()->TranslateVirtual(guest_object_ptr_);
}

void XObject::SetNativePointer(uint32_t native_ptr, bool uninitialized) {
  auto global_lock = xe::global_critical_region::AcquireDirect();

  // If hit: We've already setup the native ptr with CreateNative!
  assert_zero(guest_object_ptr_);

  auto header =
      kernel_state_->memory()->TranslateVirtual<X_DISPATCH_HEADER*>(native_ptr);

  // Memory uninitialized, so don't bother with the check.
  if (!uninitialized) {
    assert_true(!(header->wait_list_blink & 0x1));
  }

  // Stash pointer in struct.
  // FIXME: This assumes the object has a dispatch header (some don't!)
  StashHandle(header, handle());

  guest_object_ptr_ = native_ptr;
}

object_ref<XObject> XObject::GetNativeObject(KernelState* kernel_state,
                                             void* native_ptr,
                                             int32_t as_type) {
  assert_not_null(native_ptr);

  // Unfortunately the XDK seems to inline some KeInitialize calls, meaning
  // we never see it and just randomly start getting passed events/timers/etc.
  // Luckily it seems like all other calls (Set/Reset/Wait/etc) are used and
  // we don't have to worry about PPC code poking the struct. Because of that,
  // we init on first use, store our handle in the struct, and dereference it
  // each time.
  // We identify this by setting wait_list_flink to a magic value. When set,
  // wait_list_blink will hold a handle to our object.

  auto global_lock = xe::global_critical_region::AcquireDirect();

  auto header = reinterpret_cast<X_DISPATCH_HEADER*>(native_ptr);

  if (as_type == -1) {
    as_type = header->type;
  }

  if (header->wait_list_flink == 'XEN\0') {
    // Already initialized.
    // TODO: assert if the type of the object != as_type
    uint32_t handle = header->wait_list_blink;
    auto object = kernel_state->object_table()->LookupObject<XObject>(handle);

    // TODO(benvanik): assert nothing has been changed in the struct.
    return object;
  } else {
    // First use, create new.
    // http://www.nirsoft.net/kernel_struct/vista/KOBJECTS.html
    XObject* object = nullptr;
    switch (as_type) {
      case 0:  // EventNotificationObject
      case 1:  // EventSynchronizationObject
      {
        auto ev = new XEvent(kernel_state);
        ev->InitializeNative(native_ptr, header);
        object = ev;
      } break;
      case 2:  // MutantObject
      {
        auto mutant = new XMutant(kernel_state);
        mutant->InitializeNative(native_ptr, header);
        object = mutant;
      } break;
      case 5:  // SemaphoreObject
      {
        auto sem = new XSemaphore(kernel_state);
        sem->InitializeNative(native_ptr, header);
        object = sem;
      } break;
      case 3:   // ProcessObject
      case 4:   // QueueObject
      case 6:   // ThreadObject
      case 7:   // GateObject
      case 8:   // TimerNotificationObject
      case 9:   // TimerSynchronizationObject
      case 18:  // ApcObject
      case 19:  // DpcObject
      case 20:  // DeviceQueueObject
      case 21:  // EventPairObject
      case 22:  // InterruptObject
      case 23:  // ProfileObject
      case 24:  // ThreadedDpcObject
      default:
        assert_always();
        return NULL;
    }

    // Stash pointer in struct.
    // FIXME: This assumes the object contains a dispatch header (some don't!)
    StashHandle(header, object->handle());

    return object_ref<XObject>(object);
  }
}

}  // namespace kernel
}  // namespace xe
