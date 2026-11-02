# dolphin/gx/GXFifo.c

Baseline (progress.py first line at start): total: 1172 / 11560 functions, 212664 / 4434096 bytes of code (4.80 %)
After: total: 1178 / 11560 functions, 213016 / 4434096 bytes of code (4.80 %)

Status: Matching (21/21 functions, .text 100 %, .sbss 100 %).

- fn_8005A048 / fn_8005A068 -> GXSaveCPUFifo / __GXSaveCPUFifoAux. FSA's 2003 GX has the older
  shape (as in tww): GXFlush is called inside __GXSaveCPUFifoAux after OSDisableInterrupts, not in
  GXSaveCPUFifo (dolsdk2004 moved it). Bodies from dolsdk2004 SDK_REVISION < 2 path, assert kept.
- .text extended to 0x8005A464 over GXSetCurrentGXThread (fn_8005A400), GXGetCurrentGXThread
  (fn_8005A44C), GXGetCPUFifo, GXGetGPFifo (previously in an auto split, which globalized
  __GXCurrentThread/CPUFifo/GPFifo with address suffixes). Added from dolsdk2004 with assert.
- Uses int for the interrupt state (the oot-gc file has no BOOL typedef in scope).

SHARED-EDITS: none.
