# game/act_eyeball_switch (MEDM, cls_804AE678)

Baseline: total: 6426 / 11557 functions, 1284740 / 4442032 bytes of code (28.92 %)
After:    total: 6427 / 11557 functions, 1284864 / 4442032 bytes of code (28.93 %)

Functions 2/6 -> 3/6 (vfunc_0x24, __dt, __sinit match; Matching flag not set).

## Resistant
- update 99.13 % (scheduling + reloc-name): target loads fn_802D78B4's bit
  (0x248) after the 0x244 byte; .sdata2 literal order 1.0f, 32.0f in the target.
- vfunc_0x10 98.54 % (regalloc-only): this/end and params/table/params>>27 saved regs.
- init 97.05 % (different): (u8)(players - 2) <= 1 test.

## SHARED-EDITS
- include/game/Player.h: fn_8023BA68(box, layer, TVec2f* out, bool) (tu_8022AD64),
  from fn_80273C30's call to fn_80274238(box, layer, TVec2f*, bool).
- symbols.txt: renamed lbl_804AE678 -> __vt__12cls_804AE678 and the four fn_8037xxxx
  placeholders of this unit to update/__dt/vfunc_0x10/init of cls_804AE678.
