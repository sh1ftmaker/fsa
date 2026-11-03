# JSystem/JAudio/System/JASSeqCtrl.cpp

Baseline (w10-jas start): progress.py total 4784 / 11561, 863464 bytes; unit 11/12 (__sinit missing).
Now: 12/12, .text/.ctors/.bss/.sbss 100 %, Matching; `link_check --unit` SHA-1 OK; gate: ok.
Total 4785, 863708 bytes.

## Done
- fn_8003363C -> __sinit_JASSeqCtrl_cpp (local; .ctors 0x8045BD74): registers sDefaultParser's dtor
  and builds the TChannelMgr/JASTrack/JASChannel pools, as our object does.
- .bss 0x804B28E8-0x804B2918 split into the unit: the four destructor-chain nodes; lbl_804B28E8
  (0x30 placeholder) -> @173, @276, @277, @278 (0xC each, our object's local names).
- The source no longer includes JASFakeMatch.h (COORDINATOR's fakematch list named this unit); the
  pools come from the generic definition our build already has, and the gate's fakematch check passes.
- findIntr is dead-stripped in FSA.

## Resistant
None.

## SHARED-EDITS
None.
