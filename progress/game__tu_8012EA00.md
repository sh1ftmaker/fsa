# game/tu_8012EA00

Baseline (progress.py): total 5820 / 11557 functions, 1165664 / 4442032 bytes (26.24 %).
After: 5821 / 11557, 1165828 bytes (26.25 %).

Matched: ~cls_8049E8F8, init, __sinit. update at 98.51 %.

Resistant: update__12cls_8049E8F8Fv (missing/extra, 98.51 %).
- Player x/y loaded into f1/f0 in the target, f0/f1 here (compare chain otherwise identical).
- The inlined switch in setState: target cmpwi 1/beq/bge then cmpwi 3/bge; ours cmpwi 2 first.
  Tried: case order swap, default, empty cases.

SHARED-EDITS
- include/game/Player.h: fn_8023EA28(s32)->bool, fn_8023561C(s32)->bool (extern "C", owner tu_8022AD64).
- include/game/GameGlobals.h: class Unk_80119998 (member fn_80119998), extern "C" fn_80119DD0() (owner tu_80118384).
- config/G4SE01/symbols.txt: fn_80119998 -> fn_80119998__12Unk_80119998Flllllll.
- config/G4SE01/gen_game.tsv: removed this unit's 2 rows.

Next: setState switch shape and x/y register order.
