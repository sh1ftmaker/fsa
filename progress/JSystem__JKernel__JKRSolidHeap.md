# JSystem/JKernel/JKRSolidHeap.cpp

Baseline progress: total: 4319 / 11561 functions, 772344 / 4442088 bytes of code (17.39 %)
After: total: 4334 / 11561 functions, 773496 / 4442088 bytes of code (17.41 %)

Status: 22/22 functions, all sections 100 %, `Matching`, gate ok (link SHA-1 ok).

## What it took
- do_free prints TP's "free: cannot free memory block (%08x)\n" warning (FSA's
  .rodata 0x8045E718 holds that string; the function calls JUTWarningConsole_f).
- .rodata extended to 0x8045E820: check()/dump() strings at 0x8045E7A0..0x8045E81A
  are referenced only from this unit (lbl_8045E7A0 blob split into its 4 strings).
- Placeholders named: 9 JKRSolidHeap functions; in JKRHeap: dispose__7JKRHeapFPvPv
  (0x800841B8, between dispose(void*,u32) and dispose(), called by do_freeTail),
  do_changeGroupID__7JKRHeapFUc / do_getCurrentGroupId__7JKRHeapFv /
  dump_sort__7JKRHeapFv (vtable slots 0x4C/0x50/0x18 of both JKRHeap and
  JKRSolidHeap, bodies return 0/0/true).
- recordState/restoreState are dead-stripped in FSA; mkdd's "fakematch" note
  removed (nothing in the DOL to match), comment says so.

## SHARED-EDITS
- JKRThread.cpp stanza: .rodata start 0x8045E7A0 -> 0x8045E820 (the 0x80 bytes
  before are JKRSolidHeap's strings; JKRThread never references them).
- symbols.txt names in JKRHeap's range (above).
