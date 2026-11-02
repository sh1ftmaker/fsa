# dolphin/ar/arq.c

Baseline (progress.py first line at start): total: 1172 / 11560 functions, 212664 / 4434096 bytes of code (4.80 %)
After: total: 1173 / 11560 functions, 212776 / 4434096 bytes of code (4.80 %)

Status: Matching. 5/5 functions, .text/.data/.sdata/.sbss 100 %. gate: ok.

## What was wrong (no code changes)
- ARQInit's `bl fn_8004FDD8` relocation: `fn_8004FDD8` is `ARRegisterDMACallback` (ar.c).
  Bytes: saves r3, loads old `__AR_Callback`, OSDisableInterrupts, stores new callback,
  OSRestoreInterrupts, returns the old one -- identical to the donors' ARRegisterDMACallback,
  and it sits immediately before ARGetDMAStatus (ar.c's first function).
  Renamed in symbols.txt (scope:global).

## SHARED-EDITS
- symbols.txt rename `fn_8004FDD8` -> `ARRegisterDMACallback`: the function belongs to ar.c,
  whose .text stanza still starts at 0x8004FE1C (ar.c's agent extends it; done on branch unit/dolphin__ar__ar).
