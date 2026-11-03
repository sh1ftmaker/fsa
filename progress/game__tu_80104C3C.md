# game/tu_80104C3C.cpp
Baseline: 5901 / 11557 functions, 1181784 bytes (26.60 %). Now: 5905 / 11557, 1182976.
Matched: 6/6 (dtor, draw, update, fn_80104EB0 -> member, init, __sinit).
Resistant: none. Matching flag not set (coordinator).
SHARED-EDITS: config/G4SE01/gen_game.tsv (deleted this unit's 2 rows).
Names: fn_80104EB0 is now cls_8049CB58::fn_80104EB0 (symbols.txt renamed to the mangled form; no other callers).
Notes: lbl_80463EE0 is a function-local static shift table {22,17,12,7} in init(); update's `ready` must be s32 (cmpwi, not clrlwi).
