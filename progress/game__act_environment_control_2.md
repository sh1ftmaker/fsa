# game/act_environment_control_2
Baseline (progress.py): 6046 / 11557 functions, 1220300 bytes (27.47 %). After: 6051 / 11557, 1221948 bytes.

Matched: all 6 (setter, draw, update, dtor, init, generated __sinit). Only extab/extabindex < 100 % (weak entries).
Resistant: none.
Note: update's single `fn_8016A3AC()->x = 1` stores need a local pointer (`shared`) or the compiler hoists `li 1` into r30 before the call.
SHARED-EDITS: include/game/GameGlobals.h (GameRootSub20_801902C0 layout + new GameRootSub24_8016A3AC / fn_8016A3AC decl); gen_game.tsv row removed.
Next: set Matching (coordinator).
