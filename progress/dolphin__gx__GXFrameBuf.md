# dolphin/gx/GXFrameBuf.c

## Matched
All 15 functions (.text 100 %), .sdata2 100 %.
- The 3 misses (GXSetDispCopyYScale, GXGetYScaleFactor, GXGetNumXfbLines) were
  only a call to placeholder `fn_80067334`, which is byte-identical to the
  runtime's `__cvt_fp2unsigned` (tp/mp4 runtime.c asm; followed by
  `__save_fpr` at 0x80067390). Renamed in symbols.txt.

## Data
`.data` split at 0x80497158–0x80497288 (five GXRenderModeObj, decoded):
- 0x80497158 GXNtsc480IntDf: tv 0 (NTSC_INT), xfb DF, vfilter 8,8,10,12,10,8,8
- 0x80497194 GXNtsc480Int: tv 0 (NTSC_INT), xfb DF, vfilter 0,0,21,22,21,0,0
  (dolsdk2004's GXNtsc480Int; source had GXNtsc480Prog, now fixed)
- 0x804971D0 GXMpal480IntDf: tv 8 (MPAL_INT)
- 0x8049720C GXPal528IntDf: tv 4 (PAL_INT), 640x528, yorigin 23
- 0x80497248 GXEurgb60Hz480IntDf: tv 0x14 (EURGB60_INT)
- 0x80497284: 4 bytes alignment padding (next .data object 0x80497288 is a
  jump table of code at 0x8005D4E4, not this unit).

.data 99.34 %: the only difference is that symbols.txt gives
GXEurgb60Hz480IntDf size 0x40 (dtk's gap-filled guess, includes the padding);
the object is a 0x3C GXRenderModeObj. Workers may not resize real symbols.

## SHARED-EDITS
- symbols.txt: `GXEurgb60Hz480IntDf` size 0x40 -> 0x3C (coordinator). With that
  the unit should be 100 % and can be flagged Matching.
- libs/sdk_gx/include/dolphin/gx/GXFrameBuffer.h still declares GXNtsc480Prog
  (harmless extern; FSA has no such object).
