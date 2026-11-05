# game/tu_803DAC3C.cpp (game mode screen, GameMode_804B0688)

2026-11-05 (Opus, boot-path pass). Baseline (matching 5a5f0e81): 8302 functions;
after: 8321 (+19, no drops by address). Structural count 8657.

## Matched (19 / 23)
fn_803DAC3C, fn_803DAC58 (colour switch, 8 KB), fn_803DCCA4, the six draw
states (fn_803DD19C, DD1A0, DD1A4, DD3DC, DD3E0, DD760), fn_803DD8E8,
fn_803DD93C, fn_803DE05C (state 3), fn_803DEC44 (state 1), fn_803DF624
(draw), fn_803DF810 (update), fn_803DF9E8 (reset), destructor, constructor,
__sinit (PTMF state tables). .rodata, .bss, .sdata2 100 %.

## Resistant (written, FIXME in source)
- fn_803DF2B4 (state 0) 99.32 % regalloc-only: alpha-loop counter / tag
  pointer registers swapped.
- fn_803DE58C (state 2) 96.35 % missing/extra: first `*other++` store goes to
  the stack slot directly; table base via the section symbol; loop registers.
- fn_803DD970 (state 4) 97.47 % "different" (same length): same prologue
  issue as state 2, plus loop / f30-f31 registers.
- fn_803DD764 (state 7) 93.94 % missing/extra: the save-region copy loop
  (same unsolved loop as main and cls_804A8578::fn_802B1DE8).

## Data
.data: the vtable is followed by 0xC zero bytes (lbl_804B0694), most likely
the dropped weak copy of __vt__12cls_804AF86C (ours emits it before our
vtable, so .data is 97 %).

## SHARED-EDITS
- include/game/THPMovie.h (new): THPMovie_804B15D0 moved out of
  src/game/tu_80427D2C.cpp; IsMoviePlaying_804286A0 made global (symbols.txt
  has it global, tu_803DAC3C calls it); mGetRestFrame / GetMovieRestFrame
  return u32 (caller tests with cmplwi; returns -1 / 0 / counts).
- include/game/GameGlobals.h: class cls_804AF860 (ctor fn_803A7898 renamed
  __ct__12cls_804AF860Fv, 0x84 bytes from __nw__); cls_804A8578::fn_802B1558
  (renamed fn_802B1558__12cls_804A8578Fv, r3 = lbl_8053ACA8);
  Unk_8045A080::fn_80457F80(JAISoundID, s8) (renamed ...F10JAISoundIDSc);
  forward decl of JAISoundID.
- libs/sdk/include: OSSetBootDol (OSReset.h), __PADDisableRecalibration (pad.h)
  SDK prototypes.

## Next
DD970 / DE58C prologue: the original stores via the pointer even for the
first entry; maybe the "others" loop has another shape.
