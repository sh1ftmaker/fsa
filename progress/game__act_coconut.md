# act_coconut
Baseline: total 6007 / 11557 functions, 1208940 bytes (27.22 %).
After: 6013 / 11557, 1210488 bytes (27.25 %).
Matched: all 8 (dtor, vfunc_0x48, vfunc_0x4C, vfunc_0x24, draw, update, init, __sinit), each 100 % on the first try.
Only extab/extabindex 60.87 % (weak JASMemPool dtor duplicates) and .sdata2 93.33 % (original has a trailing 4-byte pad after lbl_80541180's 4.0f) differ; no code resistant.
Shape follows act_rock (update, vfunc_0x4C, KMRI spawn); draw uses the SpriteParams reset inline.
SHARED-EDITS: none outside the unit (include/game/act/act_coconut.h fields 0x230 s32, 0x234 u8; gen_game.tsv: removed both act_coconut rows). Matching flag not set (extab/sdata2 < 100 %).
