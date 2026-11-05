# game/tu_8016A2B4.cpp (checkpoint)

Baseline progress: total 7510 / 11557 functions. After: 7515 / 11557 (32.62 %).

The unit: one class (GameRootSub24_8016A3AC, lbl_8053AAF8->field_0x4->field_0x24) that places and draws J3DModels (2D-camera backdrop layers); fn_8016A7DC places one J3DModel (setBaseScale, mPosMtx, calc/entry/viewCalc).

Matched (code identical, 7 count in the honest report): fn_8016A2B4, A330, A3AC, FDE0, ED88, EDB0 (A330 inlined), B0B0 (99.7 % reloc-name only).
Code-identical but reloc-name only (sdata2 literal order needs the unwritten functions): fn_8016A9EC..AD84, B054, B8AC, D284, D2E0, D33C (99.1-99.2 %).
Resistant: fn_8016A7DC 98.26 % regalloc-only (r3/r4 and f3/f5 swapped); fn_8016DA6C 75.2 % (MWCC inlines fn_8016AD84, original calls it).
Not done: A3BC (J3D loader), ADE0 (texture quad), B248, B4D4, B910, C2FC, CB14, D398, DB60 (same inlining problem: 0 %), DCEC, DE30, EE84, F0BC, __sinit.
SHARED-EDITS: include/game/GameGlobals.h (GameRootSub24_8016A3AC fields and members, owner this unit); symbols.txt renames of own functions.
