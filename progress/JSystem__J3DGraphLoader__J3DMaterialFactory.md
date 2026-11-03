# JSystem/J3DGraphLoader/J3DMaterialFactory.cpp

Baseline (start): `total: 3711 / 11561 functions, 710408 / 4442088 bytes of code (15.99 %)`
Now:              `total: 3909 / 11561 functions, 715140 / 4442088 bytes of code (16.10 %)`

Unit: 51/120 -> 119/120 functions; .text 99.99 %, .data/.sdata2/.sbss2 100 %. Stays NonMatching (one function).

## Done
- 60 weak virtuals / Null-block dtors named from vtable slots (+ newTexGenNum by position and callers).
- .sbss2 0x80543F08-0x80543F10 split here (newTevColor's zero GXColorS10); two placeholder resizes.
- Colour getters rewritten in tp/tww form.
- Branch carries cherry-picks of J3DMaterial-branch commits (names, J3DTexCoord()) and the
  J3DMatBlock branch's four header commits, unchanged.

## Resistant
- modifyPatchedCurrentMtx (99.66 %): `coords[i] = newTexCoord(...)` - target `clrlwi r0,i; lwz r3,8(r1);
  mulli r0,r0,6; stwx r3,r28,r0`, ours swaps r0/r3. tp only matches with a `*(u32*)` cast it calls a
  fakematch. FIXME in source.

## SHARED-EDITS (own commits on this branch)
- J3DTexMtx.h: J3DTexMtxInfo::operator= out of line; fn_800E4870 -> __as__13J3DTexMtxInfoFRC13J3DTexMtxInfo (J3DShapeTable range).
- J3DGXColor.h: tp ctors J3DGXColor(const GXColor) : GXColor(color), J3DGXColor copy = __memcpy,
  J3DGXColorS10(const GXColorS10&) : GXColorS10(color), J3DGXColorS10 copy = __memcpy.
  (v21's newTevColor dips 74 -> 55 % with this until the _v21 branch's source rewrite lands; merge them together.)
- J3DPE.h: J3DFog as tp (no explicit copy ctor, ~J3DFog() {}).
- symbols.txt outside range: getTevStageNum__11J3DTevBlockCFv (J3DPacket), getTexNo__11J3DTevBlockCFUl
  (J3DDrawBuffer), getTevColor/getTevKColor/setTexNo__11J3DTevBlock (J3DMaterialAnm).

## Wave 10 (J3D batch agent)
Baseline: `total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)`; unchanged
(no commit to source). Still 119/120, NonMatching; blocked by codegen, no other unit.
Tried on modifyPatchedCurrentMtx (all keep the same r0/r3 swap or get worse):
- J3DTexCoord::operator= through setTexCoordInfo (struct assign instead of __memcpy): identical
  99.66 %.
- J3DTexCoordInfo with the implicit copy assignment (no __memcpy operator=): newTexCoord and
  modifyPatchedCurrentMtx drop to ~32-36 %, J3DMatBlock/J3DMaterial weak copies regress. Reverted.
Left as is: tp's own `*(u32*)this = *(u32*)&other` is a self-declared fakematch.
