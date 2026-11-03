# JSystem/JAudio/System/JASCmdStack.cpp

Baseline (w10-jas start): progress.py total 4784 / 11561, 863464 bytes; unit 4/5 (__sinit 99.26 %).
Now: 5/5, .text/.ctors/.bss 100 %, Matching; `link_check --unit` SHA-1 OK; gate: ok. Total 4785, 863572 bytes.

## Done
- .bss 0x804B2498-0x804B24B0 split into the unit: the two destructor-chain nodes __sinit registers
  sCommandListOnce/sCommandListStay with. lbl_804B2498 -> @71, lbl_804B24A4 -> @72 (our object's local
  names, 0xC each, same form as the other units' @NNN nodes).

## Resistant
None.

## SHARED-EDITS
None. (0x804B2470-0x804B2498, lbl_804B2470 0x28, is still unsplit; it's before this unit and not its data.)
