# JSystem/JAudio/System/JASAramStream.cpp

Baseline: `total: 3711 / 11561 functions, 710408 / 4442088 bytes of code (15.99 %)`
After (this branch, incl. the cherry-picked memPool_ commits): 3730 / 11561, 718136 bytes.

Unit: 10/25 -> 22/23 functions (the old range's last two were JASRegisterParam's);
every data section 100 %. Stays NonMatching: load() differs.

## Changes (symbols/splits only; source unchanged apart from the memPool_ commits)
- fn_80031E34 -> loadToAramTask, fn_800323A0 -> channelProcCallback,
  fn_80032FC0 -> __sinit_JASAramStream_cpp (byte match, donor order).
- fn_800282C0 -> JASDvd::getThreadPointer (initSystem calls it),
  fn_8002ACD8 -> JASChannel::playForce (channelStart; same line as the JASChannel branch).
- .sbss lbl_8053A368..79 -> sLoadThread, sReadBuffer, sBlockSize,
  sChannelMax, sSystemPauseFlag, sFatalErrorFlag (relocations in the matched functions);
  sFatalErrorFlag 0x7 -> 0x1.
- .rodata lbl_8045D1D0/DC -> OSC_RELEASE_TABLE / OSC_ENV (local, tp's names;
  OSC_ENV holds a pointer to OSC_RELEASE_TABLE), OSC_ENV 0x1C -> 0x18.
- lbl_804B28D8 0x10 -> 0xC, lbl_8053B218 0x8 -> 0x4 (padding).
- Splits: .text end 0x80033098 -> 0x80033018 (JASRegisterParam's ctor and init
  start there); .data 0x80491B38-0x80491B70, .bss 0x804B28D8-0x804B28E8,
  .sbss 0x8053A368-0x8053A380 added.

## Resistant
- load(): 98.79 %, register allocation only: target has this=r27, loop i=r28,
  aram base=r26; ours this=r28, i=r26, base=r27. tp's statement order
  (offset before size, no `block` local) and a hoisted `int i` give identical code.

## w10-jas (2026-11-02)
Unit unchanged at 22/23 (load 98.79 %), NonMatching. Codegen, not blocked by another unit.
More load() variants, none better: base address inlined into the JKRMainRamToAram argument 93.3 %;
`int i` hoisted to the top 98.79 % (identical). tp's own version needs a pointer-cast fakematch on
bhead->mSize (its comment), which is banned here.

## SHARED-EDITS
- JASRegisterParam.cpp stanza: .text start 0x80033098 -> 0x80033018.
- memPool_ commits cherry-picked from the JASChannel branch (ed6be13, e37f581).
