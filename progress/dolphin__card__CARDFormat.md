# dolphin/card/CARDFormat.c

Status: Matching (3/3 functions, .text 100 %). CARDFormatAsync is not in the
target (dead-stripped); kept in source as in donors.

No code changes; all misses were symbol names in config/G4SE01/symbols.txt:
- lbl_804B5BA0 -> __CARDBlock, lbl_804B5DC0 -> __CARDDiskNone,
  fn_800522B4 -> __CARDDefaultApiCallback, fn_800531D4 -> __CARDGetFontEncode
  (same renames as the CARDBios/other CARD units).
- fn_8006791C -> __shr2i: runtime.c sequence __div2u(0xEC) fn_800675AC(0x138)
  __mod2u(0xE4) fn_800677C8(0x10C) fn_800678D4(0x24) fn_800678F8(0x24)
  fn_8006791C(0x28) fn_80067944(0xCC) matches mkdd/pikmin2 exactly
  (__div2i, __mod2i, __shl2i, __shr2u, __shr2i, __cvt_dbl_usll); body uses sraw.
  Only __shr2i renamed here; the others are candidates for the runtime.c owner.
