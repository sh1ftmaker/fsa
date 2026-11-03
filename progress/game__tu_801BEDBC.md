# game/tu_801BEDBC.cpp

Baseline (progress.py first line at start): total: 6426 / 11557 functions, 1284740 / 4442032 bytes of code (28.92 %)
After: total: 6430 / 11557 functions, 1285380 / 4442032 bytes of code (28.94 %)

Status: not Matching. 6/8 functions (2/8 before).

## Matched
fn_801BEDBC (now cls_804A4188::fn_801BEDBC), vfunc_0x54, init, dtor, ctor (fn_801BF5A8 -> __ct__), __sinit.

## Resistant
- vfunc_0x4C 91.0 % scheduling+regalloc: same body as ActorBase::vfunc_0x4C (tu_801EE24C, also a FIXME): saved registers differ (target this r31, result r30, layer r29, pos r28).
- update 98.25 % missing/extra: target reloads mFlags for the STARTED test (we reuse the first load), layer in r29 (ours r30), mPos.x in f2 (ours f3), probe operands loaded y, dy, x, dx (ours dx first).
  Stack layout matches (probes 0x38..0x14, loop point at 0x8) via inline helpers blockedAt/burnTiles.
- .sdata2 94.74 % / extab 59 %: bytes identical to the target; relocation names only.

## SHARED-EDITS
- config/G4SE01/gen_game.tsv: both tu_801BEDBC rows deleted.
- include/game/GameGlobals.h: declared fn_801CCC78 (defined in tu_801CB270.cpp).
- include/game/BgRewrite.h: member fn_802A20F0(pos, player, layer, bool, bool); symbols.txt renamed to the mangled name (last two args are li 0/1: guess bool).
- symbols.txt: fn_801BEDBC -> fn_801BEDBC__12cls_804A4188Fv (callers act_armos, act_on_off_switch pass an actor found by serial), fn_801BF5A8 -> __ct__12cls_804A4188Fv.

## Next
Try other shapes for the mFlags reload and the layer register in update.
