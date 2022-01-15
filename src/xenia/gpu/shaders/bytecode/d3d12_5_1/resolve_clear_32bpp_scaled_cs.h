#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Buffer Definitions: 
//
// cbuffer XeResolveConstants
// {
//
//   uint2 xe_resolve_clear_value;      // Offset:    0 Size:     8
//   uint xe_resolve_edram_info;        // Offset:    8 Size:     4
//   uint xe_resolve_address_info;      // Offset:   12 Size:     4
//
// }
//
// cbuffer XeResolveResolutionScaleConstant
// {
//
//   uint xe_resolve_resolution_scale;  // Offset:    0 Size:     4
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      ID      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- ------- -------------- ------
// xe_resolve_dest                       UAV   uint4         buf      U0             u0      1 
// XeResolveConstants                cbuffer      NA          NA     CB0            cb0      1 
// XeResolveResolutionScaleConstant    cbuffer      NA          NA     CB1            cb1      1 
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// no Input
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// no Output
cs_5_1
dcl_globalFlags refactoringAllowed
dcl_constantbuffer CB0[0:0][1], immediateIndexed, space=0
dcl_constantbuffer CB1[1:1][1], immediateIndexed, space=0
dcl_uav_typed_buffer (uint,uint,uint,uint) U0[0:0], space=0
dcl_input vThreadID.xy
dcl_temps 3
dcl_thread_group 8, 8, 1
ubfe r0.xy, l(2, 11, 0, 0), l(10, 7, 0, 0), CB0[0][0].zwzz
uge r0.xz, r0.xxxx, l(2, 0, 1, 0)
and r0.w, r0.x, l(1)
and r1.x, CB1[1][0].x, l(3)
imul null, r0.y, r0.y, r1.x
ishl r0.y, r0.y, r0.w
uge r0.y, vThreadID.x, r0.y
if_nz r0.y
  ret 
endif 
ishl r1.x, vThreadID.x, l(3)
ushr r2.y, CB0[0][0].w, l(5)
mov r2.x, CB0[0][0].w
and r0.yw, r2.xxxy, l(0, 31, 0, 3)
ushr r2.y, CB1[1][0].x, l(2)
mov r2.x, CB1[1][0].x
and r1.zw, r2.xxxy, l(0, 0, 3, 3)
imul null, r0.yw, r0.yyyw, r1.zzzw
movc r0.xz, r0.xxzx, l(4,0,4,0), l(3,0,3,0)
ishl r0.xy, r0.ywyy, r0.xzxx
mov r1.y, vThreadID.y
iadd r0.xy, r0.xyxx, r1.xyxx
ubfe r0.z, l(12), l(13), CB0[0][0].z
and r1.xy, CB0[0][0].zzzz, l(1023, 4096, 0, 0)
imul null, r1.zw, r1.zzzw, l(0, 0, 80, 16)
udiv r2.xy, null, r0.xyxx, r1.zwzz
imad r0.w, r2.y, r1.x, r2.x
iadd r0.z, r0.w, r0.z
imad r0.xy, -r2.xyxx, r1.zwzz, r0.xyxx
if_nz r1.y
  ushr r0.w, r1.z, l(1)
  uge r1.x, r0.x, r0.w
  ineg r1.y, r0.w
  movc r0.w, r1.x, r1.y, r0.w
  iadd r0.x, r0.w, r0.x
endif 
imul null, r0.w, r1.w, r1.z
imad r0.x, r0.y, r1.z, r0.x
imad r0.x, r0.z, r0.w, r0.x
ushr r0.x, r0.x, l(2)
store_uav_typed U0[0].xyzw, r0.xxxx, CB0[0][0].xxxx
iadd r0.y, r0.x, l(1)
store_uav_typed U0[0].xyzw, r0.yyyy, CB0[0][0].xxxx
ret 
// Approximately 44 instruction slots used
#endif

