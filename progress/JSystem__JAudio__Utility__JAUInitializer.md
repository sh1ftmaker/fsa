# JSystem/JAudio/Utility/JAUInitializer.cpp

Baseline (progress.py): 4501 / 11561 functions, 808092 bytes (18.19 %).
After: 4504 / 11561, 809036 bytes (18.21 %). gate: ok. Unit Matching.

Unit: 2/5 -> 5/5 functions; .text, .ctors, .bss, .sdata2 100 %.

## Matched
- Pool order (child, stream, seq, se after the 3 MT pools): includes reordered to
  JAISeMgr.h, JAISeq.h, JAIStream.h, JAISoundChild.h (plain pools come out in reverse
  instantiation order). No specialization.
- initJASystem: FSA ends with tp's OSGetSoundMode switch (mono/stereo ->
  JASDriver::setOutputMode). Names its references need, from bytes:
  - fn_8002A740 = JASAudioThread::create(long): new(JASDram) JKRThread-derived object,
    stores its vtable and lbl_8053A2A8, OSInitThreadQueue, sets the system heap,
    OSResumeThread (tp's create); called with mAudioThreadPrio.
  - lbl_8053A2A8 = JASGlobalInstance<JASAudioThread>::sInstance (set in create, read by
    JASAudioThread::getThreadPointer()).
  - lbl_8053A8D0 (0x8) split: sManager__15JKRThreadSwitch (JKRThreadSwitch::getManager()
    then enter(thread, id)) + lbl_8053A8D4.
- Splits: .bss 0x804B4B40-0x804B4B98 (7 x 0xC dtor-chain objects), .sdata2
  0x8053B2A8-0x8053B2B0 (lbl_8053B2A8 resized 0x8 -> 0x4: 1.0f + padding).
- JAUReportPoolCount (mkdd, empty in release) is dead-stripped in FSA; link_check and
  gate link byte-identical.

## SHARED-EDITS
- symbols.txt names in other ranges: create__14JASAudioThreadFl (in JASAudioReseter's
  .text split: JASAudioThread's text is mis-split there), sInstance of
  JASGlobalInstance<JASAudioThread>, sManager__15JKRThreadSwitch (sits in JKRHeap's
  .sbss split 0x8053A8A8-0x8053A8D8; it is JKRThread.cpp's static, so JKRHeap's .sbss
  should end at 0x8053A8D0 and JKRThread should own 0x8053A8D0-0x8053A8D8).
