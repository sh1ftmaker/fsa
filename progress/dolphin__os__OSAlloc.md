# dolphin/os/OSAlloc.c

Baseline (progress.py first line at start): total: 1172 / 11560 functions, 212664 / 4434096 bytes of code (4.80 %)
After: total: 1173 / 11560 functions, 212680 / 4434096 bytes of code (4.80 %)

Status: Matching. 5/5 functions, .text/.sdata/.sbss 100 %. gate: ok.

## What was wrong (no code changes)
- `fn_80040B80` -> `OSSetCurrentHeap`: `old = __OSCurrHeap; __OSCurrHeap = r3; return old;`,
  between OSFreeToHeap and OSInitAlloc (donor order). Renamed in symbols.txt.
- OSAllocFromHeap, DLLookup/DLOverlap/DLSize etc. are dead-stripped in FSA.

## SHARED-EDITS
- none.
