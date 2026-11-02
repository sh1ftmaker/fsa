# dolphin/os/OSMemory.c

Baseline: total: 1172 / 11560 functions, 212664 / 4434096 bytes of code (4.80 %)
Final:    total: 1175 / 11560 functions, 213112 / 4434096 bytes of code (4.81 %)

Matched 7/7 code + .data (ResetFunctionInfo); unit set Matching. No source changes.

- fn_80043194 -> OnReset (local): its body is OnReset (if final: __MEMRegs[8] = 0xFF;
  __OSMaskInterrupts(0xF0000000); return TRUE), and it is ResetFunctionInfo's function pointer.
- .text end extended 0x80043418 -> 0x80043530 over __OSInitMemoryProtection, which was in the
  auto split auto_03_80043418_text (OSMutex.c starts at 0x80043530). That also un-globalized
  MEMIntrruptHandler_800431D0 and made the .data (ResetFunctionInfo) relocations resolve.
- OSGetPhysicalMemSize / OSGetConsoleSimulatedMemSize are dead-stripped in FSA (not in the DOL);
  kept in the source like the donors (they only leave code, no data).

SHARED-EDITS: none beyond this unit's splits.txt stanza and the one symbols.txt rename.
