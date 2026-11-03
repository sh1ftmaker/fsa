# JSystem/JAudio/System/JASSimpleWaveBank.cpp

Baseline (w10-jas start): progress.py total 4784 / 11561, 863464 bytes; unit 12/14, no .data split.
Now: 14/14, .text/.data 100 %, Matching; `link_check --unit` SHA-1 OK; gate: ok. Total 4786, 863480 bytes.

## Done
- Names from the vtables: fn_80030ED8 -> getWaveInfo__Q217JASSimpleWaveBank11TWaveHandleCFv (slot 3 of
  __vt__Q217JASSimpleWaveBank11TWaveHandle), fn_80030EE0 -> getArcCount__17JASSimpleWaveBankCFv (slot 5
  of __vt__17JASSimpleWaveBank; `li r3, 1`, the header's inline).
- .data 0x80491A88-0x80491ACC (the two vtables; the 4 zero bytes after are alignment padding, left unsplit).
- Link order: the same header split as JASBasicWaveBank's branch (JASWaveHandle/JASWaveBank in
  JASWaveInfo.h, JASSimpleWaveBank.h of its own, TP's layout), and `TWaveHandle` without a user-declared
  destructor: FSA has its dtor right after ~JASSimpleWaveBank (first use, compiler-generated) and the
  user-declared inlines (TWaveHandle ct, getWaveInfo, getArcCount) at the end.

## Resistant
None.

## SHARED-EDITS
- Headers libs/sdk/include/JSystem/JAudio/System/JASWaveInfo.h, JASWaveBank.h, JASBasicWaveBank.h,
  JASSimpleWaveBank.h: identical on the JASBasicWaveBank and JASSimpleWaveBank branches (merge either first).
