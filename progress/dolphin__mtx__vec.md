# dolphin/mtx/vec.c

Baseline (progress.py first line at start):
total: 1149 / 11560 functions, 207532 / 4434096 bytes of code (4.68 %)
After: total: 1153 / 11560 functions, 207964 / 4434096 bytes of code (4.69 %)

## Status: Matching (.text 100 %, .sdata2 100 %)

Matched: PSVECNormalize, PSVECMag, PSVECDotProduct, PSVECCrossProduct,
C_VECReflect, PSVECDistance.

- Float pool 0x8053B318-0x8053B328 (0.5f, 3.0f, 2.0f) split into this unit;
  lbl_8053B320 resized 8 -> 4 (the trailing 4 bytes are section alignment
  padding, now left in the auto split).
- fn_80047130 -> C_VECReflect: body identical to dolsdk2004's C_VECReflect
  (negate src, normalize both, dot, 2*n*cos - uI, normalize dst); 2.0f constant
  at 0x8053B320. Calls PSVEC* directly since libs/sdk/include/dolphin/mtx.h has
  no VECNormalize macros; donor asserts dropped because libs/sdk headers have no
  ASSERTMSGLINE (release build, they compile to nothing anyway).

## SHARED-EDITS
None.
