# JSystem/J2DGraph/J2DPane.cpp

Baseline (progress.py first line at start): total: 4501 / 11561 functions, 808092 / 4442088 bytes of code (18.19 %)
After: total: 4504 / 11561 functions, 808544 / 4442088 bytes of code (18.20 %)

Status: NonMatching. 61/62 functions (was 58/62); .data 98.90 % (was 80.90 %), .sdata2 95.24 %.
gate: ok.

## Changes (symbols.txt only, plus a FIXME comment)
- fn_800954B4 -> appendChild__7J2DPaneFP7J2DPane: our appendChild compiles to the same 0xB8
  bytes at that spot (null check, getParentPane, JSUPtrList::append of mTree at +0xDC,
  add(mBounds.i.x, mBounds.i.y) + calcMtx through the vtable when the old parent was null).
  prependChild/insertChild/removeChild are dead-stripped in FSA.
- fn_80095BC4 -> drawSelf__7J2DPaneFffPA3_A4_f (weak): the `blr` sits in __vt__7J2DPane
  right after drawSelf__7J2DPaneFff, the slot of the 3-argument drawSelf; our weak copy
  lands right after draw too.
- lbl_8053B9D0 / lbl_8053B9D4 -> @2066 / @2067 (scope:local): the two floats
  updateTransform loads, which our object emits at the same .sdata2 offsets (0x24, 0x28)
  and sizes. updateTransform now matches.

## Resistant
- fn_8009611C (0x30, missing): `lbz r0,0xb6(r3); extsb; ==0x78 -> stfs f1,0xb8; ==0x79 ->
  stfs f1,0xbc; else stfs f1,0xc0; blr`: rotate(f32)'s angle store without calcMtx(),
  called from game code (tu_803A60E4, tu_80434370). No donor (mkdd, tp, pikmin2, tww, sms,
  lm, Petari) has such a J2DPane function, so no name; not written. Blocked on a name.
- .sdata2 95.24 %: our 255.0f (@1685, setInfluencedAlpha only) is absent in FSA, where
  setInfluencedAlpha is dead-stripped; the linker drops it with the function.
- .data 98.90 %: __vt__7J2DPane is 0x94 in our object, 0x98 in symbols.txt (dtk folded the
  4 alignment bytes before J2DPicture's .data into it). A real symbol size, so not changed
  here; coordinator: approved_symbol_changes.txt if wanted.
- Unit can't be Matching while fn_8009611C is unnamed.

## SHARED-EDITS
- none (symbols.txt lines are this unit's own symbols).
