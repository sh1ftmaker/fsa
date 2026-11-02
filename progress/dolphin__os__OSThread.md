# dolphin/os/OSThread.c

Status: Matching. 24/24 functions, .text/.bss/.sdata/.sbss 100 %.

## Changes (data and symbols only, no source edits)
- splits: `.text` widened to 0x800450A8-0x800463E0 to take in
  DefaultSwitchThreadCallback, __OSThreadInit (before) and OSClearStack (after).
- symbols: `fn_800450A8` -> `DefaultSwitchThreadCallback` (the initial value of
  SwitchThreadCallback in .sdata 0x80539C20 is 0x800450A8; a 4-byte `blr`).
- symbols: `lbl_80539C10` -> `__OSFpscrEnableBits` (first .sdata word of
  OSError.c, value 0xF8 = FPSCR_ENABLE; OSCreateThread reads it as in all donors).
- symbols: split the .bss gap after RunQueue into IdleThread (0x804B4F18,
  0x318), DefaultThread (0x804B5230, 0x318), IdleContext (0x804B5548, 0x2C8).
  Sizes are sizeof(OSThread)/sizeof(OSContext) and fill the gap to 0x804B5810
  exactly; SelectThread passes RunQueue+0x730 to OSSetCurrentContext/
  OSClearContext (IdleContext), __OSThreadInit uses DefaultThread.
  IdleThread is unreferenced in FSA's code; named by layout/donor order.

## SHARED-EDITS
- symbols.txt `__OSFpscrEnableBits` belongs to OSError.c's data (shared symbol).
