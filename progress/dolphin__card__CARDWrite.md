# dolphin/card/CARDWrite.c

Baseline: total: 1147 / 11560 functions, 206980 / 4434096 bytes of code (4.67 %)
Final:    total: 1150 / 11560 functions, 207596 / 4434096 bytes of code (4.68 %)

Matched 4/4 (WriteCallback, EraseCallback, CARDWriteAsync, CARDWrite); unit set Matching.

- fn_80057F04 -> WriteCallback (local), fn_80058074 -> EraseCallback (local): byte-identical to
  this file's statics once named.
- fn_80058238 -> CARDWrite (global): calls CARDWriteAsync with __CARDSyncCallback then __CARDSync(fileInfo->chan);
  was in an auto split; .text end extended 0x80058238 -> 0x80058280 (CARDStat starts there).
- No source changes needed. SHARED-EDITS: none.
