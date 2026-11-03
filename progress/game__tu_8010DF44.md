# game/tu_8010DF44
Baseline: total 6229 / 11557 functions, 1252968 bytes. After: 6237 / 11557, 1253332.

Matched (9/9): vfunc_0x54/0x58/0x5C, draw, update, init, __dt__, __ct__ of cls_8049D140 (vtable 0x8049D140, 3 slots beyond ActorBase's 19), __sinit (generated include).
Remaining sections below 100%: extab/extabindex only (weak JAS pool dtor duplicates, same as every sinit unit).
SHARED-EDITS: include/game/GameGlobals.h (fn_8011888C, fn_80118798 declared next to fn_801186B0, owner tu_80118384; types from fn_80118798's lwz 0x198 on r4 and the (obj, actor) calls); gen_game.tsv: removed tu_8010DF44 sinit row.
symbols.txt: renamed placeholders to cls_8049D140 mangled names; lbl_8053C518 size 0x8 -> 0x4 (the other 4 bytes are section-end padding; ours is 0xc).
Provisional: vfunc_0x54/0x58/0x5C return types (void, s32 -1, s32 0) are unverified; no caller checked.
