# JSystem/JAudio/System/JASWSParser.cpp

Baseline (w10-jas start): progress.py total 4784 / 11561, 863464 bytes; unit 9/10 (createBasicWaveBank 99.96 %).
Now: 10/10, .text/.sbss 100 %, Matching; `link_check --unit` SHA-1 OK; gate: ok. Total 4789, 864232 bytes.

## Done
- createBasicWaveBank called fn_8002EA3C = getWaveGroup__16JASBasicWaveBankFUl. The rename (and the
  other three JASBasicWaveBank names) is the same symbols.txt change as on the JASBasicWaveBank branch,
  and the wave-bank headers are the same as there (JASWaveInfo.h, JASWaveBank.h, JASBasicWaveBank.h,
  JASSimpleWaveBank.h, TP's layout), so the branches merge in any order. The header split doesn't
  change this object's layout (its weak copies all come from JSUtility templates).

## Resistant
None.

## SHARED-EDITS
None beyond the JASBasicWaveBank branch's (identical lines).
