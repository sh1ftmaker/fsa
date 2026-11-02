# dolphin/os/OSRtc.c

Baseline: progress total 1098 / 11560 functions, 192840 bytes (4.35 %).
After: 1101 / 11560, 193964 bytes (4.37 %).

## Status: Matching (17/17 functions, .text and .bss 100 %)

- UnlockSram showed as `UnlockSram_80044800` (missing) because OSGetGbsMode /
  OSSetGbsMode (0x80044EFC..0x80045024) sat in an auto split and called the
  static from outside the unit, so dtk globalised it with a suffix. Fix:
  extended the `.text` split end from 0x80044EFC to 0x80045024 (OSSync.c
  starts at 0x80045024). No code change needed for the match.
- Cleaned speculative comments in UnlockSram/OSSetGbsMode; `Scb.sram[20]`
  written as `Scb.sram + sizeof(OSSram)` as in dolsdk2004 (same bytes).

## SHARED-EDITS
- none (splits.txt stanza is this unit's own).
