# JSystem/JKernel/JKRAram.cpp

Baseline progress: total: 4319 / 11561 functions, 772344 / 4442088 bytes of code (17.39 %)
After: see final report (13/13 functions here, +1 in JKRAramArchive)

Status: 13/13 functions, all sections 100 %, `Matching`, gate ok (link SHA-1 ok).
No source change was needed: all misses were names and one split boundary.

## What it took
- .text ends at 0x80079EFC: the function at 0x80079EFC (JKRAramArchive's default
  ctor: calls JKRArchive(), stores __vt__14JKRAramArchive) follows JKRAram's
  __sinit and weak JSUList dtor, and is the first function of TP's and our
  JKRAramArchive.cpp. JKRAramArchive.cpp .text now starts there (it matches 100 %).
- Names: nextSrcData (called by decompSZS_subroutine, same body), __sinit,
  JSUList<JKRAMCommand> dtor (registered by __sinit), decompMutex (OSInitMutex /
  OSLockMutex in JKRDecompressFromAramToMainRam; size 0x18 = OSMutex, the 4
  bytes after are padding), and the .sbss file statics szpBuf..tsArea at the
  offsets the matched code uses.

## SHARED-EDITS
- JKRAramArchive.cpp stanza: .text start 0x80079F38 -> 0x80079EFC.
