# JSystem/JKernel/JKRExpHeap.cpp

Baseline (progress.py): total 1199 / 11560 functions, 218212 / 4434096 bytes.
After: total 1241 / 11560 functions, 226148 / 4434096 bytes. gate: ok.

Status: Matching. 37/37 functions, .text/.rodata/.data/.sbss/.sdata2 100 %.
Source: mkdd import; the CW_FORCE_STRINGS fake function was replaced by a real
JKRExpHeap::DBshow() (reviewer F1; dead-stripped, its strings stay in .rodata).

## What changed
- splits.txt: .text start moved 0x800823C8 -> 0x80081454 (createRoot..appendUsedList
  were in an auto split); added .data 0x80498DD8-0x80498E38 (vtable) and
  .sbss 0x8053A888-0x8053A8A0 (DB* statics).
- configure.py: -O4,s -> -O4,p for this unit (target saves regs with individual
  stw/lwz, not stmw/lmw; -O4,s produced stmw everywhere). Then Matching.
- symbols.txt names (evidence: mkdd/tww donor symbol order+sizes, vtable slots of
  __vt__10JKRExpHeap matching JKRHeap's virtual order, identical call sites):
  JKRExpHeap's 23 functions, __vt__10JKRExpHeap, DBfoundSize/DBfoundOffset/
  DBfoundBlock/DBnewFreeBlock/DBnewUsedBlock (local; DBnewUsedBlock resized 8->4).
  Externals: JKRHeap::__dt__, callAllDisposer, getTotalFreeSize,
  getMaxAllocatableSize, dispose(void*,u32), dispose(), state_dump (last vtable slot
  in every heap vtable, blr), __dl__FPv, JUTReportConsole(_f),
  JUTWarningConsole(_f), JUTException::panic_f.
- rodata placeholder lbl_8045E338 (0x340) split into 25 string placeholders.

## SHARED-EDITS
- The -O4,s flag is set on every JKernel unit in configure.py; other JKernel
  units likely want -O4,p too (coordinator decision).
- External symbol renames above live in JKRHeap/JUTConsole/JUTException ranges.
- libs/sdk/include/JSystem/JKernel/JKRExpHeap.h: added `void DBshow();` declaration
  (coordinator-approved shared edit, no layout change).
