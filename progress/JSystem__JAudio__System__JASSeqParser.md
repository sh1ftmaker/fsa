# JSystem/JAudio/System/JASSeqParser.cpp

Baseline (start): progress.py total 1199 / 11560 functions, 218212 bytes; unit 36/60 functions.
Now (committed, no shared edits): unit 57/61 functions (sinit added to .text), total 1224 / 11560, 221080 bytes.
With the two SHARED-EDITS below applied locally: unit 61/61, every section 100 % (code, data, bss, sbss, sdata2, rodata, ctors); total 1230 / 11560, 224028 bytes. JASTrack also gains (41 -> 43 functions); no unit regresses.

## Done
- Placeholder names from evidence (call sites in matched code, sCmdInfo ptmf table, vtable):
  cmdCloseTrack/cmdCall/cmdLoopS/cmdCheckPortImport/cmdCheckPortExport/cmdSetInterrupt/
  cmdDisInterrupt/cmdFinish/cmdNop/cmdBankPrg/cmdBank/cmdPrg; JASTrack openChild, setSeqData,
  connectBus, noteOn, gateOn, readPortSelf, writePortSelf, readPort; __ptmf_test, __ptmf_scall,
  __ptmf_null (zeros, 0xC); JMath::TRandom_fast_ ctor; JASCalc::CUTOFF_TO_IIR_TABLE;
  __dt__12JASSeqParserFv (vtable slot), __vt__12JASSeqParser (0x20).
- memPool_ pools at 0x804B21AC (TChannelMgr: freed via free__50...TChannelMgr) and 0x804B21BC
  (JASTrack: alloc 0x250 = sizeof(JASTrack), free__35...JASTrack); their sbss guard bytes and
  JASMemPool_MultiThreaded<T> dtors (pairing from __register_global_object call sites);
  __ct__17JASGenericMemPoolFv (zeroes 4 words, = JASHeapCtrl.cpp ctor).
- Splits: .text extended over __sinit_JASSeqParser_cpp (0x800355BC..0x80035F4C), .ctors
  0x8045BD78, .bss 0x804B2918..0x804B2940 (3 destructor-chain nodes, 0xC each).
- rodata/sdata2 placeholders split into the cmdDump strings ("" is in .sdata2).
- Removed the JASFakeMatch.h include (no-op here: MATCHING is not defined).

## Update (coordinator-approved SHARED-EDIT #1 applied)
- JASTrack.h TP layout committed separately (42256fc): cmdBankPrg/cmdBank/cmdPrg match, JASTrack 37 -> 43.
- .text now starts at writeReg 0x80033778 (writeReg/readReg were in an auto split; both match).
  0x8003363C (JASSeqCtrl's __sinit) and 0x80033730 (~JASSeqParser, weak, emitted by JASSeqCtrl) belong to JASSeqCtrl.
- Unit: 62/63 functions; only __sinit_JASSeqParser_cpp (90.85 %) left. Total 1199 -> 1231. ninja SHA-1 OK, gate ok.

## Resistant
- __sinit_JASSeqParser_cpp: FSA also constructs the three JASPoolAllocObject_MultiThreaded<T>::memPool_
  (TChannelMgr, JASTrack, JASChannel). A generic definition in JASHeapCtrl.h makes it byte-identical,
  BUT it is wrong globally: every TU that sees it instantiates the pools, and JASTrackPort.cpp and
  JAUStreamFileTable.cpp (Matching) gain a 0xD4 __sinit FSA does not have, so the DOL no longer links
  (link_check: layout differs). The definition must have been visible only in some TUs (in a header
  JASSeqParser.cpp includes but JASTrackPort.cpp does not, or in the .cpp). No evidence which, so left open.

## FakeMatch measurement (generic MultiThreaded def vs JASFakeMatch*.h active, memPool/sinit symbols)
- 23 source files include JASFakeMatch*.h (not 37). 7 are not built (JAIAudible, JAIAudience, JAISeMgr,
  JAISoundStarter, JAUClusterSound, JAUSoundAnimator, JAUSoundObject).
- Identical with the generic def: JAISound, JAISoundHandles, JASSeqCtrl, JASTrack (all JASFakeMatch.h).
- Still different: every unit on JASFakeMatch2..12.h (non-MT JASPoolAllocObject<JAI*> pools). A generic
  non-MT def also reproduces JAISeq, JAISoundChild, JAIStream, JAUInitializer, but not JAISe, JAISeqMgr,
  JAIStreamMgr, JAUAudioArcLoader, JAUAudioMgr, JAUSectionHeap, JAUSoundMgr, JAUSoundTable.
- Because of the link break above, a generic def in JASHeapCtrl.h cannot replace any of them as is.
