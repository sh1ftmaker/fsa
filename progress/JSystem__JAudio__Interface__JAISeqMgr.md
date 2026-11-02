# JSystem/JAudio/Interface/JAISeqMgr.cpp

Baseline (progress.py): 3711 / 11561 functions, 710408 bytes (15.99 %).
After: 3719 / 11561, 712464 bytes (16.04 %). gate: ok.

Unit: 4/11 -> 10/11 functions. .data 100 %, .sdata2 85.71 % (4 bytes alignment pad
in the target), .bss/.ctors 0 % (no __sinit).

## Matched (names and splits only)
- `sInstance` of `JASGlobalInstance<JAISeqMgr>` (lbl_8053A298, ctor/dtor) and
  `<JAISoundInfo>` (lbl_8053A294, startSound); `__vt__14JAISeqDataUser` (lbl_804917A8) and
  `__dt__14JAISeqDataUserFv` (fn_80024B50) from the base-class vtable store / dtor call;
  `JASPoolAllocObject<JAISeq>::memPool_` (lbl_804B221C: new/delete JAISeq in calc,
  beginStartSeq_, endStartSeq_). .sdata2 0x8053AFD0-0x8053AFE0.

## Other names
- `__sinit_JAISeqMgr_cpp` (0x80025368, .ctors), guard `lbl_8053A2B3`, dtor `fn_80021AC0` =
  `JASMemPool<JAISeq>::~JASMemPool`. .bss 0x804B2328-0x804B2358 (dtor chain).

## Resistant
- `__sinit_JAISeqMgr_cpp`: FIXME, waits for the memPool_ definition (w3-jas).
  JASFakeMatch9.h include removed (empty without MATCHING).

## SHARED-EDITS
- symbols.txt lines shared with the JAISeq/JAISeMgr branches (identical): JAISeqDataUser
  vtable/dtor, JAISoundInfo sInstance, JAISeq pool/guard/dtor.
