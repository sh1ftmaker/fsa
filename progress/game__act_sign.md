# game/act_sign
Baseline: total: 6007 / 11557 functions, 1208940 / 4442032 bytes of code (27.22 %); unit 2/8.
Now: 6013 functions; unit 8/8 (all functions 100 %, .text/.data/.bss/.rodata/.ctors 100 %).
Not Matching: extab/extabindex 60.87 % (weak duplicates), .sdata2 93.33 % (bytes identical; the 4.0/0.0 pair at 0x80540F50 is a symbol-size effect).
Resistant: none.
Notes: draw needs `s32 flags` declared before the SpriteParams local (flags gets r31).
SHARED-EDITS: none (include/game/act/act_sign.h is this unit's own; gen_game.tsv act_sign rows deleted).
