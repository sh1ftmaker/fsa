# dolphin/os/OSCache.c

Baseline (progress.py): total: 1132 / 11560 functions, 202096 / 4434096 bytes of code (4.56 %)
Final:                  total: 1135 / 11560 functions, 202324 / 4434096 bytes of code (4.56 %)

Status: Matching. 19/19 functions, .text 100 %, .data 100 %. No source changes.

## What was wrong
- Three placeholder symbols for functions already in the source:
  - fn_800411C8 -> LCStoreBlocks (ori r6,0x2 into DMA_L; LCLoadBlocks uses 0x12)
  - fn_800411EC -> LCStoreData (its only callee is LCStoreBlocks; 128-block / 0x1000 loop)
  - fn_80041298 -> LCQueueWait (mfspr HID2, extrwi 4,4, cmpw r3, bgt loop)
  LCLoadBlocks, LCLoadData, LCQueueLength are dead-stripped in FSA (not in the DOL).
- .data blob lbl_804958A9 (size 0x207, unaligned) spanned 13 OSReport/DBPrintf
  strings; split into lbl_<addr> scope:local data:string at the compiler's offsets.

## SHARED-EDITS
- config/G4SE01/symbols.txt: only the 3 renames + the blob split above.
