# JSystem/J2DGraph/J2DTevs.cpp

Baseline (progress.py first line at start): total: 4319 / 11561 functions, 772344 / 4442088 bytes of code (17.39 %)
After: total: 4321 / 11561 functions, 773128 / 4442088 bytes of code (17.40 %)

Status: Matching. 8/8 functions (was 6/8), .text/.rodata/.sdata2 100 %. gate: ok (link SHA-1 OK).

## Changes
- Dropped `#include "std/math.h"` from J2DTevs.cpp. The unit only uses sinf/cosf (math.h);
  std/math.h's inline sqrtf put its two weak local statics
  (`_half$localstatic3$sqrtf__3stdFf`, `_three$localstatic4$sqrtf__3stdFf`, 0x10 bytes) at the
  start of our .sdata2, which FSA's .sdata2 (0x8053BAE0) lacks. That shifted every float constant
  by 0x10, so getTextureMtx/getTextureMtxMaya loaded the wrong addresses. No header changed.

## SHARED-EDITS
- none.
