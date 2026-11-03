# game/act_chain_switch

Baseline (progress.py): total: 5734 / 11557 functions, 1152552 / 4442032 bytes of code (25.95 %)
After: 5737 / 11557 functions, 1153012 bytes (25.96 %)

Matched: all 5 (dtor, draw, update, init, __sinit). No resistant functions.
Not flipped to Matching: extab/extabindex are 47% (same weak-duplicate extab issue as act_switch_or).
gen_game.tsv: removed both act_chain_switch rows (whole rows).
SHARED-EDITS: none.
Layout: 0x230 timer, 0x234 bool, 0x238 delay (field_0x90 & 0x1FFFF), 0x23C-0x23E u8 bit fields, 0x240/0x242 u16.
