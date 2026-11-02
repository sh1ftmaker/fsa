# dolphin/os/OSInterrupt.c

Baseline (progress.py first line at start):
total: 1172 / 11560 functions, 212664 / 4434096 bytes of code (4.80 %)
After: total: 1174 / 11560 functions, 212860 / 4434096 bytes of code (4.80 %)

## Status: Matching (.text, .data, .sbss 100 %)

9/10 -> 11/11: __OSInterruptInit and ExternalInterruptHandler now match.

- .text end moved 0x80042E54 -> 0x80042EA4 (OSLink.c's start): the unit's own
  static asm ExternalInterruptHandler (0x50 bytes, already a real symbol, source
  already present) sat in an auto split, so dtk globalized it as
  ExternalInterruptHandler_80042E54 and __OSInterruptInit's lis/addi reloc
  pointed there.
- No source change; configure.py flag -> Matching.

## SHARED-EDITS
none (own splits.txt stanza only).
