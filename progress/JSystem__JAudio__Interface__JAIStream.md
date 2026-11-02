# JSystem/JAudio/Interface/JAIStream.cpp

Baseline (progress.py): 3711 / 11561 functions, 710408 bytes (15.99 %).
After: 3727 / 11561, 713804 bytes (16.07 %). gate: ok.

Unit: 6/16 -> 14/15 functions (JAIStreamDataMgr's dtor left the range). .data 100 %,
.sdata2 85.71 % (4 bytes alignment pad in the target), .bss/.ctors 0 % (no __sinit).

## Matched (names and splits only)
- Vtable slots of `__vt__9JAIStream` (matched code order agrees): fn_80026D88 =
  getNumChild (returns 6), fn_80026E78/E80/E8C = getTrack / getChildTrack / getTempoMgr,
  slots 5/6 = JAISound::asSe / asSeq (fn_800257BC/C4), asStream = fn_800257CC.
- `lbl_804B220C` = `JASPoolAllocObject<JAISoundChild>::memPool_` (tryDie_, getChild,
  releaseChild), `fn_800254FC` = `JAISoundParams::mixOutAll` (mixOut_ call),
  .sdata2 0x8053B018-0x8053B028.

## Other names
- `__sinit_JAIStream_cpp` (0x80026E94, .ctors), pool `lbl_804B21FC` =
  `JASPoolAllocObject<JAIStream>::memPool_` (JAIStreamMgr allocates JAIStream from it),
  guard `lbl_8053A2B1`, dtor `fn_80021B68` = `JASMemPool<JAIStream>::~JASMemPool`;
  JAISoundChild guard/dtor as in JAISeq. .bss 0x804B2400-0x804B2440 (dtor chain).

## Resistant
- `__sinit_JAIStream_cpp`: FIXME, waits for the memPool_ definition (w3-jas).
  JASFakeMatch8.h include removed (empty without MATCHING).

## SHARED-EDITS
- `__dt__16JAIStreamDataMgrFv` (0x80026FD8-0x80027020) and `__vt__16JAIStreamDataMgr`
  (0x80491870) are JAIStreamDataMgr.cpp's, a separate TU in mkdd too. JAIStream's .text
  now ends at 0x80026FD8; a JAIStreamDataMgr.cpp unit should be added by the coordinator.
- symbols.txt renames shared with the JAISeq branch (identical lines): JAISound asSe/asSeq/
  asStream, mixOutAll, JAISoundChild pool/guard/dtor.
