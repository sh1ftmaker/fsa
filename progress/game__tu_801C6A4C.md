# game/tu_801C6A4C

Baseline (progress.py): total 5932 / 11557 functions, 1188620 bytes.
After: 5935 / 11557, 1189976 bytes. gate: ok.

Matched: fn_801C6A4C, fn_801C6BA8, fn_801C6C7C (ShadowLink2_804A43E8 members), __sinit.
Resistant: none. Only .sdata2 90.91% (the original section is 0x18 with a trailing 0.0f / padding word after 0.7f; ours is 0x14) and the usual extab note.

SHARED-EDITS: act_shadow_link_2.h (field types/offsets, 4 members), cls_804A46F4.h
(field_0x338 s32, field_0x33c s16, member fn_801D12CC), GameGlobals.h (fn_801CD930 params s32, extern lbl_8053EDF4),
gen_game.tsv row removed, symbols.txt renames (fn_801CAED4, fn_801D12CC, the three functions).
