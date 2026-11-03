# JSystem/JAudio/Utility/JAUAudioMgr.cpp

Baseline (progress.py): 4501 / 11561 functions, 808092 bytes (18.19 %).
After: 4504 / 11561, 808624 bytes (18.20 %). gate: ok.

Unit: 0/3 -> 3/5 functions (text now 0x8003AE9C-0x8003B288), .text 99.80 %, .data,
.bss, .ctors 100 %, .sdata2 62.5 %. NonMatching (see Resistant: needs a shared
rename, and .sdata2 can't match while the TU's dead-stripped code is compiled).

## What the unit is
Everything FSA kept from this TU is weak copies (first definitions in link order) plus
`__sinit`: the dtors of JAUAudience_withSetting_doppler and JAUAudience_withSetting,
JAUSoundMgr's dtor, the JAUAudibleChannel ctor, and the vtables of
JAUAudience_withSetting, JAUSoundInfo (lbl_80495470: its dtor in tu_8045A640 clears
JASGlobalInstance<JAUSoundInfo>::sInstance) and JAUAudience_withSetting_doppler. The
JAUAudioMgr class's own code is dead-stripped. Split moved: .text start 0x8003AE9C
(the two JAUAudience dtors were in JAUAudioArcLoader's range: that TU has nothing
of JAUAudience), .data 0x80495450-0x804954A0, .bss 0x804B4AC0-0x804B4B18 (7 x 0xC
dtor-chain objects, placeholder split), .sdata2 0x8053B298-0x8053B2A8.

## Matched
- Both JAUAudience dtors (header commit: audience member on the doppler setting
  base, see its message), JAUSoundMgr dtor (fn_80026428 = __dt__15JAISoundStarterFv:
  stores lbl_80491830, clears JASGlobalInstance<JAISoundStarter>::sInstance).
- Pool set fixed by includes: JAISeMgr.h, JAISeq.h before JAUAudioMgr.h gives FSA's
  order (MT x3, JAUAudible<4>, JAIStream, JAISeq, JAISe).

## Resistant
- `__sinit_JAUAudioMgr_cpp` 99.63 %: only the pool's name. FSA's 4th pool is
  `memPool___34JASPoolAllocObject<12cls_804B21EC>`; ours is JAUAudible<4>. This TU is
  evidence for cls_804B21EC = JAUAudible<4>: the JAUAudibleChannel ctor (only used by
  JAUGenericAudible_<N, JAUAudibleChannel, ...>, i.e. JAUAudible<N>) is kept from here
  and it is the TU's only non-JAI plain pool. SHARED-EDIT (coordinator/game header
  owner): rename memPool_/__init__memPool_/JASMemPool dtor of cls_804B21EC to
  JAUAudible<4> and update include/game/GameAudioBase.h.
- `__ct__17JAUAudibleChannelFv` 98.89 %: FSA's .sdata2 has 1.0, 0.0, 0.5 (the ctor's
  load order, so it was the first float user in FSA's TU); ours puts 0.0 first (our
  fabricated JAUAudioMgr ctor and newAudible use it before) and adds 32.0, 3.0, 1/15
  and the int->float double from the dead-stripped audience code. Not fixable without
  knowing FSA's JAUAudioMgr source; same for .sdata2 62.5 %.

## SHARED-EDITS
- symbols.txt: fn_80026428 -> __dt__15JAISoundStarterFv (auto split, JAISoundStarter).
- splits.txt: JAUAudioArcLoader .text end 0x8003AE9C (same line as on its branch).
- cls_804B21EC rename (above).
