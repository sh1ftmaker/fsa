# JSystem/JAudio/System/JASAiCtrl.cpp

Baseline (progress.py): total: 4501 / 11561 functions, 808092 / 4442088 bytes of code (18.19 %)
After: total: 4507 / 11561 functions, 809016 / 4442088 bytes of code (18.21 %); gate: ok.

Status: Matching (18/18 functions, all sections 100 %). No source change: names and splits only.

## Names (symbols.txt placeholders)
- fn_80029A10 / fn_80029A30 -> startDMA__9JASDriverFv / stopDMA__9JASDriverFv: bodies are
  AIStartDMA() / AIStopDMA(), byte-identical to this file's compiled functions at the same offsets.
- fn_8002A504 -> __sinit_JASAiCtrl_cpp (scope:local): the unit's .ctors entry 0x8045BD40;
  constructs the JASChannel pool, same as the compiled __sinit.
- fn_8002C454 / fn_8002C47C / fn_8002C4A4 (JASDriverIF range) -> subframeCallback /
  DSPSyncCallback / updateDacCallback__9JASDriverFv: each calls JASCallbackMgr::callback on
  the 2nd / 1st / 3rd static manager of JASDriverIF (0x804B2650 / 0x804B2550 / 0x804B2750),
  in the order of JASDriverIF's compiled object; updateDSP/finishDSPFrame call them at the same spots.
- fn_8002CE0C (JASDSPInterface range) -> syncFrame__6JASDspFUlUlUl: wraps DsyncFrame2(UlUlUl),
  called by finishDSPFrame at the place our source calls JASDsp::syncFrame.
- lbl_8053A318 (0x8) -> snIntCount__14JASAudioThread (0x4) + lbl_8053A31C (0x4, unreferenced).
- lbl_8045CAEC (0x20) split into the two strings it covers (0x11 at 0x8045CAEC, 0x9 at 0x8045CB00).

## Splits
- .bss extended to 0x804B2530: 0x804B2524 is the __sinit's pool destructor-chain object (@411).

## SHARED-EDITS
- None to shared files. The JASDriverIF/JASDSPInterface/JASAudioThread placeholder renames are
  in symbols.txt only. The JASDSPInterface branch renames fn_8002CE0C identically.
