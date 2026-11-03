# JSystem/J3DGraphBase/J3DMaterial.cpp

Baseline (start): `total: 3711 / 11561 functions, 710408 / 4442088 bytes of code (15.99 %)`
Now:              `total: 3840 / 11561 functions, 713116 / 4442088 bytes of code (16.05 %)`

Unit: 36/155 -> 155/155 functions, .text 100 %, .data 100 %. Matching; `gate.py`: ok (link SHA-1 ok).

## What it took
- 113 placeholder `fn_` weak virtuals/destructors named slot-for-slot from the
  target vtables in this unit's .data (and J3DTevBlock's vtable in J3DMatBlock);
  three deleting dtors named from the vtable they store.
- Shared headers: cherry-picked the J3DMatBlock branch's four header commits
  (J3DGD.h blend mask; __memcpy 4-byte info copies; out-of-line
  J3DIndTexMtxInfo/J3DFogInfo/J3DNBTScaleInfo operator=; J3DGXColor.h) unchanged,
  plus one own commit: J3DTexCoord() as tp (`J3DTexCoordInfo::operator=(j3dDefaultTexCoordInfo[0])`).
  Honest report: nothing worse anywhere.

## SHARED-EDITS
- Headers above. The four J3DMatBlock-branch commits are identical cherry-picks; only the J3DTexCoord() commit is new.
- symbols.txt outside this unit's range (placeholders only):
  `j3dDefaultIndTexMtxInfo` (0x804634D4), `j3dDefaultFogInfo`
  (0x80463510) [J3DTevs .rodata]; `__vt__13J3DPEBlockOpa`, `__vt__17J3DPEBlockTexEdge`,
  `__vt__13J3DPEBlockXlu`, `__vt__14J3DPEBlockFull`, `__vt__16J3DPEBlockFogOff` [J3DMatBlock .data];
  `getTexGenNum__14J3DTexGenBlockCFv` (0x800BC9CC) [J3DPacket .text].

## Notes
- Source still defines pikmin2's `copy` and `makeDisplayList_private` (dead-stripped in FSA);
  the link is byte-identical with them.
