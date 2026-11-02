# dolphin/dvd/dvdfs.c

Baseline (progress.py first line): total: 1149 / 11560 functions, 207532 / 4434096 bytes of code (4.68 %)
After: total: 1157 / 11560 functions, 209660 / 4434096 bytes of code (4.73 %)

## Matched (11/11 linked functions, .text 100 %)
__DVDFSInit, DVDConvertPathToEntrynum, DVDFastOpen, DVDOpen, DVDClose,
entryToPath, DVDGetCurrentDir, DVDReadAsyncPrio, cbForReadAsync, DVDReadPrio,
cbForReadSync.

## What was done
- splits: .text extended to 0x800480D4..0x800489B0 (__DVDFSInit and
  DVDConvertPathToEntrynum were in auto split 800480D4; DVDReadPrio and
  cbForReadSync in auto split 80048874). .data 0x80496048..0x804961B0 added
  (the four dvdfs strings).
- symbols: fn_80048784/44/74/8C named DVDReadAsyncPrio/cbForReadAsync/
  DVDReadPrio/cbForReadSync (OSPanic strings "DVDReadAsync()/DVDRead(): ...",
  callbacks passed to DVDReadAbsAsyncPrio, __DVDThreadQueue wakeup).
  fn_8006A36C named tolower (-1 check + __lower_map lookup, called by isSame).
- source: read asserts use OSPanic(__FILE__, 750/756/820/826) with
  `offset <= fileInfo->length` (dolsdk2004/pikmin2); 8.3 panic line 387.

## Remaining
- .data 52 %: our object also carries strings of dead-stripped functions
  (DVDSeek, DVDOpenDir, DVDPrepareStream*). Per COORDINATOR.md they stay;
  unit stays NonMatching until DOL SHA-1 confirms.

## SHARED-EDITS
- symbols.txt: fn_8006A36C -> tolower (MSL ctype, not my unit).
