# JSystem/JMessage/control.cpp

Baseline (start): `total: 3711 / 11561 functions, 710408 / 4442088 bytes of code (15.99 %)`
Now:              `total: 3748 / 11561 functions, 714156 / 4442088 bytes of code (16.08 %)`
(includes the processor.cpp commit cherry-picked below)

Unit: 2/7 -> 7/7 functions, .text and .data 100 %, `Matching`, gate ok (link SHA-1 ok).

## Depends on processor.cpp
This branch carries the processor.cpp commit (cherry-picked from
unit/JSystem__JMessage__processor): control.cpp needs its TProcessor.h (tp vtable layout,
on_begin, TProcessor::reset()) and its symbol names. Merge processor first; the
cherry-picked commit is then identical and drops out on rebase.

## What made it match
- Names (symbols.txt), by FSA's function order (= pikmin2/tp control.cpp order) and contents:
  fn_80078B64 -> `__dt__Q28JMessage8TControlFv` (stores `__vt__Q28JMessage8TControl`, conditional delete),
  fn_80078BAC -> `reset__Q28JMessage8TControlFv` (clears 0x14-0x24, calls TProcessor::reset on both processors),
  fn_80078C94 -> `render__Q28JMessage8TControlFv` (setBegin_messageEntryText on the rendering processor, copies the stack, calls TRenderingProcessor::process),
  fn_80078E30 -> `setMessageCode__Q28JMessage8TControlFUsUs` (getProcessor + setMessageCode_inSequence_ + setBegin).
- reset() calls `TProcessor::reset()` (FSA calls 0x800778DC) instead of the inline reset_(NULL).
- setMessageID is not in FSA (dead-stripped); kept in source like the donors.

## SHARED-EDITS
- None beyond the JMessage units.
