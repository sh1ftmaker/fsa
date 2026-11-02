# dolphin/card/CARDOpen.c

Baseline: total: 1139 / 11560 functions, 204312 / 4434096 bytes of code (4.61 %)
After:    total: 1141 / 11560 functions, 204864 / 4434096 bytes of code (4.62 %)

## Status: Matching (6/6, .text 100 %, no data sections)

Matched: __CARDCompareFileName, __CARDAccess, __CARDIsWritable, __CARDIsReadable, CARDOpen, CARDClose.

## Fix
The two misses differed only in the `lbz r0, lbl_80539CCA@sda21` relocation.
0x80539CCA is in CARDNet.c's .sdata, directly after __CARDVendorID (0xFFFF), and holds 0x1C.
Every donor CARDNet.c (mkdd, pikmin2, bfbb, dolsdk2004) declares
`u16 __CARDVendorID = 0xFFFF; u8 __CARDPermMask = 28;` in that order.
Renamed placeholder lbl_80539CCA (size 0x6) -> __CARDPermMask (size 0x1; the rest is sdata padding).
Side effect: CARDNet.c .sdata now 100 % as well.

## SHARED-EDITS
- config/G4SE01/symbols.txt: lbl_80539CCA -> __CARDPermMask (symbol belongs to CARDNet.c's .sdata).
