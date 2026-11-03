# game/act_thwomp checkpoint

Baseline: total: 5901 / 11557 functions, 1181784 / 4442032 bytes of code (26.60 %)
After: total: 5903 / 11557 functions, 1182072 bytes (26.61 %)

Matched: __dt__, draw, init, __sinit (4/5).
Resistant: update (98.43 %, different). Frame is 0x40 not 0x30 because fn_80238548 takes a TVec3f* while the original passes an 8-byte stack object at 0x8; `li` of `hit` scheduled 2 insns early. Hunk: stwu r1,-0x30 vs -0x40, box at 0x10 vs 0x14.
SHARED-EDITS: include/game/Player.h (fn_8023B190, fn_802399D8, fn_80239718 declared, owner tu_8022AD64); config/G4SE01/gen_game.tsv (act_thwomp rows 2 deleted).
Next: if fn_80238548 ever gets a TVec2f* parameter, p becomes TVec2f and update should match.
