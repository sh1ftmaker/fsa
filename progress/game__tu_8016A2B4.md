# game/tu_8016A2B4.cpp (checkpoint)

Baseline progress: total 7510 / 11557 functions. After: 7515 / 11557 (32.62 %).

The unit: one class (GameRootSub24_8016A3AC, lbl_8053AAF8->field_0x4->field_0x24) that places and draws J3DModels (2D-camera backdrop layers); fn_8016A7DC places one J3DModel (setBaseScale, mPosMtx, calc/entry/viewCalc).

Matched (code identical, 7 count in the honest report): fn_8016A2B4, A330, A3AC, FDE0, ED88, EDB0 (A330 inlined), B0B0 (99.7 % reloc-name only).
Code-identical but reloc-name only (sdata2 literal order needs the unwritten functions): fn_8016A9EC..AD84, B054, B8AC, D284, D2E0, D33C (99.1-99.2 %).
Resistant: fn_8016A7DC 98.26 % regalloc-only (r3/r4 and f3/f5 swapped); fn_8016DA6C 75.2 % (MWCC inlines fn_8016AD84, original calls it).
Not done: A3BC (J3D loader), ADE0 (texture quad), B248, B4D4, B910, C2FC, CB14, D398, DB60 (same inlining problem: 0 %), DCEC, DE30, EE84, F0BC, __sinit.
SHARED-EDITS: include/game/GameGlobals.h (GameRootSub24_8016A3AC fields and members, owner this unit); symbols.txt renames of own functions.

## 2026-11-05 Opus 5.5 literal-order pass (no code change)
Unit 7/39, total 8180 (unchanged). The 16 reloc-name near-misses (fn_8016A9EC..AD84, B054, B0B0, B8AC, D284-D33C)
cannot be fixed by writing earlier functions in address order. The original .sdata2 starts with ten 4-byte GXColor
objects (0x8053D910-34, used by CB14, C2FC, B910 in that order, i.e. reverse address order) and then 0.125, 1.0,
0.5, 0.0, all before fn_8016A7DC's 0x4330 double / 1.5 / 0.001953125. Experiments with the project compiler
(-O4,p -inline auto): literals are created in function order at code generation, EXCEPT aggregate initializers
(GXColor c = {...}, a struct of floats {0.5f, 7.0f}) inside an inline function, which are created when the inline
is parsed. So in the original those colours (and probably 0.125 / 1 / 0.5 / 0) come from inline helpers defined
before fn_8016A7DC (or in a header) that B910 / C2FC / CB14 / ADE0 inline. Reconstructing them means writing those
large paired-single draw functions with the same inline helpers; nothing smaller flips a near-miss.
Also: in fn_8016A7DC the original creates the 0x4330 double before 1.5f; in ours it comes last (MWCC creates the
conversion constant at the end of the function), so `(f32)(dir << 10)` may be ordered differently in the source.
