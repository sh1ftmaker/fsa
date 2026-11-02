# dolphin/dvd/dvd.c

Status: **Matching**. 47/47 functions, .text/.data/.bss/.sdata/.sbss all 100 %.

Fixes (no code changes beyond the version string):
- splits.txt: .text now starts at 0x800489B0 (defaultOptionalCommandChecker, DVDInit);
  added .data 0x804961B0-0x80496330 and .sbss 0x8053A508-0x8053A550.
- dvd.c: __DVDVersion string -> "Sep 16 2003 09:50:54" (FSA's DVD build).
- symbols.txt renames (placeholders, with evidence):
  fn_800489B0 -> defaultOptionalCommandChecker (stored in checkOptionalCommand),
  fn_8004AAE4 -> DVDSetAutoInvalidation (body swaps autoInvalidation),
  lbl_8053A54C -> LastState, fn_8004B56C -> __DVDPrintFatalMessage and
  lbl_8053A550 -> FatalFunc (dvdFatal.c, pikmin2), fn_8006AEE4 -> memcmp (MSL mem.c).

Notes for neighbours: lbl_8053A558 is fstload.c `status`; dvdqueue.c .text really
starts at 0x8004B0E4 (__DVDClearWaitingQueue etc.), its split starts at 0x8004B27C.
SHARED-EDITS: none.
