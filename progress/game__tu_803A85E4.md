# game/tu_803A85E4

Baseline (progress.py): 6007 / 11557 functions, 1208940 / 4442032 bytes (27.22 %).
After: 6013 / 11557, 1210464 bytes (27.25 %).

Matched: 7/7 functions (fn_803A85E4__Fv, the five cls_804AF878 members, __sinit), .text/.rodata/.data/.bss 100 %.
extab/extabindex below 100 %: only the weak-duplicate JASMemPool dtor entries every sinit unit has.
Resistant: none.

SHARED-EDITS: Map.h (TileMap_804AA858::fn_802F9714/fn_802F93F4/fn_802FF798), GameState.h (field_0x248),
GameGlobals.h (AudioMgr_8045A080::fn_80458408/fn_804580DC/fn_80458090), new headers cls_804AF878.h,
tu_803E1668.h (cls_804B0B00), tu_803F0A54.h (ConnectScreen_804B0F10); gen_game.tsv row removed; symbols.txt renames.
Next: lbl_8053AD98 (this unit's singleton pointer) still sits in tu_8036FB60's .sbss range; split it when the
sbss chain AD60-ADD8 is divided. Not set Matching (coordinator trial link).
