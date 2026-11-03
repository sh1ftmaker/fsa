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

## Wave 8 (2026-11-02)

Baseline: total: 4501 / 11561 functions, 808092 / 4442088 bytes of code (18.19 %)
Result: unchanged, 74/75; no Matching flip. Source: added tp's `GXEnd()` after the quad
(an empty inline, no code change); FIXME on draw extended.

draw__12J2DPictureExFffffbbb (97.57 %), variants tried this wave:
- `GXEnd()` after the last vertex (tp): no change.
- `volatile PPCWGPipe GXWGFifo : GXFIFO_ADDR;` in libs/sdk/include/dolphin/gx/GXVert.h
  (the real SDK form: dolsdk2004, tp, sms, melee, pikmin2, lm, bfbb all declare it volatile;
  only mkdd comments `/*volatile*/` out): draw becomes 100 %. Full-build report diff
  against the baseline, though: J2DScreen::drawSelf 100 -> 88 %, J2DWindow::drawFrameTexture
  (ffffUsUsUsUsb) 100 -> 92 %, drawFrameTexture(bbb) 90.6 -> 83 %, JPABaseShape JPADrawPoint
  100 -> 91.5 %, JPADrawStripe/StripeX 100 -> 97.6/98.6 %, JPADrawLine 97.8 -> 93.9 %,
  JUTResFont::drawChar_scale 100 -> 97.8 % (net -5 functions). Not applied. So FSA's
  J2DPictureEx saw a volatile FIFO where those units did not; how (a different header or
  an access through a volatile pointer in this file) is still open.
- Blocked by nothing outside the unit; a codegen/header difference.

SHARED-EDITS: none proposed (the GXVert.h change regresses other units).
