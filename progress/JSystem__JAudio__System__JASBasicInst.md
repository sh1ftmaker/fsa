# JSystem/JAudio/System/JASBasicInst.cpp

**Update (infra/jaudio-sym): Matching.** With the mkdd_JAudio block's `-sym on` (and mkdd's `#line`s gone) the weak copies land at the file end like FSA; `link_check.py --unit` SHA-1 OK. The notes below describe the earlier state.

Baseline (progress.py): total: 4501 / 11561 functions, 808092 / 4442088 bytes of code (18.19 %)
After: total: 4510 / 11561 functions, 809040 / 4442088 bytes of code (18.21 %); gate: ok.

Status: 15/15 functions and every section 100 % in the honest report, but **NonMatching**:
flipping it breaks the link (SHA-1 fails; tested with the full ninja link).

## Why it can't link yet (resistant: layout, not bytes)
Our object emits the out-of-line copies of inline functions right after their first use:
__dt__7JASInstFv after the ctor, __ct__Q212JASBasicInst7TKeymapFv after setKeyRegionCount.
The original has them at the end of the file (… getVeloRegion const, __dt__7JASInst,
TKeymap ct, getType, __sinit), as TP's and SMG's maps do too. Same issue the JASTrack worker
recorded ("weak functions emitted after first use; FSA has them at the end") and JASBasicBank
shows (__dt__7JASBank). Tried GC 2.5/2.6/2.7/3.0a3, -inline auto/noauto/on/deferred, no
JASInst ctor: no change (deferred reverses the whole file). -sym on puts the weak functions in
a separate .text section in exactly the original tail order, but also puts the ctor in its own
section; not pursued (flag change for all of JAudio, unverified link order). Worth a
project-level look: it blocks every JAudio unit with surviving weak functions.

## Way forward (found at the end, on the JASWaveArcLoader branch)
`-sym on` (TP's JSystem flag) makes MWCC 2.6 emit the weak copies into a trailing .text section,
exactly FSA's order; JASWaveArcLoader is Matching with it. For this file it gives, once no `#line`
is seen: section 1 = ct, dt, getParam ... getVeloRegion const; then __dt__7JASInst, TKeymap ct,
getType; then __sinit = the original layout (checked with a scratch copy of the headers with
every `#line` blanked). But each `#line` starts a new .text section under -sym on, and this TU
includes `#line`s in JASGadget.h (172, 215), JASHeapCtrl.h (187, 315, 357, 390), JUTGamePad.h,
JGadget/linklist.h (mkdd's assert-line fakes; JUT_ASSERT is `(void)0` in this NDEBUG build). With
them the ctor lands in its own section after the others and the link fails. Next step (shared
header edit for the coordinator): drop those `#line`s (release builds don't use them; check no
`__LINE__` user in NDEBUG code), drop the 6 in JASBasicInst.cpp, add "-sym on" to this unit, flip.

## Done
- Source: FSA's JASBasicInst is SMG's (JASPtrList of effects + appendEffect, ctor with
  initializer list, dtor only deletes the keymaps, getParam walks the effect list).
  appendEffect matches with `if ((it = mEffects.begin()) == NULL)` (other forms load to r0 first).
- Header commit (JASInst.h, JASGadget.h) with evidence, identical to the JASBNKParser branch's text.
- Names: fn_8002E65C setKeyRegionCount, fn_8002E6E8 appendEffect__12JASBasicInstFP34JASPtrListAdaptor<13JASInstEffect>
  (SMG map), fn_8002E728 getKeyRegion__12JASBasicInstFi, fn_8002E800/824 getVeloRegion (non-const
  called by JASBNKParser on a non-const keymap, const by getParam), fn_8002E8B4 __sinit_JASBasicInst_cpp (.ctors 0x8045BD58).
- Splits: .data 0x804919A8-0x804919D0 (__vt__12JASBasicInst, __vt__7JASInst), .bss 0x804B2860-0x804B2870
  (pool dtor chain object; lbl_804B2860 resized 0x10 -> 0xC like JASBasicBank's), .sdata2 0x8053B180-0x8053B188
  (1.0f; lbl_8053B180 resized 0x8 -> 0x4, the rest is alignment).

## SHARED-EDITS
- JASInst.h / JASGadget.h (same text as the JASBNKParser branch).
- JASBNKParser.cpp: its two setEffect calls changed to appendEffect(new JASPtrListAdaptor) so it compiles;
  the JASBNKParser branch's version supersedes it.
