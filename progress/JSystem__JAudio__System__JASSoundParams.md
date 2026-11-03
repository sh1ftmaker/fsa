# JSystem/JAudio/System/JASSoundParams.cpp

Baseline (w10-jas start): progress.py total 4784 / 11561, 863464 bytes; unit 0/1 (combine 99.78 %).
Now: 1/1, .text/.sdata2 100 %, Matching; `link_check --unit` SHA-1 OK; gate: ok. Total 4785, 863556 bytes.

## Done
- .sdata2 0x8053B178-0x8053B180 split into the unit: combine's 0.5f. lbl_8053B178 (0x8, dtk had
  swallowed the padding) -> @66, 0x4, our object's local name.
- clamp()'s three constants (@55-@57) are dead-stripped data: the trial map lists them and clamp UNUSED
  for JASSoundParams.o, so the honest report leaves them out (weakdedup manifest `dead_data`). No source change.

## Resistant
None.

## SHARED-EDITS
None.
