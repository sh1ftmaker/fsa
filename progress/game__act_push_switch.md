# game/act_push_switch.cpp

Baseline (progress.py first line at start): total: 5819 / 11557 functions, 1165268 / 4442032 bytes of code (26.23 %)
After: total: 5821 / 11557 functions, 1165680 / 4442032 bytes of code (26.24 %)

Status: not Matching. 4/5 functions; update 96.65 %. gate: ok.

## Matched
dtor, draw, init (100 %), __sinit.

## Resistant
- update (scheduling+regalloc, 96.65 %, 6 tries): same instructions. Original: loop counter r29, player count r30,
  param-bits temporary r31 (reuses the register of the early field_0x4 load); ours: count r31, counter r30, bits r29.
  Tried: box by sums / addPos / hand-added corners in i.x,f.x,i.y,f.y order (last is what matches the prologue);
  bits as a local vs inline in the && chain (worse, 89.9 %).

## SHARED-EDITS
- include/game/Player.h: `bool fn_8022DB04(s32 player);` in the extern "C" block (owner tu_8022AD64; reads player
  slot byte 0xCAC, false without a manager; called by act_push_switch and act_somaria_block, clrlwi 24).
- config/G4SE01/gen_game.tsv: removed both act_push_switch rows (dtor, sinit).
- include/game/act/act_push_switch.h (generator-owned): added getLength().

## Next
update's register allocation; maybe an inline player-in-box helper changes live ranges.
