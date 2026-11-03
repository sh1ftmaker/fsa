# JSystem/J2DGraph/J2DPictureEx.cpp

Baseline (progress.py first line at start): total: 4319 / 11561 functions, 772344 / 4442088 bytes of code (17.39 %)
After: total: 4320 / 11561 functions, 773344 / 4442088 bytes of code (17.41 %)

Status: NonMatching. 74/75 functions (was 73/75), code 15824/17272, data 464/464 (was 440/464). gate: ok.

## Changes
- drawOut(TBox2, TBox2): pikmin2's form: `alphaCalc = mMaterial->getMaterialAlphaCalc()` read
  before copying the corner colors (target loads mMaterialAlphaCalc at +0xB4, before the copy),
  `TCornerColor` + `getCornerColor`, unrolled alpha scaling `(mColorAlpha * c.a) / 255`. Matches.
- draw(): same `alphaCalc` hoist (pikmin2's form); fixes the first register block.
- symbols.txt: placeholders split/renamed to the compiler locals our object emits at the same
  offsets and sizes: lbl_80463188 (0x68) -> @1468/@1469/@1470 (setStage's three const tables,
  0x20/0x20/0x28); lbl_8053BAC8/BACC/BAD0 -> @445/@753/@1048; lbl_8053BAD8 (0x8) -> @1637
  (0x4, 255.0f; the other 4 bytes are alignment padding before J2DTevs' .sdata2). Data now 100 %.

## Resistant
- draw__12J2DPictureExFffffbbb 97.57 %: register allocation only, in the vertex writes:
  target `lis r6,0xcc01; lwz r3,0x8(r1)` / ours `lis r3,0xcc01; lwz r4,0x8(r1)`, and the
  GXTexCoord2s16 conditionals use r5/r4/r3 vs ours r6/r5/r4. Tried: 0.0f literals instead of `z`
  (tp form), GXTexCoord2u16 (tp): no change.

## SHARED-EDITS
- none (symbols.txt lines are this unit's own data).
