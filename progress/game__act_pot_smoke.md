# game/act_pot_smoke

Baseline: `total: 5866 / 11557 functions, 1174344 / 4442032 bytes of code (26.44 %)`

Matched: all 5 functions (dtor, draw, update, init, __sinit). Code 1764/1764.
Resistant: none. Data 372/440: extab/extabindex (weak-duplicate JAS pool dtor
entries), .sdata2 lacks the second float (0.0f) of the original's 8 bytes; no
function in the unit uses it.
SHARED-EDITS: config/G4SE01/gen_game.tsv (removed both act_pot_smoke rows),
include/game/act/act_pot_smoke.h (members 0x230-0x240 named).
