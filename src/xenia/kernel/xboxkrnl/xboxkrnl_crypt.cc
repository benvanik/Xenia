/**
******************************************************************************
* Xenia : Xbox 360 Emulator Research Project                                 *
******************************************************************************
* Copyright 2015 Ben Vanik. All rights reserved.                             *
* Released under the BSD license - see LICENSE in the root for more details. *
******************************************************************************
*/

#include "xenia/base/logging.h"
#include "xenia/kernel/kernel_state.h"
#include "xenia/kernel/util/aes.h"
#include "xenia/kernel/util/shim_utils.h"
#include "xenia/kernel/xboxkrnl/xboxkrnl_private.h"
#include "xenia/xbox.h"

#include "third_party/crypto/TinySHA1.hpp"
#include "third_party/crypto/des/des.cpp"
#include "third_party/crypto/des/des.h"
#include "third_party/crypto/des/des3.h"
#include "third_party/crypto/des/descbc.h"
#include "third_party/crypto/sha256.cpp"
#include "third_party/crypto/sha256.h"

namespace xe {
namespace kernel {
namespace xboxkrnl {

typedef struct {
  uint8_t S[256];  // 0x0
  uint8_t i;       // 0x100
  uint8_t j;       // 0x101
} XECRYPT_RC4_STATE;
static_assert_size(XECRYPT_RC4_STATE, 0x102);

void XeCryptRc4Key(pointer_t<XECRYPT_RC4_STATE> rc4_ctx, lpvoid_t key,
                   dword_t key_size) {
  // Setup RC4 state
  rc4_ctx->i = rc4_ctx->j = 0;
  for (uint32_t x = 0; x < 0x100; x++) {
    rc4_ctx->S[x] = (uint8_t)x;
  }

  uint32_t idx = 0;
  for (uint32_t x = 0; x < 0x100; x++) {
    idx = (idx + rc4_ctx->S[x] + key[x % 0x10]) % 0x100;
    uint8_t temp = rc4_ctx->S[idx];
    rc4_ctx->S[idx] = rc4_ctx->S[x];
    rc4_ctx->S[x] = temp;
  }
}
DECLARE_XBOXKRNL_EXPORT1(XeCryptRc4Key, kNone, kImplemented);

void XeCryptRc4Ecb(pointer_t<XECRYPT_RC4_STATE> rc4_ctx, lpvoid_t data,
                   dword_t size) {
  // Crypt data
  for (uint32_t idx = 0; idx < size; idx++) {
    rc4_ctx->i = (rc4_ctx->i + 1) % 0x100;
    rc4_ctx->j = (rc4_ctx->j + rc4_ctx->S[rc4_ctx->i]) % 0x100;
    uint8_t temp = rc4_ctx->S[rc4_ctx->i];
    rc4_ctx->S[rc4_ctx->i] = rc4_ctx->S[rc4_ctx->j];
    rc4_ctx->S[rc4_ctx->j] = temp;

    uint8_t a = data[idx];
    uint8_t b =
        rc4_ctx->S[(rc4_ctx->S[rc4_ctx->i] + rc4_ctx->S[rc4_ctx->j]) % 0x100];
    data[idx] = (uint8_t)(a ^ b);
  }
}
DECLARE_XBOXKRNL_EXPORT1(XeCryptRc4Ecb, kNone, kImplemented);

void XeCryptRc4(lpvoid_t key, dword_t key_size, lpvoid_t data, dword_t size) {
  XECRYPT_RC4_STATE rc4_ctx;
  XeCryptRc4Key(&rc4_ctx, key, key_size);
  XeCryptRc4Ecb(&rc4_ctx, data, size);
}
DECLARE_XBOXKRNL_EXPORT1(XeCryptRc4, kNone, kImplemented);

typedef struct {
  xe::be<uint32_t> count;     // 0x0
  xe::be<uint32_t> state[5];  // 0x4
  uint8_t buffer[64];         // 0x18
} XECRYPT_SHA_STATE;
static_assert_size(XECRYPT_SHA_STATE, 0x58);

void InitSha1(sha1::SHA1* sha, const XECRYPT_SHA_STATE* state) {
  uint32_t digest[5];
  for (int i = 0; i < 5; i++) {
    digest[i] = state->state[i];
  }

  sha->init(digest, state->buffer, state->count);
}

void StoreSha1(sha1::SHA1* sha, XECRYPT_SHA_STATE* state) {
  for (int i = 0; i < 5; i++) {
    state->state[i] = sha->getDigest()[i];
  }

  state->count = static_cast<uint32_t>(sha->getByteCount());
  std::memcpy(state->buffer, sha->getBlock(), sha->getBlockByteIndex());
}

void XeCryptShaInit(pointer_t<XECRYPT_SHA_STATE> sha_state) {
  sha_state.Zero();

  sha_state->state[0] = 0x67452301;
  sha_state->state[1] = 0xEFCDAB89;
  sha_state->state[2] = 0x98BADCFE;
  sha_state->state[3] = 0x10325476;
  sha_state->state[4] = 0xC3D2E1F0;
}
DECLARE_XBOXKRNL_EXPORT1(XeCryptShaInit, kNone, kImplemented);

void XeCryptShaUpdate(pointer_t<XECRYPT_SHA_STATE> sha_state, lpvoid_t input,
                      dword_t input_size) {
  sha1::SHA1 sha;
  InitSha1(&sha, sha_state);

  sha.processBytes(input, input_size);

  StoreSha1(&sha, sha_state);
}
DECLARE_XBOXKRNL_EXPORT1(XeCryptShaUpdate, kNone, kImplemented);

void XeCryptShaFinal(pointer_t<XECRYPT_SHA_STATE> sha_state,
                     pointer_t<xe::be<uint32_t>> out, dword_t out_size) {
  sha1::SHA1 sha;
  InitSha1(&sha, sha_state);

  uint8_t digest[0x14];
  sha.finalize(digest);

  std::memcpy(out, digest, std::min((uint32_t)out_size, 0x14u));
  std::memcpy(sha_state->state, digest, 0x14);
}
DECLARE_XBOXKRNL_EXPORT1(XeCryptShaFinal, kNone, kImplemented);

void XeCryptSha(lpvoid_t input_1, dword_t input_1_size, lpvoid_t input_2,
                dword_t input_2_size, lpvoid_t input_3, dword_t input_3_size,
                lpvoid_t output, dword_t output_size) {
  sha1::SHA1 sha;

  if (input_1 && input_1_size) {
    sha.processBytes(input_1, input_1_size);
  }
  if (input_2 && input_2_size) {
    sha.processBytes(input_2, input_2_size);
  }
  if (input_3 && input_3_size) {
    sha.processBytes(input_3, input_3_size);
  }

  uint8_t digest[0x14];
  sha.finalize(digest);
  std::memcpy(output, digest, std::min((uint32_t)output_size, 0x14u));
}
DECLARE_XBOXKRNL_EXPORT1(XeCryptSha, kNone, kImplemented);

// TODO: Size of this struct hasn't been confirmed yet.
typedef struct {
  xe::be<uint32_t> count;     // 0x0
  xe::be<uint32_t> state[8];  // 0x4
  uint8_t buffer[64];         // 0x24
} XECRYPT_SHA256_STATE;

void XeCryptSha256Init(pointer_t<XECRYPT_SHA256_STATE> sha_state) {
  sha_state.Zero();

  sha_state->state[0] = 0x6a09e667;
  sha_state->state[1] = 0xbb67ae85;
  sha_state->state[2] = 0x3c6ef372;
  sha_state->state[3] = 0xa54ff53a;
  sha_state->state[4] = 0x510e527f;
  sha_state->state[5] = 0x9b05688c;
  sha_state->state[6] = 0x1f83d9ab;
  sha_state->state[7] = 0x5be0cd19;
}
DECLARE_XBOXKRNL_EXPORT1(XeCryptSha256Init, kNone, kImplemented);

void XeCryptSha256Update(pointer_t<XECRYPT_SHA256_STATE> sha_state,
                         lpvoid_t input, dword_t input_size) {
  sha256::SHA256 sha;
  std::memcpy(sha.getHashValues(), sha_state->state, 8 * 4);
  std::memcpy(sha.getBuffer(), sha_state->buffer, 64);
  sha.setTotalSize(sha_state->count);

  sha.add(input, input_size);

  std::memcpy(sha_state->state, sha.getHashValues(), 8 * 4);
  std::memcpy(sha_state->buffer, sha.getBuffer(), 64);
  sha_state->count = uint32_t(sha.getTotalSize());
}
DECLARE_XBOXKRNL_EXPORT1(XeCryptSha256Update, kNone, kImplemented);

void XeCryptSha256Final(pointer_t<XECRYPT_SHA256_STATE> sha_state,
                        pointer_t<xe::be<uint32_t>> out, dword_t out_size) {
  sha256::SHA256 sha;
  std::memcpy(sha.getHashValues(), sha_state->state, 8 * 4);
  std::memcpy(sha.getBuffer(), sha_state->buffer, 64);
  sha.setTotalSize(sha_state->count);

  uint32_t hash[8];
  sha.getHash(reinterpret_cast<uint8_t*>(hash));

  std::memcpy(out, hash, std::min(uint32_t(out_size), 32u));
  std::memcpy(sha_state->buffer, hash, 32);
}
DECLARE_XBOXKRNL_EXPORT1(XeCryptSha256Final, kNone, kImplemented);

// Byteswap?
dword_result_t XeCryptBnQw_SwapDwQwLeBe(lpqword_t qw_inp, lpqword_t qw_out,
                                        dword_t size) {
  return 0;
}
DECLARE_XBOXKRNL_EXPORT1(XeCryptBnQw_SwapDwQwLeBe, kNone, kStub);

dword_result_t XeCryptBnQwNeRsaPubCrypt(lpqword_t qw_a, lpqword_t qw_b,
                                        lpvoid_t rsa) {
  // 0 indicates failure (but not a BOOL return value)
  return 1;
}
DECLARE_XBOXKRNL_EXPORT1(XeCryptBnQwNeRsaPubCrypt, kNone, kStub);

dword_result_t XeCryptBnDwLePkcs1Verify(lpvoid_t hash, lpvoid_t sig,
                                        dword_t size) {
  // BOOL return value
  return 1;
}
DECLARE_XBOXKRNL_EXPORT1(XeCryptBnDwLePkcs1Verify, kNone, kStub);

void XeCryptRandom(lpvoid_t buf, dword_t buf_size) {
  std::memset(buf, 0xFD, buf_size);
}
DECLARE_XBOXKRNL_EXPORT1(XeCryptRandom, kNone, kStub);

struct XECRYPT_DES_STATE {
  uint32_t keytab[16][2];
};

// Sets bit 0 to make the parity odd
void XeCryptDesParity(lpvoid_t inp, dword_t inp_size, lpvoid_t out_ptr) {
  DES::set_parity(inp, inp_size, out_ptr);
}
DECLARE_XBOXKRNL_EXPORT1(XeCryptDesParity, kNone, kImplemented);

struct XECRYPT_DES3_STATE {
  XECRYPT_DES_STATE des_state[3];
};

void XeCryptDes3Key(pointer_t<XECRYPT_DES3_STATE> state_ptr, lpqword_t key) {
  DES3 des3(key[0], key[1], key[2]);
  DES* des = des3.getDES();

  // Store our DES state into the state.
  for (int i = 0; i < 3; i++) {
    std::memcpy(state_ptr->des_state[i].keytab, des[i].get_sub_key(), 128);
  }
}
DECLARE_XBOXKRNL_EXPORT1(XeCryptDes3Key, kNone, kImplemented);

void XeCryptDes3Ecb(pointer_t<XECRYPT_DES3_STATE> state_ptr, lpqword_t inp,
                    lpqword_t out, dword_t encrypt) {
  DES3 des3((ui64*)state_ptr->des_state[0].keytab,
            (ui64*)state_ptr->des_state[1].keytab,
            (ui64*)state_ptr->des_state[2].keytab);

  if (encrypt) {
    *out = des3.encrypt(*inp);
  } else {
    *out = des3.decrypt(*inp);
  }
}
DECLARE_XBOXKRNL_EXPORT1(XeCryptDes3Ecb, kNone, kImplemented);

void XeCryptDes3Cbc(pointer_t<XECRYPT_DES3_STATE> state_ptr, lpqword_t inp,
                    dword_t inp_size, lpqword_t out, lpqword_t feed,
                    dword_t encrypt) {
  DES3 des3((ui64*)state_ptr->des_state[0].keytab,
            (ui64*)state_ptr->des_state[1].keytab,
            (ui64*)state_ptr->des_state[2].keytab);

  // DES can only do 8-byte chunks at a time!
  assert_true(inp_size % 8 == 0);

  uint64_t last_block = *feed;
  for (uint32_t i = 0; i < inp_size / 8; i++) {
    uint64_t block = inp[i];
    if (encrypt) {
      last_block = des3.encrypt(block ^ last_block);
      out[i] = last_block;
    } else {
      out[i] = des3.decrypt(block) ^ last_block;
      last_block = block;
    }
  }

  *feed = last_block;
}
DECLARE_XBOXKRNL_EXPORT1(XeCryptDes3Cbc, kNone, kImplemented);

void XeCryptHmacSha(lpvoid_t key, dword_t key_size_in, lpvoid_t inp_1,
                    dword_t inp_1_size, lpvoid_t inp_2, dword_t inp_2_size,
                    lpvoid_t inp_3, dword_t inp_3_size, lpvoid_t out,
                    dword_t out_size) {
  uint32_t key_size = key_size_in;
  sha1::SHA1 sha;
  uint8_t kpad_i[0x40];
  uint8_t kpad_o[0x40];
  uint8_t tmp_key[0x40];
  std::memset(kpad_i, 0x36, 0x40);
  std::memset(kpad_o, 0x5C, 0x40);

  // Setup HMAC key
  // If > block size, use its hash
  if (key_size > 0x40) {
    sha1::SHA1 sha_key;
    sha_key.processBytes(key, key_size);
    sha_key.finalize((uint8_t*)tmp_key);

    key_size = 0x14u;
  } else {
    std::memcpy(tmp_key, key, key_size);
  }

  for (uint32_t i = 0; i < key_size; i++) {
    kpad_i[i] = tmp_key[i] ^ 0x36;
    kpad_o[i] = tmp_key[i] ^ 0x5C;
  }

  // Inner
  sha.processBytes(kpad_i, 0x40);

  if (inp_1_size) {
    sha.processBytes(inp_1, inp_1_size);
  }

  if (inp_2_size) {
    sha.processBytes(inp_2, inp_2_size);
  }

  if (inp_3_size) {
    sha.processBytes(inp_3, inp_3_size);
  }

  uint8_t digest[0x14];
  sha.finalize(digest);
  sha.reset();

  // Outer
  sha.processBytes(kpad_o, 0x40);
  sha.processBytes(digest, 0x14);
  sha.finalize(digest);

  std::memcpy(out, digest, std::min((uint32_t)out_size, 0x14u));
}
DECLARE_XBOXKRNL_EXPORT1(XeCryptHmacSha, kNone, kImplemented);

// Keys
// TODO: Array of keys we need

// Retail key 0x19
static const uint8_t key19[] = {0xE1, 0xBC, 0x15, 0x9C, 0x73, 0xB1, 0xEA, 0xE9,
                                0xAB, 0x31, 0x70, 0xF3, 0xAD, 0x47, 0xEB, 0xF3};

dword_result_t XeKeysHmacSha(dword_t key_num, lpvoid_t inp_1,
                             dword_t inp_1_size, lpvoid_t inp_2,
                             dword_t inp_2_size, lpvoid_t inp_3,
                             dword_t inp_3_size, lpvoid_t out,
                             dword_t out_size) {
  const uint8_t* key = nullptr;
  if (key_num == 0x19) {
    key = key19;
  }

  if (key) {
    XeCryptHmacSha((void*)key, 0x10, inp_1, inp_1_size, inp_2, inp_2_size,
                   inp_3, inp_3_size, out, out_size);

    return X_STATUS_SUCCESS;
  }

  return X_STATUS_UNSUCCESSFUL;
}
DECLARE_XBOXKRNL_EXPORT1(XeKeysHmacSha, kNone, kImplemented);

void XeCryptAesKey(pointer_t<AES_ctx> aes_context, lpvoid_t key) {
  // AES_ctx isn't the actual AES struct being passed here
  // but since AES_ctx is smaller (192b vs 352b) it should be fine
  AES_init_ctx(aes_context, key);
}
DECLARE_XBOXKRNL_EXPORT1(XeCryptAesKey, kNone, kImplemented);

void XeCryptAesEcb(pointer_t<AES_ctx> aes_context, lpvoid_t input,
                   lpvoid_t output, dword_t encFlag) {
  memcpy(output.as<void*>(), input.as<void*>(),
         128 * 8);  // input/output are 128 bits/16 bytes
  if (encFlag) {
    AES_ECB_encrypt(aes_context, output.as<uint8_t*>());
  } else {
    AES_ECB_decrypt(aes_context, output.as<uint8_t*>());
  }
}
DECLARE_XBOXKRNL_EXPORT1(XeCryptAesEcb, kNone, kImplemented);

void XeCryptAesCbc(pointer_t<AES_ctx> aes_context, lpvoid_t input, dword_t size,
                   lpvoid_t output, lpvoid_t ivec, dword_t encFlag) {
  memcpy(output.as<void*>(), input.as<void*>(), size);
  if (ivec) {
    AES_ctx_set_iv(aes_context, ivec.as<uint8_t*>());
  }
  if (encFlag) {
    AES_CBC_encrypt_buffer(aes_context, output.as<uint8_t*>(), size);
  } else {
    AES_CBC_decrypt_buffer(aes_context, output.as<uint8_t*>(), size);
  }
}
DECLARE_XBOXKRNL_EXPORT1(XeCryptAesCbc, kNone, kImplemented);

void RegisterCryptExports(xe::cpu::ExportResolver* export_resolver,
                          KernelState* kernel_state) {}

}  // namespace xboxkrnl
}  // namespace kernel
}  // namespace xe
