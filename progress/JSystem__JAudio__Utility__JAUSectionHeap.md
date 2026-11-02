# JSystem/JAudio/Utility/JAUSectionHeap.cpp

Baseline (progress.py): 1307 / 11560 functions, 241984 bytes. Unit 8/33.
Now: 1359 / 11560, 252036 bytes. Unit 37/39 (target now 39 functions after
extending .text over the vtable thunks and __sinit).

## Matched
All JAUSection / JAUSectionHeap functions, bitset helpers, TStreamDataMgr
getStreamFileEntry/dtor, @212@ thunks. New in source: newVoiceBank,
JAUSectionHeap::setSeqDataArchive, JAUSectionHeap::getSoundTable (name and
position from Petari's JAUSectionHeap.cpp: between getNumSections and
setSeqDataUser; FSA game code calls it, sectionHeapData_.soundTable at 0x524).
Removed JASFakeMatch2.h include (was a no-op: MATCHING undefined) and the
DEBUG-only CW_FORCE_STRINGS block.

## Resistant
- newStreamFileTable: only `bl fn_8003C794` vs `__ct__18JAUStreamFileTableFv`.
  fn_8003C794 (li r0,0; stw r0,0(r3)) is JAUStreamFileTable's ctor, but naming
  it makes the Matching JAUStreamFileTable unit fail the link (dup), because
  its .text split starts at 0x8003C7A0 instead of 0x8003C794. Same for
  lbl_804954C8 = __vt__32JAUStreamDataMgr_StreamFileTable (its .data is not
  split). Fix belongs to the JAUStreamFileTable owner (SHARED-EDITS).
- __sinit (fn_8003ED08, .ctors 0x8045BDA0, .bss 0x804B4C98): registers
  memPool_ for JASPoolAllocObject_MultiThreaded<TChannelMgr/JASTrack/JASChannel>
  and JASPoolAllocObject<JAISe>. Needs the generic memPool_ template definition
  (open project-wide JASFakeMatch question), not reproducible naturally here.

## SHARED-EDITS
- libs/sdk/include/JSystem/JAudio/JAUSectionHeap.h: + getSoundTable() decl.
- new libs/sdk/include/JSystem/JAudio/System/JASVoiceBank.h. FSA's JASBank
  slot 2 is getInstParam(int,int,int,JASInstParam*) const (JASBasicBank and
  JASVoiceBank vtables); JASBank.h still has mkdd's getInst(u32). The header
  overrides getInst to stay consistent with JASBank.h; fix both together.
- symbols.txt names outside this unit (evidence = identical call sites +
  callee bodies): __nw__FUl/__nw__FUli/__nw__FUlP7JKRHeapi/__nwa__FUl/
  __nwa__FUli, fprintf, abort, __dt__11JKRDisposerFv, create__12JKRSolidHeap,
  loadToMainRAM__12JKRDvdRipper(const char*...), createBank__12JASBNKParser,
  appendBankTable__22JAUBankTableDictionary, getBank__12JAUBankTableCFUl,
  __dt__13JAISeqDataMgr, __dt__16JAIStreamDataMgr, vtables of JAISeqDataMgr,
  JAIStreamDataMgr, JASBank, JASVoiceBank, JASDram, sInstance of
  JAUSoundInfo / JASDefaultBankTable.
- JAUStreamFileTable.cpp split should start at 0x8003C794 (see above).
