# dolphin/os/OSMutex.c

Baseline (progress.py first line at start): total: 1172 / 11560 functions, 212664 / 4434096 bytes of code (4.80 %)
After: total: 1174 / 11560 functions, 212728 / 4434096 bytes of code (4.80 %)

Status: Matching. 8/8 functions (was 6/7), .text 100 %. gate: ok.

## What was wrong (no code changes)
- `fn_80043838` -> `OSInitCond`: 0x20 wrapper that just calls OSInitThreadQueue(r3), sits
  between OSTryLockMutex and OSWaitCond exactly where the donors' OSMutex.c has OSInitCond.
- `fn_8004392C` -> `OSSignalCond`: 0x20 wrapper calling OSWakeupThread(r3), directly after
  OSWaitCond (donor order). It was left in auto split auto_03_8004392C_text; OSMutex.c .text
  end extended 0x8004392C -> 0x8004394C (OSReboot.c starts at 0x8004394C).
- __OSCheckMutex / __OSCheckDeadLock / __OSCheckMutexes / IsMember are dead-stripped in FSA.

## SHARED-EDITS
- none beyond this unit's stanza and the two placeholder renames (callers of OSSignalCond are
  in unsplit auto code around 0x803B9B3C).
