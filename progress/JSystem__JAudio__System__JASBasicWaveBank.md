# JSystem/JAudio/System/JASBasicWaveBank.cpp

Baseline (w10-jas start): progress.py total 4784 / 11561 functions, 863464 bytes; unit 18/22, .data 91.8 %.
Now: 22/22, .text/.data 100 %, Matching; `link_check --unit` SHA-1 OK; gate: ok.
Total 4789 / 11561, 864232 bytes (JASWSParser's createBasicWaveBank also reaches 100 %: it calls getWaveGroup).

## Done
- Names (evidence: our object's symbols at the same offsets, and the vtables in the DOL):
  fn_8002EA3C -> getWaveGroup__16JASBasicWaveBankFUl (JASWSParser calls it),
  fn_8002F114 -> __dt__11JASWaveBankFv (slot 2 of __vt__11JASWaveBank),
  fn_8002F15C -> __dt__13JASWaveHandleFv (slot 2 of __vt__13JASWaveHandle),
  fn_8002F248 -> getWaveArc__16JASBasicWaveBankFUl (slot 4 of __vt__16JASBasicWaveBank).
- Link order (two header layout fixes):
  1. `TWaveInfo` has no user-declared destructor. FSA has ~TWaveInfo right after ~TWaveGroup, its
     first user; MWCC 2.6 with -sym on emits a compiler-generated function at its first use but
     user-declared inlines at the end of the file (checked in scratch compiles: an explicit
     `~TWaveInfo() {}` goes to the trailing section).
  2. JASWaveHandle and JASWaveBank moved into JASWaveInfo.h, JASBasicWaveBank and JASSimpleWaveBank into
     their own headers (TP's layout: JAudio2/JASWaveInfo.h, JASBasicWaveBank.h). With -sym on each header's
     inline copies get their own trailing .text section, so FSA's tail order (JASWaveBank dt, JASWaveHandle dt
     | TWaveInfo ct, TWaveHandle dt, getWaveInfo, getWaveArc, getArcCount) comes from the two headers.
     JASWaveBank.h now just includes the three headers, so other includers are unchanged. Full honest
     report diff: no change outside this unit and JASWSParser (up).

## Resistant
None.

## SHARED-EDITS
- Header split above (libs/sdk/include/JSystem/JAudio/System/JASWaveInfo.h, JASWaveBank.h, new
  JASBasicWaveBank.h, JASSimpleWaveBank.h). Header layout fix, done here; report diff clean.
