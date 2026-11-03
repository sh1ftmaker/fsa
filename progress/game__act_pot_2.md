# game/act_pot_2.cpp

Baseline: total: 5819 / 11557 functions, 1165268 / 4442032 bytes of code (26.23 %)

Matched: dtor, draw, init, __sinit. Resistant: update (80.45 %, missing/extra): target copies
field_0x80 into the stack box with lwz/stw pairs before loading mPos; we use lfs/stfs.
Tried: copy-init, assignment, per-member copy, addPos(vec), addPos(x,y), hand adds.
SHARED-EDITS: config/G4SE01/gen_game.tsv rows for act_pot_2 removed (generator-owned).
