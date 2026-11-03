# game/act_fire_manager

Baseline: total: 5839 / 11557 functions, 1169096 / 4442032 bytes of code (26.32 %)

Functions: 2/4 before (dtor, generated __sinit) -> 2/4 after; update and init written, both resistant.

## Resistant
- update__12cls_804AB2B0Fv 97.7%: the u8 field_0x245 -> float conversion in `field_0x244 + (s32)(field_0x245 * rand)`
  is the signed form in the original (xoris 0x8000, lbl_80541AC0 = 2^52+2^31); u8 fields compile the unsigned form.
  A local `s32 range = field_0x245;` gives the signed form and sdata2 100% but a different schedule (83%) and is a steering temp.
- init__12cls_804AB2B0Fv 99.8%: `lis r0,0x4330` / `lis r4,0x19` order swapped.
- .sdata2 87.9% (missing the signed 2^52+2^31 double) follows from update.

## SHARED-EDITS
- include/game/act/act_fire_manager.h (generator-owned): members 0x230-0x255; config/G4SE01/gen_game.tsv: removed both act_fire_manager rows.

## Next
Find the construct giving the signed u8->float in update (bitfield member?) .
