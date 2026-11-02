# JSystem/JAudio/Interface/JAISeq.cpp

Baseline (progress.py): total 3711 / 11561 functions, 710408 / 4442088 bytes (15.99 %).
After: total 3728 / 11561 functions, 714888 bytes (16.09 %). gate: ok.

Unit: 9/20 -> 17/18 functions (the two JAISeqDataMgr functions left the range, see below).
.data 100 %, .sdata2 85.71 % (target has 4 bytes of alignment pad), .bss / .ctors 0 % (no __sinit).

## Matched (all by data/symbols, no code change)
- 6 functions differed only in relocations: `lbl_804B220C` is
  `JASPoolAllocObject<JAISoundChild>::memPool_` (die_, tryDie_, getChild, releaseChild
  allocate/free JAISoundChild through it), the floats `lbl_8053AFC0..C8` are this
  unit's `.sdata2` (split 0x8053AFC0-0x8053AFD0), `fn_800254FC` is
  `JAISoundParams::mixOutAll` (call site in mixOut_; same slot in JAISound.o, between
  attachHandle and the JAISound ctor).
- `dtor_8002449C` = `JASCriticalSection::~JASCriticalSection()`: byte-identical to
  our weak copy, emitted right after JAISound_tryDie_ (its first user, JAS_CS_START).
- .data: vtable slots 5/7 are `JAISound::asSe` / `asStream` (`fn_800257BC/CC`);
  `fn_800257C4` between them is `asSeq` (donor order, same 8-byte body).

## Other names (evidence from __sinit_JAISeq_cpp, the .ctors entry)
- `__sinit_JAISeq_cpp` (0x80024A0C), pool 5 `lbl_804B221C` =
  `JASPoolAllocObject<JAISeq>::memPool_` (JAISeqMgr allocates JAISeq through it),
  guards `lbl_8053A2B2/3`, pool dtors `fn_80021B14` = `JASMemPool<JAISoundChild>::~`,
  `fn_80021AC0` = `JASMemPool<JAISeq>::~` (mkdd's __sinit_JAISeq_cpp, same order).
- `.bss` 0x804B22E8-0x804B2328 split: the __register_global_object chain, used only here.

## Resistant
- `__sinit_JAISeq_cpp`: FIXME in source; needs the generic memPool_ definition
  (w3-jas, JASHeapCtrl.h). JASFakeMatch6.h include removed (it was empty without MATCHING).

## SHARED-EDITS
- JAISeqDataMgr.cpp is its own TU in FSA as in mkdd: `.text 0x80024B50-0x80024BE0`
  (`__dt__14JAISeqDataUserFv` = fn_80024B50, `__dt__13JAISeqDataMgrFv`), `.data
  0x80491790-0x804917C0` (`__vt__13JAISeqDataMgr`, `__vt__14JAISeqDataUser` = lbl_804917A8).
  I trimmed JAISeq's .text to end at 0x80024B50; the coordinator should add a
  JAISeqDataMgr.cpp unit (source exists in libs/jsystem) with that stanza.
- symbols.txt renames outside this unit's range: JAISound's mixOutAll / asSe / asSeq /
  asStream, the two JASMemPool dtors at 0x80021AC0/0x80021B14.
