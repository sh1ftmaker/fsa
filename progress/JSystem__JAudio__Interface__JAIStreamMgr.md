# JSystem/JAudio/Interface/JAIStreamMgr.cpp

Baseline (progress.py): 3711 / 11561 functions, 710408 bytes (15.99 %).
After: 3718 / 11561, 712220 bytes (16.03 %). gate: ok.

Unit: 1/7 -> 6/7 functions. .sdata2 85.71 % (4 bytes alignment pad in the target),
.bss/.ctors 0 % (no __sinit).

## Matched
- `fn_80027418` = `JAIStreamMgr::stop()`: walks mStreamList calling JAISound::stop();
  added to the source before stop(u32) as in tp's and Petari's JAIStreamMgr.cpp
  (stop(u32) itself is dead-stripped in FSA; kept, like the donors).
- Names: `sInstance` of `JASGlobalInstance<JAIStreamMgr>` (lbl_8053A290, ctor) and
  `<JAISoundInfo>` (lbl_8053A294, startSound), `JASPoolAllocObject<JAIStream>::memPool_`
  (lbl_804B21FC, calc / newStream_). .sdata2 0x8053B028-0x8053B038.

## Other names
- `__sinit_JAIStreamMgr_cpp` (0x80027548, .ctors), guard `lbl_8053A2B1`, dtor
  `fn_80021B68` = `JASMemPool<JAIStream>::~JASMemPool`. .bss 0x804B2440-0x804B2470.

## Resistant
- `__sinit_JAIStreamMgr_cpp`: FIXME, waits for the memPool_ definition (w3-jas).
  JASFakeMatch10.h include removed (empty without MATCHING).
- Debt: `CW_FORCE_STRINGS(... "category >= 0")` under `#ifdef DEBUG` is still there
  (no effect on this build); the natural fix is the dead-stripped stopCategory(int).

## SHARED-EDITS
- symbols.txt lines shared with the JAIStream/JAISeMgr branches (identical): JAIStream
  pool/guard/dtor, JAISoundInfo sInstance.
