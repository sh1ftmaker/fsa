# JSystem/JAudio/Utility/JAUAudioMgr.cpp

## Wave 13 (2026-11-03)
Before 4/5; after 5/5, code 100 %, .sdata2 93.75 %. Still NonMatching.
- cls_804B21EC -> JAUAudible<4> (symbols.txt memPool_/guard/JASMemPool dtor) together
  with include/game/GameAudioBase.h (JAUAudience.h + FIXME_instantiateAudiblePool stand-in).
  Full honest report vs matching: only this unit changes (+1 function); every game unit
  identical; ninja SHA-1 OK; gate ok.
- Blocker for the flip: `link_check --unit` fails (with -sym off and on). The .data
  vtable order differs: FSA has __vt__23JAUAudience_withSetting (0x80495450),
  __vt__12JAUSoundInfo (0x80495470), __vt__31JAUAudience_withSetting_doppler (0x80495480);
  ours emits the doppler vtable before JAUSoundInfo's, so after dead-stripping they swap
  (the honest report matches .data by name, so it shows 100 %). The order comes from
  the fabricated JAUAudioMgr class (mkdd: bases JAUSoundMgr, JAUSoundInfo; member
  JAUAudience_<..., doppler>); FSA's real class is dead-stripped. Not guessed at.

## Wave 10 (2026-11-02)
Baseline (progress.py): total 4784 / 11561 functions, 863464 / 4442088 bytes (19.44 %).
After: unchanged (no commit-worthy change found). Since the `-sym on` merge the unit
is 4/5: `__ct__17JAUAudibleChannelFv` now matches and .sdata2 is 93.75 % (ours has
1.0, 0.0, 0.5, 32.0; FSA 1.0, 0.0, 0.5 + pad: the 32.0 is from dead-stripped code).
- `__sinit_JAUAudioMgr_cpp` 99.63 %: still only the cls_804B21EC pool name (below).
  Blocked by the cls_804B21EC rename (coordinator + include/game/GameAudioBase.h).
- .sdata2: the extra 32.0 can only be dropped by the honest report's dead-data rule,
  which needs a SHA-1-OK trial link; `link_check --unit` fails today (DOL SHA-1
  a2a071ac...) because the __sinit references memPool_<JAUAudible<4>> instead of the
  symbol at 0x804B21EC. Resizing placeholder lbl_8053B2A0 0x8 -> 0x4 lowers .sdata2
  to 85.71 % until then (tried, reverted).
- Next: after the cls_804B21EC -> JAUAudible<4> rename, rerun link_check --unit; if
  SHA-1 OK, the 32.0 should be stripped as dead data and the unit can flip.

## Earlier wave
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
