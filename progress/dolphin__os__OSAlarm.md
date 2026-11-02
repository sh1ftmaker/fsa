# dolphin/os/OSAlarm.c

Baseline (progress.py first line, before work): total: 1098 / 11560 functions, 192840 / 4434096 bytes of code (4.35 %)
After: total: 1109 / 11560 functions, 196152 / 4434096 bytes of code (4.42 %)

## Matched (8/8, unit Matching)
OSInitAlarm, OSCreateAlarm, InsertAlarm, OSSetAlarm, OSCancelAlarm,
DecrementerExceptionCallback, DecrementerExceptionHandler (original SDK asm), OnReset.
OSSetPeriodicAlarm is dead-stripped in FSA; it stays in the source like the donor.

## Fixes (no code changes)
- splits.txt: OSAlarm .text start 0x8004061C -> 0x800402FC (OSInitAlarm..OSSetAlarm
  sat in auto_03_800402D0; DecrementerExceptionHandler_80040968 was a globalized static).
- symbols.txt: fn_800409B8 -> OnReset (only reference is ResetFunctionInfo.func;
  calls __DVDTestAlarm/OSCancelAlarm exactly as donor OnReset).
- symbols.txt: fn_800675AC -> __div2i (between __div2u and __mod2u, size 0x138,
  same layout/size as donor runtime symbols; also renamed on other pending branches).

## SHARED-EDITS
- symbols.txt rename __div2i (shared runtime symbol; identical line on other branches).
- 0x800402D0..0x800402FC (OSRegisterVersion) is now the only content of the auto split;
  belongs to OS.c's owner if OS.c should extend over it.
