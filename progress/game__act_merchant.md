# game/act_merchant (Merchant_8049CD48)
Baseline: total: 5920 / 11557 functions, 1185564 / 4442032 bytes of code (26.69 %). After: 5924 / 11557, 1186132 bytes.

Matched: dtor, draw, init, ctor, __sinit (5/6).
Resistant: update (92.13%, scheduling): in the fn_8038AD24 call the original loads mSerial, the mPos copy
address, field_0x4 and the player before the mPos psq_l copy and the msg clrlwi; ours does the copy first.
SHARED-EDITS: include/game/Player.h (fn_8022FF8C, fn_802300EC), config gen_game.tsv row removed.
Next: not flagged Matching (update not 100%).
