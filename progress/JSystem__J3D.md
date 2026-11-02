# J3D (import from projectPiki/pikmin2)

Branch `import/j3d`. 32 units, `libs/jsystem/JSystem/J3DGraph{Base,Animator,Loader}`,
GC/2.6 `-O4,p` (cflags_jsystem_mkdd), pikmin2 link order, TP path layout.
Headers: pikmin2's, in `libs/sdk/include/JSystem/J3D` (mkdd include tree),
adapted to the mkdd SDK headers. GX FIFO helpers / command enums that the
mkdd GX headers lack live in `J3DGD.h`.

Status: 638 / 1183 functions. Matching: J3DVertex, J3DCluster, J3DSkinDeform,
J3DModelLoaderCalcSize.

## Open problems (all units)

- `.bss`: J3D's statics are commons at 0x80536C50-0x80539BA0 (j3dSys,
  J3DSys statics, sGDLObj, j3dTexCoordTable..j3dZModeTable, sMtxLoadCache,
  J3DMtxBuffer statics). Splitting them `align:4 common` (as pikmin2 does)
  made the linker allocate j3dSys 0x22C and shift .sdata by 0xA0 (SHA-1
  fail); splitting without `common` gives a dtk link-order cycle. Left
  unsplit. symbols.txt sizes there are dtk guesses (j3dTexCoordTable 0x1DC7
  is really 0x2528); fix those first, then retry `common`.
- "missing" functions in fdiff are FSA functions whose donor twin was
  ambiguous (identical weak stubs, dtors) or that pikmin2 doesn't have.
  Name them from vtables (`__vt__*` are named in .data) and call sites.
- `GXWGFifo` in mkdd's GXVert.h is not volatile (pikmin2's is). Functions
  that write the FIFO several times in a row may need a volatile view; not
  yet seen in a diff.
- J3DBinaryFormat / J3DModelSaver: pikmin2 creates their weak functions
  with fabricated functions (fakematch); find where the weak really comes from.
- J3DTevs.cpp: pikmin2 puts two non-const tables in .rodata/.sdata2 with
  `__declspec(section ...)`; check whether `const` gives the same bytes.

## FSA-era variants

- J3DMatBlock (214/427, .text 60 %, .data 53 %): 209 of 213 misses are
  "missing" (placeholder names, mostly 8-byte stubs and block dtors with
  identical bytes). .data 0x8049AF18-0x8049BA98 holds the block vtables;
  most are still `lbl_`, so naming them from their slots names the stubs.
- J3DMaterialFactory (51/120, .data 54 %): 56 of 69 misses are missing names;
  the Null-block vtables in .data 0x8049AA38-0x8049AC88 are pikmin2's but
  FSA lacks J3DIndBlockNull/J3DPEBlock vtables here.
- J3DAnimation (26/62, .data 0 %): all 36 misses are missing names; .data
  0x8049A8E8-0x8049AA18 is 0x130 against pikmin2's 0xF0 (FSA has more or
  bigger anm vtables), all `lbl_`.
- J3DPacket (.data 0 %): FSA's .data is 4 vtables (0x50); pikmin2 adds
  sDifferedRegister / sSizeOfDiffered and J3DCallBackPacket's vtable.
  FSA has 6 small unnamed functions (fn_800BC460...).

## Names left for the J2D owner

`fn_800A6484` is `JSUConvertOffsetToPtr<J3DAnmVtxColorIndexData>` (weak, in
the J2D region; J3DAnmLoader calls it). Not renamed here.
