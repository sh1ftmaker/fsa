# dolphin/os/OS.c

Status: Matching. 14/14 functions, .data/.bss/.sdata/.sbss 100 %.

Matched: all (incl. __OSFPRInit, now in the unit).

Changes:
- OSInit: arena-lo fallback uses `_stack_addr` (as in dolsdk2004, mp4, ttyd);
  mkdd's `_db_stack_end` is the older SDK.
- splits.txt: OS.c .text starts at 0x8003F810 (__OSFPRInit, which references
  OS.c's ZeroF/ZeroPS statics) instead of 0x8003F938.
- symbols.txt: split placeholder lbl_8049563C (0x1B4) into the 17 string
  literals plus __OSExceptionLocations (0x8049571C, 0x3C); added DriveBlock
  (.bss 0x804B4D20, 0x30; &DriveBlock passed to DVDInquiryAsync in OSInit).

SHARED-EDITS: none needed. Note 0x8003F794..0x8003F810
(__DBExceptionDestination, __DBIsExceptionMarked, DBPrintf) is still auto-split;
it belongs to db.c (whose .text currently ends at 0x8003F794).
