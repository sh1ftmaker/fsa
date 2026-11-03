# JSystem/JAudio/Utility/JAUAudience.cpp

## Wave 13 (2026-11-03): no source change, 11/18 (data 38 %)
- __sinit (9 pools). FSA constructs, after the 3 MT pools: 0x8052E78C, 0x8052E79C,
  0x8052E7AC (local, own guards 0x8053A3C0..), JAUAudible<4> (0x804B21EC, named on
  unit/JAUAudioMgr), 0x8052E7BC, JAUDopplerAudible<4> (0x804B21DC). MWCC constructs a
  group in reverse instantiation order, so FSA's TU instantiated D<4>, 7BC, A<4>, 7AC,
  79C, 78C. SMG's dummies (A1, A2, A4, D1, D2, D4, after the class's own D<4>) give
  D2, D1, A4, A2, A1, D4: A<4> third, not fourth. No donor gives FSA's order; any
  arrangement of dummies that does would be invented. Left as is.
- newAudible (99.9 %): FSA allocates from 0x804B21DC (the pool symbols.txt calls
  JAUDopplerAudible<4>) and passes __ct__17JAUAudibleChannelFv to __construct_array.
  A derived JAUDopplerAudibleChannel always gets its own implicit ctor, and FSA has
  none, so FSA's audible uses JAUAudibleChannel itself as its channel (matching the
  0x24 channel and the missing doppler term in mixChannelOut). The `24JAUDopplerAudibleChannel`
  in four real symbols.txt names (getOuterParams/__dt__/calc/__vt__ of
  JAUGenericAudible_<4,...>) comes from the mkdd import, not FSA. Proposal (needs a
  name review and approved_symbol_changes.txt lines): make the channel parameter
  JAUAudibleChannel and rename those four to `...<4,17JAUAudibleChannel,23JAUDopplerAudibleAbsPos>...`.
  Not applied: renames of real names are coordinator-reviewed.
- calcPriority 98.9 %: register allocation only (see Resistant); not retried.

Baseline (this round): `total: 4501 / 11561 functions, 808092 / 4442088 bytes of code (18.19 %)`
After: `total: 4506 / 11561 functions, 809288 / 4442088 bytes of code (18.22 %)`. Gate ok.
Per-unit diff against the baseline report: only JAUAudience changed (6 -> 11).

Unit: 6/18 -> 11/18 functions. .data 95.2 %, .sdata2 96.8 %, .bss 0 %. NonMatching.

## Done (commits 238e9b1, 9ee352e)
- JAUAudience.h: FSA's layout (see the commit message for the byte evidence):
  JAUGenericAudible_ 0x40 bytes with one channel slot, 0x24-byte channel
  (params, relPos_, dist_), empty AbsPos, 0x30-byte audience state (Mtx);
  mixChannelOut/calcPriority/newAudible rewritten to FSA's code.
- JAUAudience3DSetting::init (SMG map name) defined first in the .cpp: gives
  FSA's .sdata2 order; updateSetting and the ctor now match.
- Placeholders named: mixChannelOut / calcPriority / newAudible (vtable slots).

## Resistant
- calcPriority 98.9 %: register allocation only, second loop: FSA channel
  r29 / deltaPriority cursor r26 / rv r25 / i r24; ours r24 / r29 / r26 / r25.
  Declaring channel at function scope gets 99.4 %; taking the channel from the
  state's inline (c9 variant) gets the registers right but reloads. Not kept.
- newAudible 99.9 %: FSA passes `__ct__17JAUAudibleChannelFv` (0x8003B0B0,
  JAUAudioMgr's range, a real name from the mkdd import) to __construct_array;
  ours emits `__ct__24JAUDopplerAudibleChannelFv`. The bytes are identical
  (JASSoundParams init). FSA has no copy of a Doppler-channel ctor, so either
  that symbol's real name is __ct__24JAUDopplerAudibleChannelFv (rename
  proposal, needs evidence of which TU emitted it first), or FSA's channel
  storage is JAUAudibleChannel. Undecided.
- __sinit_JAUAudience_cpp 15 % and fn_8003D488..D584 (local JASMemPool dtors):
  9 pools: 3 MT (TChannelMgr, JASTrack, JASChannel), then 8052E78C, 8052E79C,
  8052E7AC, cls_804B21EC, 8052E7BC, JAUDopplerAudible<4>. SMG's dummies
  (newAudible_1/2/4, newDopplerAudible_1/2/4) suggest JAUAudible<1,2,4> and
  JAUDopplerAudible<1,2,4>, but that order puts JAUAudible<4> third, while
  cls_804B21EC (GameAudioBase's probable JAUAudible<4>) is fourth. Not done.
- .data 95 %: the calc slot of both audible vtables. FSA's blr at 0x8003D260
  is named `__ct__23JAUDopplerAudienceStateFv` in symbols.txt, but it is only
  referenced from those two vtables' calc slot. Rename proposal (needs
  approval): `__ct__23JAUDopplerAudienceStateFv` ->
  `calc__74JAUGenericAudible_<4,24JAUDopplerAudibleChannel,23JAUDopplerAudibleAbsPos>Fv`
  (SMG's map has the same calc for AudGenericAudible_). Our object still
  emits the state ctor (testInstanciate) so that function keeps matching.

## For the coordinator (GameAudio.h FIXME stand-in)
Nothing new proven about what instantiates the JAUDopplerAudible<4> pool in
the game units; the pool is constructed in this unit's __sinit (last of 9).

## SHARED-EDITS
- libs/sdk/include/JSystem/JAudio/JAUAudience.h (used by JAUAudioMgr,
  JAUSeqDataBlockMgr, Inagaki headers and the game's GameAudio.h): layout
  change. No other unit's matched functions changed (per-unit report diff).
- JAUAudience_withSetting: setting_ is now a JAUAudience3DSetting (0x38),
  the derived _3c.._4c are direct members.
