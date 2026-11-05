# game/tu_8016A2B4.cpp (checkpoint)

Baseline progress: total 7510 / 11557 functions. After: 7515 / 11557 (32.62 %).

The unit: one class (GameRootSub24_8016A3AC, gGameRoot_8053AAF8->field_0x4->field_0x24) that places and draws J3DModels (2D-camera backdrop layers); fn_8016A7DC places one J3DModel (setBaseScale, mPosMtx, calc/entry/viewCalc).

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

## 2026-11-05 Opus 5.5 pass 2 (pool order fixed)
Unit 7/39 -> 24/39; total 8368 / 11557 after rebase on matching (+17 honest; structural 8699).
- .sdata2 starts with ten file-scope `static const GXColor` (users B910 / C2FC / CB14, unwritten).
- The next five literals (0.125, 1, 0.5, 0, 0x4330...80000000 double) come from a dead-stripped
  helper defined before fn_8016A7DC that ADE0, B248, B4D4, B910, C2FC and CB14 all inline:
  `getMapTexView(center, half)` (map view in texture units: 0.125 / map cell counts, 1 / camera
  zoom, half = inv * 0.5 * sx * fbWidth). Its out-of-line copy sets the pool order; fn_8016ADE0
  matches it to 99.97 % with the helper inlined, which is the evidence for its body
  (camera fetched first, half.x/y/z then center.x/y/z as separate statements; s32 casts of the
  u16 render-mode sizes, as tu_8013AAD4 does).
- That flipped all 15 one-line placers + fn_8016B0B0. New: fn_8016DCEC 100 %, fn_8016A3BC 99.79 %
  (regalloc r29/r30), fn_8016ADE0 99.97 % (colour/arg-copy stack slots swapped), fn_8016B248
  73.96 % (scheduling+regalloc).
- fn_8016B8AC stays reloc-name: 102.4 follows fn_8016B4D4's 1/4096 and 3.
- fn_8016B4D4 not written: copies of center/half are stored from registers in the original
  (ours psq-copy), and center2.z / half2.z go through frsp; best 80.7 % missing/extra.
- fn_8016EE84 is the compiler destructor (mr. r3, extsh delete flag): textures 0xf4-0x120,
  emitters 0x124-0x13c, the four cls_8016FDE0 entries (emitter + ActorSoundObj delete),
  field_0x0->destroy(). Writing it as ~GameRootSub24_8016A3AC needs tu_80139BE8's
  `field_0x24->fn_8016EE84(1)` to become `delete field_0x24` (shared edit; not done).
- fn_8016D398 not written (0x6D4); the call in fn_8016DCEC uses matching's extern "C" declaration.
- .sbss2 0x80543F38-0x80543F40 split into the unit (ADE0's zero colour template).
SHARED-EDITS: include/game/GameGlobals.h (GameRootSub24_8016A3AC: field_0x54 / 0x6c animations,
0xf4 and 0x110-0x120 JUTTexture*, field_0x140 is now cls_8016ED88 (EDB0 calls fn_8016ED88 inline,
still 100 %); includes game/cls_8016ED88.h; fn_8016ADE0/B248/DCEC declarations);
config splits.txt (.sbss2); symbols.txt renames fn_8016B248, fn_8016DCEC.
