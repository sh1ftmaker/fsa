# JSystem/JKernel/JKRAramArchive.cpp

Baseline progress: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: total: 4787 / 11561 functions, 864072 / 4442088 bytes of code (19.45 %)

Status: 12/12 functions, all sections 100 %, `Matching`, gate ok (link SHA-1 ok,
`link_check --unit` byte-identical).

## What it took
- 0x8007A138 __dt__7JKRFileFv (weak; stores __vt__7JKRFile, calls
  __dt__11JKRDisposerFv) - placeholder name only.
- 0x8007A198 / 0x8007A298 are JKRAramArchive::mountFixed(long, EMountDirection)
  and unmountFixed(), both called from game code. Source from ac-decomp's
  JKRAramArchive.cpp (CC0; GAFU01 symbols have unmountFixed__14JKRAramArchiveFv
  at the same 0x100 size). fixedInit() is auto-inlined into mountFixed (its
  field stores appear in that order inside 0x8007A198) and dead-stripped, as
  in AC. FSA passes the heap explicitly: the call is
  check_mount_already__10JKRArchiveFlP7JKRHeap with sCurrentHeap, so the
  source calls check_mount_already(entryNum, JKRGetCurrentHeap()).
  unmountFixed does not free mExpandSizes (unlike the destructor), matching AC.
- Removed the `#ifdef DEBUG CW_FORCE_STRINGS(...)` line (no effect on the
  release build; the gate rejects it in a Matching unit).

## SHARED-EDITS
- libs/sdk/include/JSystem/JKernel/JKRArchive.h: declared
  JKRAramArchive::fixedInit/mountFixed/unmountFixed (declarations only; full
  rebuild, progress total rose only by this unit's 3 functions / 608 bytes).
