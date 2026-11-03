# JSystem/JAudio/System/JASBNKParser.cpp

Baseline (this round): `total: 4501 / 11561 functions, 808092 / 4442088 bytes of code (18.19 %)`
After: `total: 4524 / 11561 functions, 811828 / 4442088 bytes of code (18.28 %)`. Gate ok.

Unit: 3/16 -> 15/16 functions; .ctors/.data/.bss/.sbss/.sdata2 100 %. NonMatching
(Ver1::createBasicBank 96.7 %).

## Done
- Ported tp/SMG's two-version parser: createBasicBank (dispatch on header
  version), Ver1::createBasicBank (findChunk and createEffector inlined, as
  FSA has no copies of them), Ver0::createBasicBank, Ver0::findOscPtr, the
  nine weak JSUConvertOffsetToPtr<T> (names: tp's map for TInst/TOsc/TKeymap/
  TVmap/TPerc/TPmap, MarioClub's for TRand/TSense).
- Data: .data 0x80491A40-0x80491A50 (__vt__13JASInstEffect, weak copy, 0xC),
  .sdata2 0x8053B188-0x8053B1B0 (lbl_8053B1A8 resized 8 -> 4, the rest is
  alignment).

## Resistant
- Ver1::createBasicBank 96.7 %: FSA recomputes the inlined findChunk's
  `stream + 0x20` and `stream + header->mSize` for each of the three calls
  (ENVT/OSCT/LIST); our build CSEs them, which shifts all registers:
      50 lwz r0,0x4(r29); addi r15,r29,0x20; add r4,r29,r0   (x3 in FSA)
      ours: computed once, then mr r16,r5 / mr r17,r5
  Tried: tp/SMG body, end in loop condition, for loop, explicit inline, body
  in header, no header local, cast argument, cursor = param, definition after
  the caller (then not inlined). None stops the CSE.

## Review round 1 (rebased on matching 0c895ea)
- Approved renames applied: Ver0::getOscTableEndPtr; 0x8003D260 =
  JAUGenericAudible_<4,...>::calc (JAUAudience .data now 100 %, the empty
  JAUDopplerAudienceState ctor is gone).
- findChunk/createEffector inline (no extra .text); FIXME on Ver1.
- TPerc::appendEffect uses JASBasicInst's form: 100 %.
- After: 4599 / 11561 functions, 833596 bytes; ninja SHA-1 OK; gate ok.

## SHARED-EDITS (commit bd29082, header commit)
- JASBank.h: JASBank's 2nd virtual is getInstParam (vtable evidence);
  JASBasicBank mInstTable[0xF0], non-virtual getInst(int) const;
  JASVoiceBank.h overrides getInstParam.
- JASInst.h / JASDrumSet.h: JASBasicInst (0x20) and TPerc (0x1C) in FSA's
  layout, JASPtrList effects + appendEffect; JASGadget.h gains
  JASPtrListAdaptor/JASPtrList.
- JASBank.cpp (noteOn uses getInstParam), JASBasicBank.cpp (getInstParam,
  getInst from tp), JASBasicInst.cpp and JASDrumSet.cpp (appendEffect, list
  loops) adapted to compile. Side effect: JASBasicBank is now 100 % in code
  and data (could be flipped to Matching after link_check), JASBasicInst
  getParam/setKeyRegionCount/getKeyRegion/getVeloRegion x2 and JASDrumSet
  getParam/setVeloRegionCount/getVeloRegion now match.
- symbols.txt: placeholders in other units named (SMG map names):
  appendEffect x2, setKeyRegionCount, getKeyRegion, getVeloRegion x2,
  getInst, __vt__11JASInstRand / __vt__12JASInstSense (size 0xC).
- splits.txt: .data 0x80491A68-0x80491A78 to JASInstRand.cpp and
  0x80491A78-0x80491A88 to JASInstSense.cpp (their vtables; needed once
  the vtables have real names, else the Matching JASInstSense object's
  vtable is multiply defined).
- JASBasicInst.cpp is claimed by w7-jas2: their JASInst.h work will conflict
  with this commit's layout change (same evidence, same SMG layout).
