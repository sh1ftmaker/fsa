# JSystem/JAudio/Utility/JAUSoundObject.cpp

Baseline (progress.py): 3711 / 11561 functions, 710408 bytes (15.99 %).
After: 3727 / 11561, 714200 bytes (16.08 %). gate: ok. Per-unit honest comparison against
the baseline report: no function lost anywhere; JAUSoundObject 1 -> 12, plus JAISeMgr,
JAISeqMgr, JAIStreamMgr, JAUSoundMgr +1 each from the shared names.

Unit: 1/13 -> 12/13 functions; .data 96.9 %, .bss/.ctors 0 % (no __sinit).

## Matched
FSA has the later JAUSoundObject (Petari's JAudio2 version, not mkdd's): an
`mIsAllocated` flag at 0x0C (set by the ctor, cleared by dispose, checked by
process/start*), the reserved-handle mask `_10` at 0x10, and startSoundIndex /
startLevelSoundIndex / stopSound. Vtable `__vt__14JAUSoundObject` (lbl_804955B8) slots:
process, dispose, stopOK, startSound, startSoundIndex, startLevelSound,
startLevelSoundIndex: these name fn_8003F00C/EF58/F03C/F110/F184/F290; ctor/dtor by
position and the vtable stores; stopSound (fn_8003F37C) as in Petari's order.
- `fn_8003B320`/`fn_8003B2D0` = JAISoundHandle ctor/dtor (the ctor/dtor passed to
  __construct_new_array, element size 4). `sInstance` of JAISoundStarter (lbl_8053A2A0),
  JAISeMgr (lbl_8053A29C), JAISoundInfo (lbl_8053A294). JAISe pool names (from __sinit).
- Splits: .data 0x804955B8-0x804955E0, .bss 0x804B4CC8-0x804B4D00.

## Resistant
- `__sinit_JAUSoundObject_cpp`: FIXME, waits for the memPool_ definition (w3-jas).
  JASFakeMatch2.h include removed (empty without MATCHING).
- .data 96.9 %: our object also has `__vt__21JAUDopplerSoundObject` (mkdd's class, kept
  in the .cpp because mkdd-derived game headers use it); FSA's unit has no Doppler code.
  Whether FSA had it (dead-stripped) is unknown; decide before going Matching.

## SHARED-EDITS
- libs/sdk/include/JSystem/JAudio/JAUSoundObject.h: FSA layout (mIsAllocated, u32 _10)
  and the three extra members/virtuals. Separate commit; no other unit changed.
- symbols.txt names in other units' ranges: JAISoundHandle ctor/dtor, JASGlobalInstance
  sInstances, JAISe pool / guard / dtor (identical lines to the JAISeMgr branch).
