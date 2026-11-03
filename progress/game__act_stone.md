# game/act_stone
Baseline: total: 6322 / 11557 functions, 1265640 / 4442032 bytes of code (28.49 %); unit 2/8.
Now: 6328 functions, 1267484 bytes; unit 8/8 (all functions 100 %, .text/.data/.bss/.rodata/.ctors/.sdata2 100 %).
Not Matching: extab/extabindex 60.87 % (weak duplicates, same as act_sign). Coordinator flips Matching.
Resistant: none.
Notes: vfunc_0x24 needs `s32 layer = field_0x4;` read once (kept in r31 across the KMRI call). update copies act_rock's tile-probe shape; the layer-flag branch (viewLayerFlag) resets y -= z, z = 0 once (field_0x235).
SHARED-EDITS: none (include/game/act/act_stone.h is this unit's own; gen_game.tsv act_stone rows deleted).
