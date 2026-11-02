# dolphin/gx/GXTransform.c

Baseline (progress.py first line at start): total: 1172 / 11560 functions, 212664 / 4434096 bytes of code (4.80 %)
After: total: 1175 / 11560 functions, 213056 / 4434096 bytes of code (4.80 %)

Status: Matching (14/14 functions, .text 100 %, .sdata2 100 %).

- fn_80060054 -> GXSetZScaleOffset (dolsdk2004 body, CHECK_GXBEGIN(996) kept); bytes:
  zOffset(0x50C) = offset*16777215, zScale(0x510) = scale*16777215+1, XF regs 0x101C/0x101F.
- .sdata2 0x8053B400-0x8053B418 split into the unit (0.0f, 1.0f, 0.5f, 342.0f, 16777215.0f).
- Pool order 0.0, 1.0, 0.5 needs GXProject (first function in the dolsdk2004 file; dead-stripped
  in FSA) to be present, so it was added from dolsdk2004 with its assert.

SHARED-EDITS: none.
