# JSystem/J2DGraph/J2DWindowEx.cpp

Baseline (progress.py first line at start): total: 4501 / 11561 functions, 808092 / 4442088 bytes of code (18.19 %)
After: total: 4502 / 11561 functions, 808704 / 4442088 bytes of code (18.21 %)

Status: **Matching**. 48/48 functions, code 10984/10984, data 480/480 (was 47/48, 448/480).
`link_check.py --unit JSystem/J2DGraph/J2DWindowEx.cpp`: links byte-identical (SHA-1 OK);
gate: ok after the flip.

## Changes
- drawFrameTexture(f32 x4, u16 x4, J2DMaterial*, bool): the parent-alpha branch in tp's and
  pikmin2's form, `u8 matColorAlpha = ...->a; matColorAlpha = matColorAlpha *
  parentPane->mColorAlpha / 0xff; GXSetChanMatColor(GX_ALPHA0, JUTColor(matColorAlpha));`
  (two int locals had the multiply operands in swapped registers).
- symbols.txt: .rodata placeholder lbl_80463088 (0xE0) split into the five const tables our
  object emits at the same offsets/sizes: @401 (0x10), @454 (0x80), @1093 (0x18), @1094
  (0x18), @1095 (0x1E; 2 padding bytes follow); lbl_80463168 -> @1617; .sdata2
  lbl_8053BA88/90/98/9C/A0 -> @445, @448, @478, @833, @834 (the last resized 0x8 -> 0x4,
  4 bytes padding). All scope:local. setStage's table relocation now matches by name.
- configure.py: J2DWindowEx.cpp -> Matching.

## SHARED-EDITS
- none.
