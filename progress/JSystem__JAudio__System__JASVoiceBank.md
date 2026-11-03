# JSystem/JAudio/System/JASVoiceBank.cpp

New unit (was unsplit, 0x80039A84..0x80039B7C, after JASTrackPort). tp's
JASVoiceBank.cpp (mkdd has none) on mkdd's headers: JASInstParam fields are
mkdd's _18/_1c/_20 (stw 0x18/0x20/0x1c in the asm). 0/4 -> 4/4, Matching;
`link_check` SHA-1 OK.

- getInstParam, ~JASVoiceBank, getType ('VOIC'), `__sinit_JASVoiceBank_cpp`
  (JASChannel pool, from JASBank.h's JASChannel.h).
- .ctors 0x8045BD80, .rodata 0x8045D398 sOscData (tp's {0, 1.0f, 0, 0, 1.0f, 0}),
  .data 0x80493670 __vt__12JASVoiceBank (+ pad to jdsp's 32-aligned 0x804936A0),
  .bss 0x804B2980 (placeholder resized 0x20 -> 0xC; pad to audio_task),
  .sbss 0x8053A398 sOscTable (placeholder lbl_8053A398 0x8 -> 0x4).
- JASVoiceBank.h gains tp's two statics.
- 0x80039B7C..0x80039B80 is alignment padding before dspproc.c (not this unit).
