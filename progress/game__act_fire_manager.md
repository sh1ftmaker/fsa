# game/act_fire_manager

Baseline: total: 5839 / 11557 functions, 1169096 / 4442032 bytes of code (26.32 %)

Functions: 2/4 before (dtor, generated __sinit) -> 3/4 after.

## Matched
- update__20FireManager_804AB2B0Fv: 100% with `(s32)((s32)field_0x245 * rand)` (rules card shape).

## Resistant
- init__20FireManager_804AB2B0Fv 99.84%, scheduling: `lis r0,0x4330` / `lis r4,0x19` order swapped.

## SHARED-EDITS
- include/game/act/act_fire_manager.h (generator-owned): members 0x230-0x255; config/G4SE01/gen_game.tsv: removed both act_fire_manager rows.

## Next
Init lis scheduling.
