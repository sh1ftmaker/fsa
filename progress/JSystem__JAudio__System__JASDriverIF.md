# JSystem/JAudio/System/JASDriverIF.cpp

Baseline (w10-jas start): progress.py total 4784 / 11561 functions, 863464 bytes; unit 6/13.
Now: unit 13/13, .text/.ctors/.bss/.sdata 100 %, Matching; `link_check --unit` SHA-1 OK; gate: ok.
Total 4791 / 11561, 863956 bytes.

## Done
- The 7 misses were all the unsplit callback managers: `.bss` 0x804B2550-0x804B2850 (three 0x100
  `JASCallbackMgr`s, source order sDspSyncCallback, sSubFrameCallback, sUpdateDacCallback) added to the unit.
- Names: lbl_804B2650 -> sSubFrameCallback__9JASDriver, lbl_804B2750 -> sUpdateDacCallback__9JASDriver
  (evidence: subframeCallback/registerSubFrameCallback and updateDacCallback load them; the source's
  declaration order; our object's symbol offsets 0x100/0x200). 0x804B2550 keeps the section anchor
  `...bss.0` (dtk drops the anchor when a named symbol shares its address, which the gate reads as a rename).
- waitSubFrame is in the source but dead-stripped in FSA (not in the target).

## Resistant
None.

## SHARED-EDITS
None.
