# game/act_off_switch
Baseline: total 5751 / 11557 functions, 1155176 bytes (26.01 %). After: 5753, 1155312.
Matched: dtor, draw, init, __sinit (4/5).
Resistant: update (99.28 %, missing/extra): original has an extra unreachable `b` to the end after the `field_0x230 < 0` branch (offset 0x30); explicit default before/after cases did not produce it.
SHARED-EDITS: none (gen_game.tsv rows for this unit removed).
