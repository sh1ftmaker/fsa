# JSystem/J3DGraphAnimator/J3DAnimation.cpp

Baseline (progress.py first line at start): total: 3711 / 11561 functions, 710408 / 4442088 bytes of code (15.99 %)
After: total: 3747 / 11561 functions, 716852 / 4442088 bytes of code (16.14 %)
(+36 functions, +6444 bytes: exactly this unit's gain, so no other unit moved.)

Status: NonMatching. 62/62 functions (was 26/62), .text/.data/.sdata2 100 % in objdiff, but the
link SHA-1 fails: the weak ~J3DAnmBase lands after J3DAnmTransform's ctor instead of before it.

## Resistant: ~J3DAnmBase position (FIXME in source)
FSA has `__dt__10J3DAnmBaseFv` at 0x800C404C, between update and `__ct__15J3DAnmTransformFsPfPsPf`.
It must be inline in the header: FSA's J3DAnmLoader inlines it into ~J3DAnmTransformKey and
~J3DAnmVisibilityFull. MWCC emits a weak function right after the first function that inlines a
constructor storing the vtable (same mechanism places ~J3DAnmVtxColor after J3DAnmVtxColorFull's ctor
and ~J3DAnmColor after J3DAnmColorFull's ctor, which match). So some function between update and
J3DAnmTransform's ctor, dead-stripped in FSA, must construct a J3DAnmBase subclass. Tried, no effect:
non-inline J3DAnmBase(s16) after update or before update (an out-of-line copy of an auto-inlined
function does not trigger the emission). An out-of-line `J3DAnmBase::~J3DAnmBase() {}` here (SMS style)
links byte-identical, but contradicts the loader's bytes, so it is not used (commit 0ff90d0 had it,
52eba70 reverts it).

## What made it match
FSA's file has TP's layout (pikmin2's header-inline ctors were the difference):
- `J3DFrameCtrl::checkPass(f32)` (0x5A0) between init and update; TP/TWW body.
- Constructors defined in this .cpp (TP order), not inline in headers: J3DAnmTransform(s16, f32*, s16*, f32*),
  J3DAnmTextureSRTKey, J3DAnmVtxColor, J3DAnmVtxColorFull, J3DAnmVtxColorKey, J3DAnmColor,
  J3DAnmColorFull, J3DAnmColorKey, J3DAnmTevRegKey, J3DAnmTexPattern. Evidence: J3DAnmLoaderDataBase::load
  (0x800CEDxx) calls them with `bl`; the base-class ctors (VtxColor, Color) are auto-inlined and
  dead-stripped, and their weak dtors land right after the first derived ctor (0x800C550C, 0x800C5AD0),
  which our compile reproduces.
- New class J3DAnmTransformFullWithLerp (kind 16, vtable 0x8049A9CC: its dtor resets
  FullWithLerp -> TransformFull -> Transform -> Base vtables); getTransform from TP.
- Names: 36 fn_ and 16 vtable lbl_ from vtable slots (dtor chains and getKind return values:
  TexPattern 2, TevRegKey 5, Color 1, Cluster 3, TextureSRTKey 4, FullWithLerp 16, Transform 0).
- `lbl_8053BC30` (float 1023.0) resized 0x8 -> 0x4: the extra 4 bytes are alignment padding
  before J3DCluster's .sdata2.

## SHARED-EDITS (separate commit, evidence above)
- libs/sdk/include/JSystem/J3D/J3DAnmBase.h: `J3DAnmBase(s16)` ctor,
  `J3DAnmKind_TransformFullWithLerp = 16` (~J3DAnmBase stays inline, see above).
- J3DAnmTransform.h: `J3DAnmTransform(s16, f32*, s16*, f32*)` declared; Full/Key ctors call it;
  J3DAnmTransformFullWithLerp added.
- J3DAnmColor.h, J3DAnmVtxColor.h, J3DAnmTevRegKey.h, J3DAnmTexPattern.h, J3DAnmTextureSRTKey.h:
  ctors declared only (bodies moved to J3DAnimation.cpp unchanged).
- J3DFrameCtrl.h: `BOOL checkPass(f32)` declared.
Totals moved by exactly this unit's gain.
The same header commits are cherry-picked on unit/JSystem__J3DGraphLoader__J3DAnmLoader (identical).

## Notes for others
- J3DAnmLoaderDataBase::load sits at the end of the J3DShapeFactory.cpp .text split
  (0x800CEDxx, before 0x800CF3E8); the J3DAnmLoader unit needs that boundary moved.
- JUTNameTab's dtor is weak; FSA keeps the copy at 0x8008EAEC (fn_8008EAEC) in JUTNameTab's range.
