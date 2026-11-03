# JSystem/JAudio/Interface/JAISoundStarter.cpp

New unit (was unsplit, 0x8002640C..0x80026560). mkdd's file plus the JASTrack.h
include (FSA's __sinit builds the TChannelMgr/JASTrack/JASChannel pools, the
JAIAudible/JAISoundHandles precedent). 0/3 -> 3/3, Matching; `link_check` SHA-1 OK.

- .text: ctor, dtor, `__sinit_JAISoundStarter_cpp` (was fn_8002648C);
  startLevelSound is dead-stripped in FSA (compiled, unused in the link).
- .ctors 0x8045BD28, .data 0x80491830 `__vt__15JAISoundStarter` (was
  lbl_80491830), .bss 0x804B23D8-0x804B2400: placeholder lbl_804B23D8 (0x28)
  split into three 0xC placeholders lbl_804B23D8/E4/F0 (+4 bytes pad).
- Matches with and without `-sym off` (tested; links SHA-1 OK both ways).
