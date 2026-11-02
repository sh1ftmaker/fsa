# dolphin/gx/GXBump.c

Baseline (progress.py first line): total: 1139 / 11560 functions, 204312 / 4434096 bytes of code (4.61 %)
Final: total: 1149 / 11560 functions, 206128 / 4434096 bytes of code (4.65 %)

Status: Matching. 10/10 functions, .text 100 %, .sdata2 100 %.

- Added GXSetIndTexMtx (was fn_8005E510), GXSetIndTexOrder (fn_8005E7CC), GXSetTevIndWarp (fn_8005E924)
  from doldecomp/dolsdk2004 / zeldaret/tp GXBump.c, asserts kept (CHECK_GXBEGIN, ASSERTMSGLINE).
- fn_8005E988 (a lone blr) = __GXUpdateBPMask (empty in non-MQ builds, position matches donor order).
- Split .text extended 0x8005E924 -> 0x8005E9E0 (the unit's own tail was in auto_03_8005E924_text:
  GXSetTevIndWarp, __GXUpdateBPMask, __GXSetIndirectMask, __GXFlushTextureState).
- Added .sdata2 0x8053B3A8-0x8053B3B0 (1024.0f used by GXSetIndTexMtx; +4 bytes alignment padding before
  GXPixel's sdata2). Placeholder lbl_8053B3A8 resized 0x8 -> 0x4.

- Review fix: removed oot-gc NO_INLINE() from GXSetTevIndirect (now written as dolsdk2004/tp with CHECK_GXBEGIN);
  restored dolsdk2004 asserts in GXSetIndTexCoordScale, GXSetNumIndStages, GXSetTevDirect. Still 10/10.

SHARED-EDITS: none.
