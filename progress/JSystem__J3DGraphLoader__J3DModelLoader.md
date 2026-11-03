# JSystem/J3DGraphLoader/J3DModelLoader.cpp

Baseline (progress.py first line at start): total: 4319 / 11561 functions, 772344 / 4442088 bytes of code (17.39 %)
After: total: 4337 / 11561 functions, 773248 / 4442088 bytes of code (17.41 %)

Status: Matching. 48/48 functions (was 30/48), .text and .data 100 %; gate: ok (link SHA-1 OK).
No source change: every miss was a placeholder name.

## Changes (symbols.txt only)
Names from the slots of this unit's vtables (same slot in our compiled object's vtable of the
same class), and the dtors from the vtable each one stores:
- 0x800CC1E8/0x800CC244/0x800CC2A0: __dt__ of J3DModelLoader_v26 / _v21 / J3DModelLoader.
- 0x800CC5A4/A8, 0x800CC6F4/F8: J3DModelLoader's empty readMaterial_v21 / readMaterial /
  readMaterialTable_v21 / readMaterialTable (slots 10/9/12/11 of __vt__14J3DModelLoader).
- 0x800CDF0C/14: calcSizeMaterial / calcSizeMaterialTable__14J3DModelLoader (slots 13/14).
- 0x800CCC54 __dt__19J3DMtxCalcNoAnmBaseFv; 0x800CDF64-0x800CE194 the __dt__/init/calc of the
  three J3DMtxCalcNoAnm<...> templates (Softimage's init was already named).
- Weak J3DMtxCalc base functions in the auto split at 0x8007903C-0x800790A4 (slots 1-6 of every
  J3DMtxCalc vtable here): setWeight (8007903C), setAnmTransform(UcP) (80079040),
  __dt__10J3DMtxCalcFv (80079044, stores __vt__10J3DMtxCalc), setAnmTransform(P) (8007908C),
  getAnmTransform(Uc) (80079090), getAnmTransform() (80079098), getWeight (800790A0).

## Notes
- Our object also defines loadBinaryDisplayList__22J3DModelLoaderDataBase, getFmtType and weak
  copies of J3DMtxCalc's functions/vtable; FSA dead-strips them or keeps an earlier TU's copy,
  and the link is byte-identical.

## SHARED-EDITS
- symbols.txt: the seven 0x8007903C-0x800790A0 placeholders above lie in auto_03_80079034_text
  (not a unit's split). Unit J3DShapeMtx's branch names fn_80079034 (getUseMtxNum__11J3DShapeMtxCFv)
  next to them; no overlap.
