test_vupkhsh_1_GEN:
  #_ REGISTER_IN v1 [00000000, 00000000, 00000000, 00000000]
  vupkhsh v2, v1
  blr
  #_ REGISTER_OUT v1 [00000000, 00000000, 00000000, 00000000]
  #_ REGISTER_OUT v2 [00000000, 00000000, 00000000, 00000000]

test_vupkhsh_2_GEN:
  #_ REGISTER_IN v1 [00000001, 00000001, 00000001, 00000001]
  vupkhsh v2, v1
  blr
  #_ REGISTER_OUT v1 [00000001, 00000001, 00000001, 00000001]
  #_ REGISTER_OUT v2 [00000000, 00000001, 00000000, 00000001]

test_vupkhsh_3_GEN:
  #_ REGISTER_IN v1 [0000FFFF, FFFF0000, 00000000, FFFF0000]
  vupkhsh v2, v1
  blr
  #_ REGISTER_OUT v1 [0000FFFF, FFFF0000, 00000000, FFFF0000]
  #_ REGISTER_OUT v2 [00000000, FFFFFFFF, FFFFFFFF, 00000000]

test_vupkhsh_4_GEN:
  #_ REGISTER_IN v1 [00010203, 04050607, 08090A0B, 0C0D0E0F]
  vupkhsh v2, v1
  blr
  #_ REGISTER_OUT v1 [00010203, 04050607, 08090A0B, 0C0D0E0F]
  #_ REGISTER_OUT v2 [00000001, 00000203, 00000405, 00000607]

test_vupkhsh_5_GEN:
  #_ REGISTER_IN v1 [000D000D, 000D000D, 000D000D, 000D000D]
  vupkhsh v2, v1
  blr
  #_ REGISTER_OUT v1 [000D000D, 000D000D, 000D000D, 000D000D]
  #_ REGISTER_OUT v2 [0000000D, 0000000D, 0000000D, 0000000D]

test_vupkhsh_6_GEN:
  #_ REGISTER_IN v1 [00112233, 44556677, 8899AABB, CCDDEEFF]
  vupkhsh v2, v1
  blr
  #_ REGISTER_OUT v1 [00112233, 44556677, 8899AABB, CCDDEEFF]
  #_ REGISTER_OUT v2 [00000011, 00002233, 00004455, 00006677]

test_vupkhsh_7_GEN:
  #_ REGISTER_IN v1 [00FFFF00, 0000FF00, FF0000FF, FFFF00FF]
  vupkhsh v2, v1
  blr
  #_ REGISTER_OUT v1 [00FFFF00, 0000FF00, FF0000FF, FFFF00FF]
  #_ REGISTER_OUT v2 [000000FF, FFFFFF00, 00000000, FFFFFF00]

test_vupkhsh_8_GEN:
  #_ REGISTER_IN v1 [04040404, 04040404, 04040404, 04040404]
  vupkhsh v2, v1
  blr
  #_ REGISTER_OUT v1 [04040404, 04040404, 04040404, 04040404]
  #_ REGISTER_OUT v2 [00000404, 00000404, 00000404, 00000404]

test_vupkhsh_9_GEN:
  #_ REGISTER_IN v1 [07070707, 07070707, 07070707, 07070707]
  vupkhsh v2, v1
  blr
  #_ REGISTER_OUT v1 [07070707, 07070707, 07070707, 07070707]
  #_ REGISTER_OUT v2 [00000707, 00000707, 00000707, 00000707]

test_vupkhsh_10_GEN:
  #_ REGISTER_IN v1 [08080808, 08080808, 08080808, 08080808]
  vupkhsh v2, v1
  blr
  #_ REGISTER_OUT v1 [08080808, 08080808, 08080808, 08080808]
  #_ REGISTER_OUT v2 [00000808, 00000808, 00000808, 00000808]

test_vupkhsh_11_GEN:
  #_ REGISTER_IN v1 [12121212, 12121212, 12121212, 12121212]
  vupkhsh v2, v1
  blr
  #_ REGISTER_OUT v1 [12121212, 12121212, 12121212, 12121212]
  #_ REGISTER_OUT v2 [00001212, 00001212, 00001212, 00001212]

test_vupkhsh_12_GEN:
  #_ REGISTER_IN v1 [12345678, 87654321, 11223344, 55667788]
  vupkhsh v2, v1
  blr
  #_ REGISTER_OUT v1 [12345678, 87654321, 11223344, 55667788]
  #_ REGISTER_OUT v2 [00001234, 00005678, FFFF8765, 00004321]

test_vupkhsh_13_GEN:
  #_ REGISTER_IN v1 [3F800000, 3FC00000, 3F8CCCCD, 3FF33333]
  vupkhsh v2, v1
  blr
  #_ REGISTER_OUT v1 [3F800000, 3FC00000, 3F8CCCCD, 3FF33333]
  #_ REGISTER_OUT v2 [00003F80, 00000000, 00003FC0, 00000000]

test_vupkhsh_14_GEN:
  #_ REGISTER_IN v1 [41200000, C1200000, 41700000, C1700000]
  vupkhsh v2, v1
  blr
  #_ REGISTER_OUT v1 [41200000, C1200000, 41700000, C1700000]
  #_ REGISTER_OUT v2 [00004120, 00000000, FFFFC120, 00000000]

