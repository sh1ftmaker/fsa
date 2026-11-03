# JSystem/J3DGraphBase/J3DShapeMtx.cpp

Baseline (progress.py first line at start): total: 4319 / 11561 functions, 772344 / 4442088 bytes of code (17.39 %)
After: total: 4336 / 11561 functions, 775768 / 4442088 bytes of code (17.46 %)
(+2 in J3DShapeFactory from the two weak dtor names below.)

Status: NonMatching, code complete. 37/37 functions (was 22/37), .text/.ctors/.sbss/.sdata2 100 %,
.data 99.83 %: only `__vt__11J3DShapeMtx` differs, its symbols.txt size is 0x24 (dtk counted
the 4 bytes of section padding before 0x8049BD98); the vtable is 0x20 (2 header words + 6 slots).
Needs a coordinator size change (SHARED-EDITS), then the unit can be flipped to Matching.

## Changes
- symbols.txt names from the vtables in this unit's .data (each slot names its function):
  fn_800E1EC8 load__21J3DShapeMtxConcatViewCFv, fn_800E221C load__26...MultiConcatViewCFv,
  fn_800E2540 load__27...BBoardConcatViewCFv, fn_800E2648 load__28...YBBoardConcatViewCFv,
  the __dt__/getUseMtxNum/getUseMtxIndex/loadNrmMtx(iUs) weak functions at 0x800E2784-0x800E297C
  (dtors confirmed by the vtables they store).
- data: lbl_8049BAC0/BB20/BB50 split into the pointer-to-member constants @396-@407 (0xC each,
  mkdd style) and sMtxLoadPipeline__21J3DShapeMtxConcatView; lbl_8049BBB0 sMtxLoadLODPipeline__21...;
  qMtx$435/qMtx2$436; switch tables @550/@549; mtxCache; lbl_8053AA18 sMtxPtrTbl__21... (all
  addresses equal our object's layout).
- J3DMtxBuffer.h (own commit): FSA has tp's `mpUserAnmMtx` at 0x34 (vtable 0x38, size 0x3C).
  The ConcatView/Multi/BBoard/YBBoard loads use getUserAnmMtx, as tp.
- BBoard load: tp/tww's unconditional 1/x (no zero checks); loadExecute: tp's
  getTexCoord(i)->getTexGenType() 0/1 check around both switches.

## SHARED-EDITS
- `__vt__11J3DShapeMtx` size 0x24 -> 0x20 (real symbol: approved_symbol_changes.txt, coordinator).
- symbols.txt placeholders outside this unit, named from this unit's vtables:
  fn_80079034 -> getUseMtxNum__11J3DShapeMtxCFv (`li r3,1; blr`, slot 2 of __vt__11J3DShapeMtx,
  in auto split auto_03_80079034_text), fn_800CE8EC -> __dt__21J3DShapeMtxConcatViewFv,
  fn_800CE948 -> __dt__11J3DShapeMtxFv (both in J3DShapeFactory.cpp's range; they store those vtables).
- libs/sdk/include/JSystem/J3D/J3DMtxBuffer.h: new member mUserAnmMatrices at 0x34 (separate commit).
