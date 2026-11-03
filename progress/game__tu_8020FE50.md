# game/tu_8020FE50

Baseline: total: 6046 / 11557 functions, 1220300 / 4442032 bytes of code (27.47 %)
After: total: 6053 / 11557 functions, 1221768 / 4442032 bytes of code (27.50 %)

Matched (8/8, code and data 100 %): draw, vfunc_0xA0, vfunc_0x60, vfunc_0x90, init, ~dtor, ctor of
cls_804A6100 (vtable 0x804A6100, IBBM), and the generated __sinit.
Resistant: none. extab/extabindex show 59.26 % in try.py (weak-duplicate entries only).

SHARED-EDITS: see the report (symbols.txt renames of tu_80210630 / tu_8043A0E8 / ActorManager placeholders,
GameGlobals.h, ActorManager.h, gen_game.tsv row).
Next: tu_80210630 owns cls_804A61C8 (header include/game/act/tu_80210630.h; vtable symbol renamed).