const BYTE resolve_clear_32bpp_scaled_cs[] =
{
     68,  88,  66,  67, 151, 173, 
    156, 244, 183, 241, 226,  31, 
    214, 214,  54, 114,  10, 217, 
    153, 194,   1,   0,   0,   0, 
    104,   9,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
    228,   2,   0,   0, 244,   2, 
      0,   0,   4,   3,   0,   0, 
    204,   8,   0,   0,  82,  68, 
     69,  70, 168,   2,   0,   0, 
      2,   0,   0,   0, 248,   0, 
      0,   0,   3,   0,   0,   0, 
     60,   0,   0,   0,   1,   5, 
     83,  67,   0,   5,   4,   0, 
    128,   2,   0,   0,  19,  19, 
     68,  37,  60,   0,   0,   0, 
     24,   0,   0,   0,  40,   0, 
      0,   0,  40,   0,   0,   0, 
     36,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
    180,   0,   0,   0,   4,   0, 
      0,   0,   4,   0,   0,   0, 
      1,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
      1,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 196,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 215,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
    120, 101,  95, 114, 101, 115, 
    111, 108, 118, 101,  95, 100, 
    101, 115, 116,   0,  88, 101, 
     82, 101, 115, 111, 108, 118, 
    101,  67, 111, 110, 115, 116, 
     97, 110, 116, 115,   0,  88, 
    101,  82, 101, 115, 111, 108, 
    118, 101,  82, 101, 115, 111, 
    108, 117, 116, 105, 111, 110, 
     83,  99,  97, 108, 101,  67, 
    111, 110, 115, 116,  97, 110, 
    116,   0, 196,   0,   0,   0, 
      3,   0,   0,   0,  40,   1, 
      0,   0,  16,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 215,   0,   0,   0, 
      1,   0,   0,   0,  60,   2, 
      0,   0,  16,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 160,   1,   0,   0, 
      0,   0,   0,   0,   8,   0, 
      0,   0,   2,   0,   0,   0, 
    192,   1,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    228,   1,   0,   0,   8,   0, 
      0,   0,   4,   0,   0,   0, 
      2,   0,   0,   0,   0,   2, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,  36,   2, 
      0,   0,  12,   0,   0,   0, 
      4,   0,   0,   0,   2,   0, 
      0,   0,   0,   2,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 120, 101,  95, 114, 
    101, 115, 111, 108, 118, 101, 
     95,  99, 108, 101,  97, 114, 
     95, 118,  97, 108, 117, 101, 
      0, 117, 105, 110, 116,  50, 
      0, 171, 171, 171,   1,   0, 
     19,   0,   1,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    183,   1,   0,   0, 120, 101, 
     95, 114, 101, 115, 111, 108, 
    118, 101,  95, 101, 100, 114, 
     97, 109,  95, 105, 110, 102, 
    111,   0, 100, 119, 111, 114, 
    100,   0,   0,   0,  19,   0, 
      1,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 250,   1, 
      0,   0, 120, 101,  95, 114, 
    101, 115, 111, 108, 118, 101, 
     95,  97, 100, 100, 114, 101, 
    115, 115,  95, 105, 110, 102, 
    111,   0, 100,   2,   0,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,   2,   0,   0,   0, 
      0,   2,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    120, 101,  95, 114, 101, 115, 
    111, 108, 118, 101,  95, 114, 
    101, 115, 111, 108, 117, 116, 
    105, 111, 110,  95, 115,  99, 
     97, 108, 101,   0,  77, 105, 
     99, 114, 111, 115, 111, 102, 
    116,  32,  40,  82,  41,  32, 
     72,  76,  83,  76,  32,  83, 
    104,  97, 100, 101, 114,  32, 
     67, 111, 109, 112, 105, 108, 
    101, 114,  32,  49,  48,  46, 
     49,   0,  73,  83,  71,  78, 
      8,   0,   0,   0,   0,   0, 
      0,   0,   8,   0,   0,   0, 
     79,  83,  71,  78,   8,   0, 
      0,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,  83,  72, 
     69,  88, 192,   5,   0,   0, 
     81,   0,   5,   0, 112,   1, 
      0,   0, 106,   8,   0,   1, 
     89,   0,   0,   7,  70, 142, 
     48,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,  89,   0, 
      0,   7,  70, 142,  48,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0, 156,   8,   0,   7, 
     70, 238,  49,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  68,  68, 
      0,   0,   0,   0,   0,   0, 
     95,   0,   0,   2,  50,   0, 
      2,   0, 104,   0,   0,   2, 
      3,   0,   0,   0, 155,   0, 
      0,   4,   8,   0,   0,   0, 
      8,   0,   0,   0,   1,   0, 
      0,   0, 138,   0,   0,  17, 
     50,   0,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      2,   0,   0,   0,  11,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,  10,   0,   0,   0, 
      7,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    230, 138,  48,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  80,   0, 
      0,  10,  82,   0,  16,   0, 
      0,   0,   0,   0,   6,   0, 
     16,   0,   0,   0,   0,   0, 
      2,  64,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   7, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   9,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     10, 128,  48,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   3,   0,   0,   0, 
     38,   0,   0,   8,   0, 208, 
      0,   0,  34,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   1,   0, 
      0,   0,  41,   0,   0,   7, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     80,   0,   0,   6,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,   2,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     31,   0,   4,   3,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     62,   0,   0,   1,  21,   0, 
      0,   1,  41,   0,   0,   6, 
     18,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,   2,   0, 
      1,  64,   0,   0,   3,   0, 
      0,   0,  85,   0,   0,   9, 
     34,   0,  16,   0,   2,   0, 
      0,   0,  58, 128,  48,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   5,   0, 
      0,   0,  54,   0,   0,   7, 
     18,   0,  16,   0,   2,   0, 
      0,   0,  58, 128,  48,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,  10, 162,   0, 
     16,   0,   0,   0,   0,   0, 
      6,   4,  16,   0,   2,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,  31,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,  85,   0, 
      0,   9,  34,   0,  16,   0, 
      2,   0,   0,   0,  10, 128, 
     48,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      2,   0,   0,   0,  54,   0, 
      0,   7,  18,   0,  16,   0, 
      2,   0,   0,   0,  10, 128, 
     48,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,  10, 
    194,   0,  16,   0,   1,   0, 
      0,   0,   6,   4,  16,   0, 
      2,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
     38,   0,   0,   8,   0, 208, 
      0,   0, 162,   0,  16,   0, 
      0,   0,   0,   0,  86,  13, 
     16,   0,   0,   0,   0,   0, 
    166,  14,  16,   0,   1,   0, 
      0,   0,  55,   0,   0,  15, 
     82,   0,  16,   0,   0,   0, 
      0,   0,   6,   2,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      2,  64,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  41,   0,   0,   7, 
     50,   0,  16,   0,   0,   0, 
      0,   0, 214,   5,  16,   0, 
      0,   0,   0,   0, 134,   0, 
     16,   0,   0,   0,   0,   0, 
     54,   0,   0,   4,  34,   0, 
     16,   0,   1,   0,   0,   0, 
     26,   0,   2,   0,  30,   0, 
      0,   7,  50,   0,  16,   0, 
      0,   0,   0,   0,  70,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   0,  16,   0,   1,   0, 
      0,   0, 138,   0,   0,  11, 
     66,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
     12,   0,   0,   0,   1,  64, 
      0,   0,  13,   0,   0,   0, 
     42, 128,  48,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,  12,  50,   0,  16,   0, 
      1,   0,   0,   0, 166, 138, 
     48,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
    255,   3,   0,   0,   0,  16, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  38,   0, 
      0,  11,   0, 208,   0,   0, 
    194,   0,  16,   0,   1,   0, 
      0,   0, 166,  14,  16,   0, 
      1,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  80,   0, 
      0,   0,  16,   0,   0,   0, 
     78,   0,   0,   8,  50,   0, 
     16,   0,   2,   0,   0,   0, 
      0, 208,   0,   0,  70,   0, 
     16,   0,   0,   0,   0,   0, 
    230,  10,  16,   0,   1,   0, 
      0,   0,  35,   0,   0,   9, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      2,   0,   0,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   2,   0, 
      0,   0,  30,   0,   0,   7, 
     66,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     35,   0,   0,  10,  50,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   0,  16, 128,  65,   0, 
      0,   0,   2,   0,   0,   0, 
    230,  10,  16,   0,   1,   0, 
      0,   0,  70,   0,  16,   0, 
      0,   0,   0,   0,  31,   0, 
      4,   3,  26,   0,  16,   0, 
      1,   0,   0,   0,  85,   0, 
      0,   7, 130,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   1,   0, 
      0,   0,  80,   0,   0,   7, 
     18,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     40,   0,   0,   5,  34,   0, 
     16,   0,   1,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  55,   0,   0,   9, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   1,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  30,   0,   0,   7, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     21,   0,   0,   1,  38,   0, 
      0,   8,   0, 208,   0,   0, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      1,   0,   0,   0,  42,   0, 
     16,   0,   1,   0,   0,   0, 
     35,   0,   0,   9,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     35,   0,   0,   9,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     85,   0,   0,   7,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      2,   0,   0,   0, 164,   0, 
      0,  10, 242, 224,  33,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   6,   0,  16,   0, 
      0,   0,   0,   0,   6, 128, 
     48,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  30,   0,   0,   7, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   1,   0,   0,   0, 
    164,   0,   0,  10, 242, 224, 
     33,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  86,   5, 
     16,   0,   0,   0,   0,   0, 
      6, 128,  48,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  62,   0, 
      0,   1,  83,  84,  65,  84, 
    148,   0,   0,   0,  44,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     16,   0,   0,   0,  13,   0, 
      0,   0,   2,   0,   0,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0
};
