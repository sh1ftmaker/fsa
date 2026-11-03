# game/act_burning_block

Baseline: total 6007 / 11557 functions, 1208940 bytes of code.
After: 6011 / 11557, 1210452 bytes. Unit 6/6 functions match.

Matched: all (dtor, vfunc_0x24, draw, update, init, __sinit).
Resistant: none.
SHARED-EDITS: include/game/Player.h (fn_80239108, fn_80238BC4, fn_80238E08 declared,
owner tu_8022AD64); include/game/Map.h (fn_802260DC takes TVec2f*, reads +0/+4 only,
no other callers); config/G4SE01/gen_game.tsv (both act_burning_block rows removed).
Next: coordinator trial link / Matching flag.
