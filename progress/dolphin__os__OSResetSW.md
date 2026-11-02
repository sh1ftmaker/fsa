# dolphin/os/OSResetSW.c

Baseline: total: 1172 / 11560 functions, 212664 / 4434096 bytes of code (4.80 %)
Final:    total: 1175 / 11560 functions, 213068 / 4434096 bytes of code (4.81 %)

Matched 3/3 code + .sbss; unit set Matching. No source changes.

- .text extended to 0x800440E8..0x80044494 (was 0x800441DC..0x80044474): pulls in
  __OSResetSWInterruptHandler (already named, sat in auto_03_80043DF0_text after OSReset.c's
  functions) and the 0x20-byte wrapper after OSGetResetButtonState.
- fn_80044474 -> OSGetResetSwitchState (global): body is just `return OSGetResetButtonState();`
  under `#pragma dont_inline on`, as in the donor; JUTGamePad calls it.
- lbl_8053A468 -> ResetCallback (local): the handler loads it, clears it and calls it (r12).
- lbl_8053A470 (size 8) -> LastState (local, size 4): the handler stores TRUE to it together
  with Down; the other 4 bytes are alignment padding before the 8-byte HoldUp.

SHARED-EDITS: OSGetResetSwitchState rename also resolves the call in JSystem/JUTGamePad.
Note for OSReset.c's owner: fn_800440D8 (swaps bootThisDol, returns old) and OSResetSystem /
OSGetResetCode still sit in auto_03_80043DF0_text; OSReset.c's .text should extend to 0x800440E8.
