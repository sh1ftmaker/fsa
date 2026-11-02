# JSystem/JAudio/Interface/JAISeMgr.cpp

Baseline (progress.py): 3711 / 11561 functions, 710408 bytes (15.99 %).
After: 3721 / 11561, 713320 bytes (16.06 %). gate: ok.

Unit: 14/22 -> 21/22 functions; .data, .rodata, .sdata2 100 %; .bss/.ctors 0 % (no __sinit).

## Matched
- Relocation-only misses fixed by names: `__vt__8JAISeMgr` (lbl_80491718),
  `__vt__16JAISeCategoryMgr` (lbl_8049172C), `__vt__14JAISeqDataUser` (lbl_804917A8) and
  `__dt__14JAISeqDataUserFv` (fn_80024B50) from the ctors/dtors' vtable stores and base dtor call;
  `sInstance` of `JASGlobalInstance<JAISeMgr>` (lbl_8053A29C, ctor/dtor) and
  `<JAISoundInfo>` (lbl_8053A294, startSound); `JASPoolAllocObject<JAISe>::memPool_`
  (lbl_804B222C, calc/startSound). .data split 0x80491718-0x80491740 (both vtables).
- `fn_800231F8` = `JAISeMgr::stop(u32)`: loop over 16 categories calling the inlined
  `JAISeCategoryMgr::stop(u32)`; Petari's JAISeMgr.cpp has it in the same place
  (between stop() and stopSoundID) and its symbols list `stop__8JAISeMgrFUl` there.

## Other names
- `__sinit_JAISeMgr_cpp` (0x80023A68, .ctors entry), guard `lbl_8053A2B4`, pool dtor
  `fn_80021A6C` = `JASMemPool<JAISe>::~JASMemPool`. .bss 0x804B22B8-0x804B22E8 (dtor chain).

## Resistant
- `__sinit_JAISeMgr_cpp`: FIXME in source, waits for the memPool_ definition (w3-jas).
  JASFakeMatch2.h include removed (empty without MATCHING).

## SHARED-EDITS
- libs/sdk/include/JSystem/JAudio/Interface/JAISeMgr.h: declares `void stop(u32 fadeCount);`
  (non-virtual member, no layout change; separate commit).
- symbols.txt: `__dt__14JAISeqDataUserFv` / `__vt__14JAISeqDataUser` (JAISeqDataMgr TU),
  same lines as the JAISeq branch.
