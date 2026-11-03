# game/act_ending_map_skipper
Baseline: total: 5630 / 11557 functions, 1143176 / 4442032 bytes (25.74 %). After: 5632 functions, 1143204 bytes.
Matched: dtor, draw, init, __sinit (4/5 per try.py).
Resistant: update (71%, missing/extra): original loads row[0] into r31 before fn_802D6A9C() and recomputes the table address for row[1] after; ours computes the row pointer once. Tries: plain 2D index, struct array, row pointer.
SHARED-EDITS: include/game/GameGlobals.h: GameRoot_8049EF20::fn_80136798(s32, s32) (called as lbl_8053AAF8->fn_80136798(15, 0), act_ending_map_skipper update); symbols.txt placeholder renamed to fn_80136798__17GameRoot_8049EF20Fll.
