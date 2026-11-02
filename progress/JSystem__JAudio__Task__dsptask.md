# JSystem/JAudio/Task/dsptask.c

Baseline (progress.py): total: 1307 / 11560 functions, 241984 / 4434096 bytes of code (5.46 %)
After: unchanged (same line); gate: ok.

## Status: Matching (6/6 functions, code 680/680, data 15880/15880)

All functions already matched. The only data miss was in `jdsp` (DSP microcode, 0x1D20 bytes):
dtk had turned the microcode word 0x80005600 at jdsp+0x1884 (0x80494F24) into a false
R_PPC_ADDR32 relocation to `@etb_80005600`, so the target object had zeros there.
Fix: `jdsp` in symbols.txt gets `align:32 noreloc`, exactly as in the donors
(mkdd, sms, pikmin2, tp, tww and others all carry `jdsp ... align:32 noreloc`).
Address, size, type and name unchanged. `jdsp` is declared `ALIGN(32)` in source.

## SHARED-EDITS
None (symbols.txt attribute on this unit's own symbol only).

## Notes
extra_cflags still lists both -O4,p and -O4,s; code matches as is, left untouched.
