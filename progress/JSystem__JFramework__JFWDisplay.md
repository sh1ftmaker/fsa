# JSystem/JFramework/JFWDisplay.cpp

Baseline (progress.py first line at start): total: 1199 / 11560 functions, 218212 / 4434096 bytes of code (4.92 %)

After: total: 1228 / 11560 functions, 225428 / 4434096 bytes of code (5.08 %)

Status: Matching, gate: ok. All 20 target functions at 100 % (was 9/18); .text/.ctors/.rodata/.data/.bss/.sbss/.sdata2 100 %.

## Changes
Source (libs/jsystem/JSystem/JFramework/JFWDisplay.cpp, mkdd donor):
- beginRender: wholeLoopEnd / wholeLoopStart+idleStart are each under `if (mIsWaiting)`;
  `_34 = _2C - JUTVideo::getVideoLastTick()` (the old DEBUG-only branch is what FSA has).
- endFrame: the prevFrame / setCostFrame block is under a second `if (mIsWaiting)`.
- clearEfb(iiii): trailing `GXSetDstAlpha(GX_DISABLE, color.a)` (was 0).
- clearAllXfb added (pikmin2 lists it at this position, size 0x1D4 = FSA's fn_800E5FC4):
  prepareCopyDisp + clearEfb + preGX inlined, then GXCopyDisp + JFWDrawDoneAlarm per xfb.
- Removed mkdd's fabricated frameToTick.

splits.txt: .data end 0x8049BEC0 -> 0x8049BEB8 (the 8 zero bytes are alignment padding of the
next object; with them the link shifted), .text extended to 0x800E6594 (__sinit + JSUList<JFWAlarm> dtor), .ctors 0x8045BDEC,
sList__8JFWAlarm as `.bss ... align:4 common` at 0x80539BA0 (as in mkdd's splits).

symbols.txt (placeholder -> name, evidence):
- vtable __vt__10JFWDisplay slots: fn_800E4EE0 beginRender, fn_800E5748 endFrame.
- fn_800E5FC4 clearAllXfb (pikmin2 size note 0x1D4), fn_800E64F8 __sinit_JFWDisplay_cpp,
  fn_800E6540 __dt__18JSUList<8JFWAlarm>Fv (registered by the sinit).
- lbl_80463588 string pool split into the six OSReport strings @1384..@1389 (data:string).
- lbl_8049BE20 -> e_mtx (0x30), lbl_804BA5C8 -> @264 (dtor chain), lbl_8053AA4C/50 ->
  prevFrame$1063 / init$1064 (endFrame statics).
- Other units' placeholders, all by donor (tww/mkdd/AC) symbol order + size + bytes:
  fn_80067944 __cvt_dbl_usll (runtime.c, after __shr2i, 0xCC), fn_80084370 __nw__FUl,
  dtor_80084580 __dl__FPv (JKRHeap), fn_80087BAC flushMessage_dbPrint__12JUTAssertionFv,
  fn_80088D1C draw__17JUTConsoleManagerCFv, fn_8008CB40 control__8JUTFaderFv (0x100 as tww),
  fn_8008EEA4 draw__10JUTProcBarFv (0x34), fn_80093048 waitRetraceIfNeed__8JUTVideoFv,
  fn_80093108 destroyManager__6JUTXfbFv, lbl_8053A984 sVideoLastTick__8JUTVideo,
  lbl_8053A988 sVideoInterval__8JUTVideo, lbl_8053A8F0 sDebugPrint__10JUTDbPrint (resized 8 -> 4).

## SHARED-EDITS
- libs/sdk/include/JSystem/JUtility/JUTXfb.h: add `void *getBufferPtr(int i) const { return mBuffer[i]; }`
  (pikmin2's JUTXfb.h has `getBufferPtr(int)`); needed by clearAllXfb. Separate commit.
- symbols.txt renames of other units' placeholders listed above.
