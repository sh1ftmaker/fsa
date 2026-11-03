# game/act_check_switch

Baseline: total 5708 / 11557 functions, 1150032 / 4442032 bytes (25.89 %).

Matched: dtor, draw, init, __sinit (4/5).
Resistant: update (99.55 %, regalloc-only). The original loads the shift table
entry into r0 and field_0x90 into r4 (`srw r0, r4, r0`); ours loads field_0x90
into r0 and the entry into r3 (`lwzx r3,r3,r5`, `srw r0,r0,r3`). Tried: split
into a local shift (worse, reloc-name), u32 table, u32 bit.
SHARED-EDITS: none. gen_game.tsv: removed this unit's two rows.
Names: mState/mMode (0x230/0x234) from update/init asm; rodata lbl_8046D460 is
the function-local static {22,17,12,7}.
