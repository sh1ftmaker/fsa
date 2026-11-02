# dolphin/mtx/mtx44.c

Baseline (progress.py first line at start):
total: 1172 / 11560 functions, 212664 / 4434096 bytes of code (4.80 %)
After: total: 1173 / 11560 functions, 212816 / 4434096 bytes of code (4.80 %)

## Status: NonMatching (.text 100 %, .sdata2 80 %)

Matched: C_MTXOrtho (0/1 -> 1/1).

- Split the float pool 0x8053B308-0x8053B318 (1.0f, 2.0f, 0.0f, -1.0f) into
  this unit. That was the only miss: C_MTXOrtho's four lfs relocations pointed
  at the auto split's lbl_8053B308.. instead of the unit's own @5..@8.
- No code change.

## Why still NonMatching
Our object's .sdata2 is 1.0, 2.0, 0.0, -1.0, 0.5, pi/180 (0x18 bytes); the
target's is the first 0x10. The trailing 0.5f and 0.01745329f are used only by
C_MTXPerspective, which FSA's linker dead-stripped together with C_MTXFrustum
(the target .text is exactly C_MTXOrtho). Per the COORDINATOR precedent
(GXPerf.c, GXPixel.c) dead functions stay in the source like the donors, so the
unit stays NonMatching until the linked DOL's SHA-1 can confirm it.

## SHARED-EDITS
none (own splits.txt stanza only).
