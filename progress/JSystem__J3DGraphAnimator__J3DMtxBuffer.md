# JSystem/J3DGraphAnimator/J3DMtxBuffer.cpp

Baseline (progress.py first line at start): total: 4319 / 11561 functions, 772344 / 4442088 bytes of code (17.39 %)
After: total: 4325 / 11561 functions, 773736 / 4442088 bytes of code (17.42 %)

Status: Matching. 12/12 functions (was 7/11), .text/.sdata/.sdata2 100 %; gate: ok (link SHA-1 OK).

## Changes
- J3DMtxBuffer.h (own commit, cherry-picked from unit/JSystem__J3DGraphBase__J3DShapeMtx):
  `Mtx* mUserAnmMatrices` at 0x34, vtable 0x38 (createAnmMtx stores it; J3DModel allocates 0x3C).
  initialize() clears it, createAnmMtx() sets it to the anim matrices (tp).
- J3DCalcViewBaseMtx (own commit): out of line at the end of J3DMtxBuffer.cpp, declared in
  J3DMtxBuffer.h (tp); FSA calls it from calcDrawMtx and J3DModel. Was inline in J3DModel.h.
- calcBBoardMtx: tp's version (unconditional 1/x, if/else-if on the joint's mtx type); the joint
  index as u32 (our J3DJointTree getters take/return u32) and the count as u32 give FSA's code.
- splits.txt: J3DShapeDraw/J3DMtxBuffer boundary 0x800E3A20 -> 0x800E39DC. 0x800E39DC is
  J3DMtxBuffer::initialize (zeroes 0x00-0x34, mViewCount = 1; J3DModel calls it right after
  storing J3DMtxBuffer's vtable at 0x38), the first function of J3DMtxBuffer.cpp (pikmin2, tp).
- symbols.txt: fn_800E39DC initialize__12J3DMtxBufferFv, fn_800E413C calcDrawMtx__12J3DMtxBuffer...,
  fn_800E46C0 J3DCalcViewBaseMtx__FPA4_fRC3VecRA3_A4_CfPA4_f, fn_800E3994 __dt__12J3DShapeDrawFv
  (stores __vt__12J3DShapeDraw; J3DShapeDraw 2 -> 3 matched).

## SHARED-EDITS
- splits.txt: J3DShapeDraw.cpp .text end 0x800E3A20 -> 0x800E39DC; symbols.txt fn_800E3994 in its range.
- libs/sdk/include/JSystem/J3D/J3DMtxBuffer.h, J3DModel.h (two header commits; J3DModel.cpp
  unchanged in the report, now calls J3DCalcViewBaseMtx like FSA).
