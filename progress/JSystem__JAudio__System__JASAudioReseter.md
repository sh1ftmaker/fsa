# JSystem/JAudio/System/JASAudioReseter.cpp

Baseline (progress.py): total: 4501 / 11561 functions, 808092 / 4442088 bytes of code (18.19 %)
After: total: 4502 / 11561 functions, 808304 / 4442088 bytes of code (18.20 %); gate: ok.

Status: Matching (5/5 functions, .text and .sdata2 100 %).

## What was wrong
- The .text split ran to 0x8002AAF8, but 0x8002A740-0x8002AAF8 is JASAudioThread.cpp
  (ctor storing `this` to the JASAudioThread sInstance, stop -> OSJamMessage, run ->
  initAI/boot/initBuffer/initAll/JASChannel newMemPool, DMA/DSP callbacks, dtor, and the
  __sinit with the JASChannel pool). The .ctors entry 0x8045BD44 belongs to that __sinit.
  JASAudioReseter's own range is 0x8002A55C-0x8002A740 (5 functions = 0x1E4 bytes); calc
  is inlined into callback and the dtor is dead-stripped. It has no __sinit, so the
  JASChannel.h include (added only to emit a pool) was removed (mkdd's file doesn't have it).
- .sdata2 0x8053B0B0-0x8053B0B8: the u32->float conversion double used by calc.
- Names: lbl_8053A2A8 -> sInstance__35JASGlobalInstance<14JASAudioThread> (calc's getInstance,
  the ctor at 0x8002A740 stores this there); fn_8002A7F0 -> stop__14JASAudioThreadFv
  (called from calc as pAudioThread->stop(); body is OSJamMessage, as in tp).

## SHARED-EDITS (coordinator)
- New unit JSystem/JAudio/System/JASAudioThread.cpp: .text 0x8002A740-0x8002AAF8,
  .ctors 0x8045BD44-0x8045BD48, .data 0x804918D0-0x804918E0 (lbl_804918D0), .bss around
  lbl_804B2530 (pool dtor chain object), .sbss 0x8053A318-0x8053A320 (lbl_8053A318),
  .sdata2 0x8053B0B8-0x8053B0C0 (string for JASProbe::start). Needs a configure.py line.
- docs/COORDINATOR.md lists JASAudioReseter among TUs with the JASChannel pool: that pool
  is JASAudioThread's.
