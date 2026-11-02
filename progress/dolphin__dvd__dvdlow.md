# dolphin/dvd/dvdlow.c

Baseline: total: 1139 / 11560 functions, 204312 / 4434096 bytes of code (4.61 %)
After:    total: 1143 / 11560 functions, 205516 / 4434096 bytes of code (4.63 %)

Status: Matching. 21/21 functions, .text/.bss/.sdata/.sbss 100 %. No source changes.

What made it match (data/symbols only):
- `.text` split start moved 0x80047578 -> 0x80047258: `__DVDInitWA` and
  `__DVDInterruptHandler` sat in auto_03_80047258_text, which globalized
  `Read` as `Read_8004766C`.
- `fn_80047578` -> `AlarmHandler` (local): bytes are AlarmHandler with
  ProcessNextCommand inlined (cmd 1 -> Read, cmd 2 -> DVDLowSeek); the
  function is referenced by WaitBeforeRead's OSSetAlarm in DVDLowRead.
- New bss symbols in the unnamed gaps: `AlarmForWA` 0x804B5850 (0x28),
  `Prev` 0x804B58C8 (0xC), `Curr` 0x804B58D4 (0xC), per the donor static
  order (CommandList, AlarmForWA, AlarmForTimeout, AlarmForBreak, Prev, Curr).
  objdiff's pooled-base relocations (CommandList+0xb8.. / +0x40) mismatched
  without them.
- `lbl_8053A4AC` -> `ResetCoverCallback` (sbss order between Callback and
  LastResetEnd, as in pikmin2/tww).

Dead-stripped helpers kept in the source as in the donors: ProcessNextCommand,
SetTimeoutAlarm, HitCache, DoJustRead, WaitBeforeRead (inlined in the target).

SHARED-EDITS: none.
