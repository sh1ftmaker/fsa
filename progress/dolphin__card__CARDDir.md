# dolphin/card/CARDDir.c

Baseline: total: 1172 / 11560 functions, 212664 / 4434096 bytes of code (4.80 %)
Final:    total: 1174 / 11560 functions, 213072 / 4434096 bytes of code (4.81 %)

Matched 4/4 (__CARDGetDirBlock, WriteCallback, EraseCallback, __CARDUpdateDir); unit set Matching.

- fn_80054DB8 -> WriteCallback (local), fn_80054E88 -> EraseCallback (local): byte-identical to
  this file's statics (mkdd/dolsdk2004 CARDDir.c) once named; EraseCallback passes WriteCallback
  to __CARDWrite and __CARDUpdateDir passes EraseCallback to __CARDEraseSector (relocs match).
- No source changes needed. No data sections in this unit. SHARED-EDITS: none.
