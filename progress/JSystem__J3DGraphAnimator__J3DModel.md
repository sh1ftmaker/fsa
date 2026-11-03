# JSystem/J3DGraphAnimator/J3DModel.cpp

Baseline (progress.py first line at start): total: 4319 / 11561 functions, 772344 / 4442088 bytes of code (17.39 %)
After: total: 4323 / 11561 functions, 773996 / 4442088 bytes of code (17.42 %)

Status: Matching. 16/16 functions (was 13/16), .text/.data/.sdata2 100 %; gate: ok (link SHA-1 OK).

## Changes
- Header commits cherry-picked from the J3DShapeMtx / J3DMtxBuffer branches (same commits):
  J3DMtxBuffer's user anim matrix at 0x34 (size 0x3C, J3DModel allocates 0x3C and writes the
  vtable at 0x38), and J3DCalcViewBaseMtx out of line (FSA's viewCalc calls it at 0x800C31F8 and
  0x800C3244) instead of pikmin2's inline in J3DModel.h.
- entryModelData: tp's mdlFlags -> onFlag() block (flags 1, 2, 0x10, 0x20000, 0x40000, 0x80000,
  4, 8 in FSA's order), with pikmin2's J3DMODEL_* enum names.
- symbols.txt: fn_800C2794 entryModelData, fn_800C3150 viewCalc, fn_800C35C0
  __dt__12J3DMtxBufferFv (stores __vt__12J3DMtxBuffer); and, as on the J3DMtxBuffer branch
  (identical lines), fn_800E39DC initialize__12J3DMtxBufferFv, fn_800E413C calcDrawMtx,
  fn_800E46C0 J3DCalcViewBaseMtx (viewCalc/entryModelData call them).
- lbl_8053BBF8 (size 0x8: 1.0f + 4 zero bytes) -> @410, size 0x4: the only constant J3DModel
  uses; the zero word is .sdata2 alignment padding before 0x8053BC00 (link stays identical).

## SHARED-EDITS
- symbols.txt: the three J3DMtxBuffer names above (same lines as unit/JSystem__J3DGraphAnimator__J3DMtxBuffer).
- J3DMtxBuffer.h / J3DModel.h header commits (same as on the J3DMtxBuffer branch).
