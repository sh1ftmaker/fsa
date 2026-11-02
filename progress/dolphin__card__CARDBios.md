# dolphin/card/CARDBios.c

Status: Matching. 24/24 functions, .text/.data/.bss/.sdata/.sbss all 100 %.

No source changes were needed; all misses were splits/symbols:
- splits: .text start moved to 0x800522B4 (picks up __CARDDefaultApiCallback, was auto-split),
  added .data 0x80496AA8-0x80496B00 (version string + ResetFunctionInfo) and
  .bss 0x804B5BA0-0x804B5DE0 (__CARDBlock[2] + __CARDDiskNone).
- symbols: fn_800522B4 -> __CARDDefaultApiCallback, fn_800531D4 -> __CARDGetFontEncode,
  fn_80053518 -> OnReset (local), lbl_80496AF0 -> ResetFunctionInfo (local),
  lbl_804B5BA0 -> __CARDBlock, lbl_804B5DC0 -> __CARDDiskNone.

SHARED-EDITS: none.
