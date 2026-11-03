# JSystem/JAudio/Utility/JAUBankTable.cpp

Baseline (progress.py): total 4784 / 11561 functions, 863464 / 4442088 bytes (19.44 %).
After: total 4784 / 11561 functions, 863464 bytes (19.44 %) (no loss: the four
functions that left the range were unmatched). gate: ok.

Unit: 2/6 -> 2/2 functions; .text 100 %. `link_check --unit` SHA-1 OK; flipped to Matching.

## What changed
- The range was too long. mkdd's map (MarioClub_us symbols.txt/splits.txt) has
  JAUBankTable.cpp = appendBankTable + weak getBank only, followed by a separate
  JAUClusterSound.cpp TU: weak `__ct__Q29JGeometry8TVec3<f>Fv`,
  `__dt__14JAISoundHandleFv`, `__ct__14JAISoundHandleFv`, then
  `__sinit_JAUClusterSound_cpp` (its real functions are dead-stripped). FSA has the
  identical sequence at 0x8003B2CC-0x8003B400 (sinit fn_8003B32C, 0xD4, three pools).
  .text trimmed to 0x8003B288-0x8003B2CC; .ctors dropped from this unit.
- Source back to mkdd's file: removed the JASTrack.h include an earlier agent added
  for the (JAUClusterSound) __sinit.
- JAUBankTable.h: `class JASBank;` instead of `#include JASBank.h` (tp's JAUBankTable.h
  and JASBank.h only forward-declare). JASBank.h pulls in JASChannel.h, and MWCC then
  instantiates JASPoolAllocObject_MultiThreaded<JASChannel>::memPool_ and a
  __sinit/.ctors entry in this TU, which broke the trial link. Full honest report
  diffed before/after the header change: 0 units changed.

## Resistant
None.

## SHARED-EDITS
- New unit JSystem/JAudio/Utility/JAUClusterSound.cpp (source exists in libs/jsystem):
  `.text 0x8003B2CC-0x8003B400`, `.ctors 0x8045BD8C-0x8045BD90`,
  `.bss 0x804B4B18-0x804B4B40` (lbl_804B4B18, its register chain);
  rename fn_8003B32C -> `__sinit_JAUClusterSound_cpp` (scope:local).
- libs/sdk/include/JSystem/JAudio/JAUBankTable.h (also included by JAUSectionHeap.h):
  forward declaration instead of JASBank.h; no report change elsewhere.
