# game/tu_80427D2C.cpp (THP player + sms THPDec/THPAudio)

Status: NonMatching. 162 / 168 functions at 100 % in fdiff; .data, .rodata, .ctors 100 %.
Closest donor: zeldaret/tww's d_a_movie_player.cpp (the same THPPlayer adaptation in one TU).

## Resistant (each has a FIXME in the source)
- MixAudio (98.6 %): r3-r7 permuted inside the loop.
- THPGXYuv2RgbSetup (98.4 %): scheduling of the fbWidth load vs the -1.0f literal.
- __THPInverseDCTY8 / __THPInverseDCTNoYPos / __THPReadQuantizationTable: .sdata2 literal
  order only (target: ReadQuantization's double before the IDCT floats).
- __sinit (99.3 %): calls the JASMemPool ctors kept at 0x8042E964.. (tu_8042E964's range).

## Order (why it can't flip yet)
With -inline off,deferred the target places the C-style wrappers (0x8042E1E4-0x8042E780) and the
JFWDisplay inlines after THPAudio.c, while their extab entries sit right after their first callers.
Lead for reaching Matching (reviewer's experiment, not adopted): move the wrappers into a THPPlayer
header and build with `-sym on`. MWCC then opens a .text section per #line file, and the wrappers and
the JFWDisplay.h inlines go to trailing sections in the target's order. Still open: under -sym on the
JUTVideo.h, JKRHeap.h (JKRFree) and GX header inlines also move to trailing sections (in the target
they are interleaved), and THPDec.c/THPAudio.c become separate sections. A guess, untested: those
headers came from a precompiled header. The .sdata2 order is unchanged by -sym on.

## Names
- THPStatistics (0x8052A540, 0x460): zeldaret/tww d_a_movie_player.cpp:29 and
  config/GZLE01/rels/d_a_movie_player/symbols.txt:190, the same slot in front of THPDec.c's statics.
- AudioSystem (0x8053AEA0): the SDK sample's name (oot-gc THPPlayer.c, tww daMP_AudioSystem).
- The movie layer, its statics, THPMovie_804B15D0's methods, the extra read-thread functions and
  cls_80529DEC::getHeap have no evidence: provisional names with their address.

## SHARED-EDITS
- (approved, bf27205) JFWDisplay.h getFrameRate()/setFrameRate(u16): getFrameRate comes from tww's
  JFWDisplay.h (and its weak getFrameRate__10JFWDisplayCFv in d_a_movie_player), setFrameRate from tp.
- (approved, bf27205) THPInit returns bool (callers test a byte; oot-gc's thp.h declares bool).
- include/game/GameGlobals.h: the singleton lbl_80529DEC (0x354 bytes, heap pointer at +8) is declared
  locally as cls_80529DEC; it belongs in a shared header.
- symbols.txt: lbl_80543A90/94 merged (one GXColorS10), lbl_8052AD00 and lbl_8052BECC split.
