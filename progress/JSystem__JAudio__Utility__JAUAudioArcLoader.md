# JSystem/JAudio/Utility/JAUAudioArcLoader.cpp

Baseline (progress.py): 4501 / 11561 functions, 808092 bytes (18.19 %).
After: 4518 / 11561, 809168 bytes (18.22 %). gate: ok. Unit Matching.

Unit: 1/20 -> 18/18 functions (text now 0x8003AA08-0x8003AE9C), data, bss, ctors 100 %.

## Matched
- FSA has only the plain JAUAudioArcLoader (tp/Petari JAudio2 shape): newVoiceBank
  handler (vtable slot after readBMS_fromArchive), no JAUAudioArcLoader_withoutCopy.
  Header change in its own commit (JAUAudioArchive.h).
- Names from the vtable at 0x80495408 (ctor stores it; slot order = tp's) and the
  JAUSection calls each handler makes. `__sinit` already gives FSA's 4 pools
  (TChannelMgr, JASTrack, JASChannel, JAISe) with the generic memPool_ definition.
- Splits: .text end 0x8003AE9C, .data 0x80495408-0x80495450 (vtable 0x44 + pad),
  .bss 0x804B4A90-0x804B4AC0 (4 x 0xC destructor-chain objects, placeholders split).

## Split boundary
0x8003AE9C-0x8003AF6C (JAUAudience_withSetting_doppler / _withSetting dtors) and
.data 0x80495450-0x804954A0 (vtables of JAUAudience_withSetting, JAUSoundInfo
(lbl_80495470, its dtor in tu_8045A640 clears JASGlobalInstance<JAUSoundInfo>),
JAUAudience_withSetting_doppler) are not this TU's: nothing here uses JAUAudience,
and including JAUAudience.h emits none of them. They are weak copies from
JAUAudioMgr.cpp (JAUAudioMgr derives from JAUSoundInfo), whose .text then starts
at 0x8003AE9C; handled on the JAUAudioMgr branch.

## SHARED-EDITS
- libs/sdk/include/JSystem/JAudio/JAUAudioArchive.h (also used by
  JAUAudioArcInterpreter.cpp and the unbuilt Inagaki/GameAudioArcLoader.h).
