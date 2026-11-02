# JSystem/JAudio/System/JASChannel.cpp

Baseline (honest report, before any change on this branch):
`total: 3711 / 11561 functions, 710408 / 4442088 bytes of code (15.99 %)`
After: `total: 3735 / 11561 functions, 719936 / 4442088 bytes of code (16.21 %)`.

Unit: 8/23 -> 18/18 functions (the 5 trailing functions were JASDriverIF's),
code 6160/6160, .text/.ctors/.bss/.sbss/.sdata2 100 %, .data 0 %.
Stays NonMatching because of .data (below).

## memPool_ work (commits ed6be13, e37f581)
Generic `JASPoolAllocObject(_MultiThreaded)<T>::memPool_` definitions in
JASHeapCtrl.h, per-TU include fixes, JASFakeMatch headers retired. Gate ok.
Pool set per unit was checked against the ctor/dtor relocations of the
original objects (scratch script: JAudio units' __sinit pool order).

## Matched
All 18 functions. Fixes: tp's updateAutoMixer / updateMixer (mono output
mode handling), tp's pause handling in updateDSPChannel, `_66 = 0` in
initialUpdateDSPChannel, inverted isDolbyMode branch in updateEffectorParam.

## Resistant
- .data: our object has 5 switch tables, FSA 4. The extra one (`@323`)
  belongs to the out-of-line `effectOsc`, which FSA dead-stripped (its two
  inlined copies in initialUpdateDSPChannel / updateDSPChannel keep theirs).
  calcEffect, calcPan, copyOsc, ~JASChannel and sendBankDisposeMsg are
  dead-stripped too. Not fixable without making effectOsc inline, which
  nothing evidences. Check with link_check before ever flagging Matching.

## Symbols / splits
- fn_8002AC14/ACD8/AEDC/BC40/BD78 -> play, playForce, updateEffectorParam,
  updateAutoMixer, updateMixer (byte match).
- .text end 0x8002C360 -> 0x8002C308: 0x8002C308-0x8002C360 is
  JASDriverIF.cpp's first five functions (setDSPLevel, getChannelLevel_dsp,
  getDSPLevel, setOutputMode, getOutputMode; byte match), named.
- .data 0x804918E0-0x80491958, .bss 0x804B2540-0x804B2550,
  .sdata2 0x8053B0C0-0x8053B120 added. lbl_804B2540 0x10 -> 0xC,
  lbl_8053B118 0x8 -> 0x4 (padding), lbl_80539BD8 -> one__11JASWaveInfo
  (value 1, JASDSPInterface.cpp defines it; JASChannel's ctor references it).

## SHARED-EDITS
- JASDriverIF.cpp stanza: .text start 0x8002C360 -> 0x8002C308.
- memPool_ commits: JASHeapCtrl.h, JASTrack.h, new JASTrackPort.h,
  JASCmdStack.h, JAISound.h, new JAISoundChild.h, JAIStream.h, includes in
  JAI*/JAU*/JAS* .cpp files, JASFakeMatch*.h deleted.
