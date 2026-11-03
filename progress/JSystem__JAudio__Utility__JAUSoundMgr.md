# JSystem/JAudio/Utility/JAUSoundMgr.cpp

Baseline (progress.py): 4501 / 11561 functions, 808092 bytes (18.19 %).
After: 4506 / 11561, 808764 bytes (18.21 %). gate: ok. Unit Matching.

Unit: 1/6 -> 6/6 functions; .text, .ctors, .data, .bss 100 %.

## Matched
- calc, mixOut (mkdd names/order), stop() (0x8003C150: stop() on the three managers,
  no fade argument; declared in JAUSoundMgr.h, own commit).
- Pool set: JAISeq pool added by including JAISeMgr.h, JAISeq.h before JAUSoundMgr.h
  (FSA: MT x3, JAIStream, JAISeq, JAISe; plain pools come out in reverse
  instantiation order).
- fn_8002640C = JAISoundStarter::JAISoundStarter(bool) (sets sInstance if the flag is
  set, stores lbl_80491830 = JAISoundStarter's vtable, which its dtor fn_80026428 also
  stores); the ctor calls it with true.
- .bss 0x804B4B98-0x804B4BE0 (6 x 0xC dtor-chain objects, placeholder split).

## SHARED-EDITS
- libs/sdk/include/JSystem/JAudio/JAUSoundMgr.h: + stop() (also included by
  JAUAudioMgr.h and the unbuilt Inagaki/GameAudioMgr.h).
- symbols.txt: __ct__15JAISoundStarterFb (auto split at 0x8002640C).
