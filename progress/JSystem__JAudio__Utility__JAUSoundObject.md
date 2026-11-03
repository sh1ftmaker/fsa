# JSystem/JAudio/Utility/JAUSoundObject.cpp

Baseline: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After:    total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
Unit: 13/13 functions, code 2012/2012, data 4/100 -> 60/100 (100/100 with the proposed resize below).

## Status: NonMatching (waiting on one reviewed resize; link_check --unit SHA-1 OK, gate ok)

## Data gaps
1. .bss (0 %): placeholder `lbl_804B4CC8` (0x38) is the four 0xC destructor-chain
   objects `__sinit` registers (@740..@743 in our object, at +0x0/+0xC/+0x18/+0x24;
   `addi r5, r31, 0xc/0x18/0x24` in __sinit) plus 8 bytes of padding. Split
   into lbl_804B4CC8/CD4/CE0/CEC, size 0xC each (same style as JASAiCtrl's
   lbl_804B2524). .bss and __sinit now 100 %.
2. .data: `__vt__21JAUDopplerSoundObject` is dead-stripped data; the honest
   report already removes it with the trial-link map (`UNUSED 000024 ........
   __vt__21JAUDopplerSoundObject JAUSoundObject.o`, all its referencers UNUSED).
3. .data still 96.9 %: the real symbol `__vt__14JAUSoundObject` is sized
   0x28 in symbols.txt but is 2 header words + 7 slots (process, dispose,
   stopOK, startSound, startSoundIndex, startLevelSound, startLevelSoundIndex)
   = 0x24; the last word is alignment padding before the next unit's .data at
   0x804955E0. Trial map:
       00003f18 000048 804955b8  1 .data   JAUSoundObject.o
       UNUSED   000024 ........ __vt__21JAUDopplerSoundObject JAUSoundObject.o
       00003f18 000024 804955b8  4 __vt__14JAUSoundObject   JAUSoundObject.o
   Verified locally (not committed): with size 0x24 the unit is 100/100 data.

## SHARED-EDITS
- config/G4SE01/symbols.txt: placeholder lbl_804B4CC8 (0x38) split into four 0xC objects (committed).
- PROPOSED (needs review, approved_symbol_changes.txt): `__vt__14JAUSoundObject 0x28 0x24`
  (2 header words + 7 slots; dtk included 4 bytes of padding before 0x804955E0;
  same case as __vt__11J3DShapeMtx). Then flip JAUSoundObject.cpp to Matching.

## Resistant
None in code.
