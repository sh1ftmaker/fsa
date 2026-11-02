# dolphin/os/OSReboot.c

Baseline (progress.py first line at start): total: 1172 / 11560 functions, 212664 / 4434096 bytes of code (4.80 %)
After: total: 1175 / 11560 functions, 213520 / 4434096 bytes of code (4.82 %)

Status: Matching. 4/4 functions (was 1/2), .text/.bss/.sbss 100 %. gate: ok.

## Changes
- `fn_8004395C` -> `Callback` (local): `Prepared = TRUE`; __OSReboot passes it to __DVDPrepareResetAsync.
- __OSReboot (0x80043968, 0x340) and `fn_80043CA8` were left in auto split auto_03_80043968_text.
  .text end extended 0x80043968 -> 0x80043CB4. The existing __OSReboot source matches as-is.
- `fn_80043CA8` -> `OSSetSaveRegion`: `SaveStart = r3; SaveEnd = r4;` (tww/pikmin2/MP-port donors,
  same position after __OSReboot). Added to the source.
- `lbl_8053A448`/`lbl_8053A44C` -> `SaveStart`/`SaveEnd` (local): __OSReboot stores them to
  0x812FDFF0/0x812FDFEC, OSSetSaveRegion writes them.
- `lbl_804B4DA0` -> `Header` (local, 0x20): __OSReboot's apploader-header DVD read buffer
  (DVDReadAbsAsyncPrio(..., 0x20, 0x2440), then reads +0x14 size / +0x18 rebootSize).
  Added `.bss 0x804B4DA0..0x804B4DC0` (between OSError.c and OSRtc.c bss).

## Notes for OSReset.c's owner
- OSRegisterResetFunction (0x80043CB4..0x80043D38) is still in auto split auto_03_80043968_text,
  before OSReset.c's .text start 0x80043D38.

## SHARED-EDITS
- none outside this unit's stanza and placeholder renames.