test_vupkhsh_15_GEN:
  #_ REGISTER_IN v1 [7F800203, 04050607, 7F800A0B, 0C0D0E0F]
  vupkhsh v2, v1
  blr
  #_ REGISTER_OUT v1 [7F800203, 04050607, 7F800A0B, 0C0D0E0F]
  #_ REGISTER_OUT v2 [00007F80, 00000203, 00000405, 00000607]

test_vupkhsh_16_GEN:
  #_ REGISTER_IN v1 [80081010, 808F0000, 7FFFFFFF, 8FFFFFFF]
  vupkhsh v2, v1
  blr
  #_ REGISTER_OUT v1 [80081010, 808F0000, 7FFFFFFF, 8FFFFFFF]
  #_ REGISTER_OUT v2 [FFFF8008, 00001010, FFFF808F, 00000000]

test_vupkhsh_17_GEN:
  #_ REGISTER_IN v1 [BF800000, BFC00000, BF8CCCCD, BFF33333]
  vupkhsh v2, v1
  blr
  #_ REGISTER_OUT v1 [BF800000, BFC00000, BF8CCCCD, BFF33333]
  #_ REGISTER_OUT v2 [FFFFBF80, 00000000, FFFFBFC0, 00000000]

test_vupkhsh_18_GEN:
  #_ REGISTER_IN v1 [C1200000, 41A00000, C1A00000, 41F00000]
  vupkhsh v2, v1
  blr
  #_ REGISTER_OUT v1 [C1200000, 41A00000, C1A00000, 41F00000]
  #_ REGISTER_OUT v2 [FFFFC120, 00000000, 000041A0, 00000000]

test_vupkhsh_19_GEN:
  #_ REGISTER_IN v1 [CDCDCDCD, CDCDCDCD, CDCDCDCD, 04010203]
  vupkhsh v2, v1
  blr
  #_ REGISTER_OUT v1 [CDCDCDCD, CDCDCDCD, CDCDCDCD, 04010203]
  #_ REGISTER_OUT v2 [FFFFCDCD, FFFFCDCD, FFFFCDCD, FFFFCDCD]

test_vupkhsh_20_GEN:
  #_ REGISTER_IN v1 [F8F9FAFB, FCFDFEFF, 00010203, 04050607]
  vupkhsh v2, v1
  blr
  #_ REGISTER_OUT v1 [F8F9FAFB, FCFDFEFF, 00010203, 04050607]
  #_ REGISTER_OUT v2 [FFFFF8F9, FFFFFAFB, FFFFFCFD, FFFFFEFF]

test_vupkhsh_21_GEN:
  #_ REGISTER_IN v1 [FEFEFEFE, FEFEFEFE, FEFEFEFE, FEFEFEFE]
  vupkhsh v2, v1
  blr
  #_ REGISTER_OUT v1 [FEFEFEFE, FEFEFEFE, FEFEFEFE, FEFEFEFE]
  #_ REGISTER_OUT v2 [FFFFFEFE, FFFFFEFE, FFFFFEFE, FFFFFEFE]

test_vupkhsh_22_GEN:
  #_ REGISTER_IN v1 [FFFCFFFD, FFFEFFFF, 00000001, 00020003]
  vupkhsh v2, v1
  blr
  #_ REGISTER_OUT v1 [FFFCFFFD, FFFEFFFF, 00000001, 00020003]
  #_ REGISTER_OUT v2 [FFFFFFFC, FFFFFFFD, FFFFFFFE, FFFFFFFF]

test_vupkhsh_23_GEN:
  #_ REGISTER_IN v1 [FFFDFF7E, 00020081, FFFCFF7D, 00030082]
  vupkhsh v2, v1
  blr
  #_ REGISTER_OUT v1 [FFFDFF7E, 00020081, FFFCFF7D, 00030082]
  #_ REGISTER_OUT v2 [FFFFFFFD, FFFFFF7E, 00000002, 00000081]

test_vupkhsh_24_GEN:
  #_ REGISTER_IN v1 [FFFF0101, 7070FFFF, FFFFFFFF, 00000000]
  vupkhsh v2, v1
  blr
  #_ REGISTER_OUT v1 [FFFF0101, 7070FFFF, FFFFFFFF, 00000000]
  #_ REGISTER_OUT v2 [FFFFFFFF, 00000101, 00007070, FFFFFFFF]

test_vupkhsh_25_GEN:
  #_ REGISTER_IN v1 [FFFFFF80, 0000007F, FFFEFF7F, 00010080]
  vupkhsh v2, v1
  blr
  #_ REGISTER_OUT v1 [FFFFFF80, 0000007F, FFFEFF7F, 00010080]
  #_ REGISTER_OUT v2 [FFFFFFFF, FFFFFF80, 00000000, 0000007F]

test_vupkhsh_26_GEN:
  #_ REGISTER_IN v1 [FFFFFFFF, FFFFFFFF, FFFFFFFF, FFFFFFFF]
  vupkhsh v2, v1
  blr
  #_ REGISTER_OUT v1 [FFFFFFFF, FFFFFFFF, FFFFFFFF, FFFFFFFF]
  #_ REGISTER_OUT v2 [FFFFFFFF, FFFFFFFF, FFFFFFFF, FFFFFFFF]

