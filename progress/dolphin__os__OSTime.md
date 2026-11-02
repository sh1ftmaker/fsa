# dolphin/os/OSTime.c

Baseline: total: 1147 / 11560 functions, 206980 / 4434096 bytes of code (4.67 %)
Final:    total: 1148 / 11560 functions, 207496 / 4434096 bytes of code (4.68 %)

Status: Matching (5/5 functions, .text and .data 100 %). No source change needed.

## Matched
- OSGetTime, OSGetTick, __OSGetSystemTime, GetDates (already matching)
- OSTicksToCalendarTime: only miss was the callee placeholder fn_800677C8.
  Renamed to __mod2i: its body (0x800677C8, 0x10C) is instruction-for-instruction
  identical to __mod2i in zeldaret/tp libs/PowerPC_EABI_Support/Runtime/Src/runtime.c
  (signed 64-bit remainder, sign taken from dividend via cr7), and it sits in
  runtime.c source order between __mod2u and __shl2i.

## SHARED-EDITS
- config/G4SE01/symbols.txt: fn_800677C8 -> __mod2i (runtime.c symbol, placeholder rename).

## Next
- fn_80067944 (runtime.c, after __shr2i) is probably __cvt_dbl_usll by donor order; not checked, not mine.
