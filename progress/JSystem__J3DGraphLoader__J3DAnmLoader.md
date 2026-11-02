# JSystem/J3DGraphLoader/J3DAnmLoader.cpp

Baseline (progress.py first line at start): total: 3711 / 11561 functions, 710408 / 4442088 bytes of code (15.99 %)
After: total: 3741 / 11561 functions, 713800 / 4442088 bytes of code (16.07 %)
(per-unit compare against the base report: only J3DAnmLoader 29 -> 41 functions and J3DAnimation
26 -> 44, the latter from the shared J3DAnimation names; nothing else moved.)

Status: NonMatching. 41/41 functions (was 29/35; the split grew by 6), .text/.data 100 %,
.sdata2 75 %. gate: ok.

## Changes
- splits.txt: J3DAnmLoader .text starts at 0x800CECCC, not 0x800CF3E8. The six functions
  0x800CECCC-0x800CF3E8 (load, J3DAnmFullLoader_v15 ctor/dtor, ~J3DAnmLoader, J3DAnmKeyLoader_v15
  ctor/dtor) were at the end of J3DShapeFactory.cpp's range; their vtables are this unit's .data.
  J3DShapeFactory.cpp's .text now ends at 0x800CECCC (after its last JSUConvertOffsetToPtr<_GXVtxDescList>).
- J3DAnmLoaderDataBase::load takes `J3DAnmLoaderDataBaseFlag` (mkdd/TP); flag bit 1 builds a
  J3DAnmTransformFullWithLerp for 'bca1' (target tests `clrlwi. r30, 31` and stores vtable 0x8049A9CC).
  Mangled name `load__20J3DAnmLoaderDataBaseFPCv24J3DAnmLoaderDataBaseFlag` as in mkdd's symbols (same size 0x5E4).
- fn_800D06B8/06D0 = JSUConvertOffsetToPtr<J3DAnmCluster{Key,Full}Table>__FPCvPCv (both overloads
  compile to the same bytes; every donor map uses the void* form, offsets stay `void*`).
- Names: vtables 0x8049AE10.. (KeyLoader_v15, FullLoader_v15, J3DAnmLoader, J3DAnmVisibilityFull
  (getKind 6), J3DAnmTransformKey (getKind 8)) and their functions, from vtable slots.

## Resistant
- .sdata2 (0x8053BC80, 8 bytes): target is 0.0f + 4 zero bytes; ours 0.0f, 1.0f. The 1.0f belongs to
  the weak J3DAnmCluster::getWeight copy this TU emits (the kept copy is in J3DAnimation, 0x800C6DD4);
  FSA's linker stripped the duplicate and left padding. Resizing lbl_8053BC80 to 4 made it worse (gap
  symbol, 66 %). Not fixable in source; the unit can't be flagged Matching while objdiff counts this.

## SHARED-EDITS
- Cherry-picked from unit/JSystem__J3DGraphAnimator__J3DAnimation (identical commits): the J3D header
  commit (out-of-line J3DAnm ctors, J3DAnmTransformFullWithLerp, checkPass) and the follow-up that keeps
  ~J3DAnmBase inline (this unit's ~J3DAnmTransformKey/~J3DAnmVisibilityFull need it inline).
- symbols.txt: the 52 J3DAnimation renames (ctors, dtors, vtables), identical lines to that branch,
  needed for this unit's relocations.
- splits.txt: J3DShapeFactory.cpp .text end 0x800CF3E8 -> 0x800CECCC.
