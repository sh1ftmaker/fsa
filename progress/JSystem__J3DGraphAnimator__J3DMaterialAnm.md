# JSystem/J3DGraphAnimator/J3DMaterialAnm.cpp

Baseline (progress.py first line at start): total: 3711 / 11561 functions, 710408 / 4442088 bytes of code (15.99 %)
After: total: 3717 / 11561 functions, 710620 / 4442088 bytes of code (16.00 %)
(+6 functions, +212 bytes: exactly this unit's gain.)

Status: Matching. 15/15 functions (was 9/15), .text/.data 100 %. gate: ok (link SHA-1 ok).

## Changes
- J3DMaterialAnm::setMatColorAnm/setTexMtxAnm/setTexNoAnm/setTevColorAnm/setTevKColorAnm defined
  in the .cpp in TP's order instead of inline in the header. FSA's J3DMaterialAttach calls
  setTexMtxAnm/setTevColorAnm/setTevKColorAnm with `bl` (0x800DFD68, 0x800DFE44, 0x800DFED0);
  setMatColorAnm and setTexNoAnm are dead-stripped in FSA.
- Names from bytes: fn_800C768C/76CC/770C store at mTexMtxAnmList (0x14), mTevColAnmList (0xB4),
  mTevKColAnmList (0xD4) + idx*8 -> setTexMtxAnm, setTevColorAnm, setTevKColorAnm.
  fn_800C7648/7650/7658 sit at J3DTevBlock vtable slots 0x74/0x68/0x4C (J3DMatBlock's
  __vt__11J3DTevBlock) and are `li r3,0; blr` / `blr` -> getTevKColor, getTevColor,
  setTexNo(u32, u16) (weak, first emitted here; called from calc).

## SHARED-EDITS (separate commit)
- libs/sdk/include/JSystem/J3D/J3DMaterialAnm.h: the four inline setters become declarations
  (setTexNoAnm already was). Totals moved by exactly this unit's gain.
