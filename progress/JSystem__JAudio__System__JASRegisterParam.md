# JSystem/JAudio/System/JASRegisterParam.cpp

Baseline (w10-jas start): progress.py total 4784 / 11561, 863464 bytes; unit 2/4 (write/read 99.8 %).
Now: 4/4, .text/.data 100 %, Matching; `link_check --unit` SHA-1 OK; gate: ok. Total 4786, 864032 bytes.

## Done
- .data 0x80491B70-0x80491BC0 split into the unit: write's and read's switch jump tables
  (jumptable_80491B70/80491B98, 0x28 each; our @117/@136). No source change.

## Resistant
None.

## SHARED-EDITS
None.
