# dolphin/ar/ar.c

Baseline (progress.py first line at start): total: 1172 / 11560 functions, 212664 / 4434096 bytes of code (4.80 %)
After: total: 1176 / 11560 functions, 212868 / 4434096 bytes of code (4.80 %)
(includes arq.c's ARQInit, which matches once ARRegisterDMACallback is named.)

Status: Matching. 10/10 functions (was 7/9), .text/.data/.sdata/.sbss 100 %. gate: ok.

## Changes
- `fn_8004FDD8` -> `ARRegisterDMACallback`: swaps `__AR_Callback` under OSDisable/RestoreInterrupts
  and returns the old one (donor code); it was in auto split auto_03_8004FDD8_text, so ar.c .text
  start extended 0x8004FE1C -> 0x8004FDD8 (ai.c ends at 0x8004FDD8).
- `fn_80050074` -> `ARGetSize`: `lwz r3, __AR_Size; blr`, between ARInit and __ARHandler (donor order).
- `fn_80050114` -> `__ARGetInterruptStatus`: target is `lhz 0xCC00500A; rlwinm 26,26` = `& 0x20`.
  mkdd/MP4 source had `& 0x0200`; changed to dolsdk2004/ttyd's `return __DSPRegs[5] & 0x20;`.
- ARGetBaseAddress, ARFree, ARCheckInit and the __ARRead/WriteDMA helpers are dead-stripped in FSA.

## SHARED-EDITS
- symbols.txt rename `fn_8004FDD8` -> `ARRegisterDMACallback` is the same line as on branch
  unit/dolphin__ar__arq (arq.c's ARQInit calls it); identical edit, merges cleanly.
