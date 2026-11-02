# dolphin/gx/GXPixel.c

Baseline: total: 1139 / 11560 functions, 204312 / 4434096 bytes of code (4.61 %)
Now:      total: 1140 / 11560 functions, 204860 / 4434096 bytes of code (4.62 %)

## Matched
All 12 linked functions (12/12, .text 1608/1608, .data 32/32).
GXSetFog fixed by splitting .sdata2 0x8053B3B0-0x8053B3E8 into this unit
(its 8 float/double constants: 0, 1, 0.5, 1.0d, 2, 0.5d, 8388638, int->float magic).

## Why still NonMatching
.sdata2 is 73.68 %: our object has 0x20 extra bytes after the GXSetFog pool
(@130..@135: 0.0d, sqrt(3)~1.732f, 3.0d, 256.0f, 0x4330000000000000) used only by
GXInitFogAdjTable, which the linker dead-stripped (target text is exactly the
other 12 functions). Per COORDINATOR (GXPerf precedent) the dead function stays
in the source; NonMatching until DOL SHA-1 confirms.
The target data after 0x8053B3E8 belongs to auto_03_8005F78C (next unit), not here.

## SHARED-EDITS
none. (lbl_8053B3A8 is GXBump.c's constant; its owner should extend GXBump's
.sdata2 split to 0x8053B3A8-0x8053B3B0.)
