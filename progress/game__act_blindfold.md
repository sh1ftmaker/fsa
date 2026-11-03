# game/act_blindfold

Baseline: total 6046 / 11557 functions, 1220300 bytes (27.47 %). After: 6048 functions, 1221472 bytes (27.50 %).

Matched: dtor, update (100 %), init (100 %), __sinit (generated; gen_game.tsv rows deleted).
Resistant: draw 94.2 % (scheduling+regalloc). Same instructions; the original forms the lis of the
"black.bti" address and loads the 0x23c conversion into r4 earlier, 0xff/0 in r8/r7. 9 tries used.

SHARED-EDITS
- include/game/ActorBase.h: SpriteParams_801F5AE0::field_0x1c u8 -> s8 (fn_8023CA88 returns s8; the
  original stores a shared -1 register). Re-tried the 11 units using SpriteParams: unchanged.
- include/game/Player.h: s32 fn_8022EA0C(s32 player) (extern "C", owner tu_8022AD64; r3 in/out, asm).
- config/G4SE01/gen_game.tsv: act_blindfold rows removed; include/game/act/act_blindfold.h fields added.

Next: draw allocation; update's else branch duplicates the loop body as in the original.
