# JSystem/J2DGraph/J2DScreen.cpp

Baseline (start): `total: 1199 / 11560 functions, 218212 / 4434096 bytes of code (4.92 %)`
Now:              `total: 1238 / 11560 functions, 225080 / 4434096 bytes of code (5.08 %)`

Unit: 4/20 -> 32/34 functions match (.text grew from 20 to 34 functions). .data, .sbss, .sdata2 100 %.
Not Matching: 2 functions wait on the JSU/J2DManage split task (below).
Progress total now: `total: 1258 / 11560 functions, 227708 / 4434096 bytes of code (5.14 %)`

## Round 2 (after coordinator rule update)

- configure.py: `-O4,s` -> `-O4,p` for this unit. Evidence: the target saves 2-4 GPRs
  with separate `stw`/`lwz` (e.g. `__dt__9J2DScreenFv`: `stw r31,0xc(r1)` / `stw r30,0x8(r1)`;
  `setPriority(const char*,...)`: four `stw r28..r31`) and uses `stmw` only for 5+
  (makeHierarchyPanes `stmw r21`, createPane `stmw r26`). `-O4,s` emits `stmw r30` /
  `stmw r28` there; `-O4,p` gives exactly the target in all functions.
- splits.txt: `.text` extended to 0x8009DFA0 (J2DTextBox ctor `fn_8009DFA0` starts there:
  J2DTextBoxEx's ctor calls it as its base ctor), and `.data 0x804997E8..0x80499884`
  (`__vt__9J2DScreen`, size 0x9C; the preceding jumptable/lbl belong to the 0x8009A708 TU).
- 0x8009DB58..0x8009DFA0 is this unit's: animation, setAnimation(Color, TextureSRTKey,
  TexPattern, TevRegKey, VtxColor, VisibilityFull), then the weak getTypeID, calcMtx,
  setAnimation(J2DAnmTransform), setAnimation(J2DAnmBase), clearAnmTransform,
  setAnimationVF, setAnimationVC: same order as the compiled mkdd object, and each one fills
  the matching `__vt__9J2DScreen` slot (J2DPane.h slot comments _0C.._8C).
- Renamed 32 more placeholders: those 14, plus the J2DPane functions they or the vtable
  reference (setCullBack(bool)/(GXCullMode), setConnectParent, update, drawSelf(ff),
  makeMatrix(ff), rewriteAlpha, setAnimation(J2DAnmTransform), clearAnmTransform,
  animationTransform, setVisibileAnimation (calls vtable 0x84 = setAnimationVF),
  setVtxColorAnimation (calls 0x8C = setAnimationVC), animationPane) and J2DMaterial's
  animation + 4 setAnimation (mkdd J2DMaterial.cpp order; call sites line up).
- set(name)/set(stream), gather, getMaterial: not in FSA's TU (createMaterial is followed
  directly by isUsed, search by searchUserInfo, clean by setPriority). Non-virtual and
  unreferenced, so most likely dead-stripped (mkdd, same JSystem era, has them): kept.
  They add no data (set(name) uses the same JSUMemoryInputStream inlines as setPriority).

## Resistant

- `setPriority(const char*,...)` 99.20 %: relocs only. Needs `lbl_80499028/4904C/49068` ->
  JSU vtables and `fn_80086A9C` -> `__dt__14JSUInputStreamFv` (coordinator's JSU split task).
- `getNameResource` 99.75 %: reloc only, needs `fn_800A68D4` -> `get__13J2DDataManageFPCc`
  (J2DManage split task).
- Matching link (tested with the flag flipped): fails only on `undef J2DDataManage::get(const char*)`;
  everything else links. Flip to Matching after those two renames land.

## SHARED-EDITS

1. J2DManage.cpp: `.text` should start at 0x800A68D4, then name `get__13J2DDataManageFPCc`.
2. JSUInputStream.cpp / JSUMemoryStream.cpp: split .data (0x80499028, 0x8049904C,
   0x80499068) and the dtor at 0x80086A9C, then name the three vtables and
   `__dt__14JSUInputStreamFv`.
