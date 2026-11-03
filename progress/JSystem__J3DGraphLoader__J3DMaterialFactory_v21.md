# JSystem/J3DGraphLoader/J3DMaterialFactory_v21.cpp

Baseline (start): `total: 3711 / 11561 functions, 710408 / 4442088 bytes of code (15.99 %)`
Now:              `total: 3912 / 11561 functions, 715428 / 4442088 bytes of code (16.11 %)`

Unit: 17/25 -> 25/25, every section 100 %. Matching; gate ok (link SHA-1 ok).

## How
- Branch is built on top of the J3DMaterialFactory branch (same commits, reset to its tip
  426978c) - it needs that branch's shared header commits (J3DTexMtxInfo::operator= out of line,
  J3DGXColor(S10) tp ctors, J3DFog as tp, the J3DMatBlock-branch header cherry-picks, J3DTexCoord()).
  Merge J3DMaterialFactory first (or together); its commits merge as-is.
- Own commit: newMatColor/newTevColor/newTevKColor in tp form; .sbss2 0x80543F10-0x80543F18 split,
  lbl_80543F10 resized to 0x8 (lbl_80543F14 folded in).

## SHARED-EDITS
- None beyond those inherited from the J3DMaterialFactory branch.
