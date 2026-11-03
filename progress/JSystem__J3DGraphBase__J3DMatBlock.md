# JSystem/J3DGraphBase/J3DMatBlock.cpp

Baseline (start of this session): progress.py `total: 3711 / 11561 functions, 710408 / 4442088 bytes of code (15.99 %)`;
unit 214 / 427 functions, .data 52.6 %.
Now: progress.py `total: 3914 / 11561 functions, 729332 / 4442088 bytes of code (16.42 %)`;
unit 408 / 427 functions (.text 99.6 % fuzzy), .data 81.1 %. `gate: ok`. Stays NonMatching.

## Done
- 209 `fn_` placeholders in the unit's .text named from the .data vtables, slot by slot
  (target vtable slot vs the same slot of our object's vtable of the same class);
  5 `lbl_` vtables named `__vt__14J3DPEBlockFull`, `__vt__16J3DPEBlockFogOff`,
  `__vt__13J3DPEBlockXlu`, `__vt__17J3DPEBlockTexEdge`, `__vt__13J3DPEBlockOpa` by their getType slot.
- J3DGD.h: blend BP mask 0xFE00FFE3 / 0xFE00FFE7 (tp), FSA writes 0xFF.
- J3DTypes.h / J3DInd.h: 4-byte *Info structs copy with `__memcpy` (tp); J3DTexCoord's
  assignment copies only the Info part.
- J3DIndTexMtxInfo / J3DFogInfo / J3DNBTScaleInfo `operator=` out of line (tp's J3DStruct.cpp);
  placeholders 0x800E48EC / 0x800E4910 / 0x800E49BC named. J3DNBTScale ctors field-wise (tp).
- J3DGXColor.h: `operator=(const GXColor)` by value, J3DGXColorS10 assignment `__memcpy` (tp).
Every header change was checked against the honest report: no unit lost a function.

## Resistant
- setTevOrder x10, setTexCoord, setIndTexOrder x2, setIndTexCoordScale x2 (20 bytes, 96 %):
  target `lwz r5,0(r5); addi r0,r4,off; stwx r5,r3,r0`, ours `lwz r0,0(r5); addi r4,r4,off; stwx r0,r3,r4`.
  Only tp's `*(u32*)this = *(u32*)&other` reproduces it (tested: setTevOrder 100 %), and tp itself
  labels that a fakematch for J3DTexCoord/J3DIndTexOrder; not used.
- initialize__12J3DTevBlock2/4, __13J3DTevBlock16, __18J3DTevBlockPatched (82-97 %): FSA loads the
  second word of j3dDefaultTevColor via `li rX, j3dDefaultTevColor@sda21; lwz 0x4(rX)`; we emit
  `lwz j3dDefaultTevColor+0x4@sda21`. TevBlock4 then also needs a 5th saved register (stmw r27).

## .data (81 %)
- FSA's object has no vtables for the abstract bases J3DPEBlock, J3DTexGenBlock, J3DColorBlock,
  J3DIndBlock (ours emits them, +0x1BC bytes): in FSA the base classes' weak functions and dtors
  live in other TUs (see list below; e.g. `__dt__21J3DColorBlockLightOffFv` is at 0x800BDC0C).
- Vtable slots point at weak base-class functions that are still `fn_` placeholders in other units.

## SHARED-EDITS (not applied; other units' placeholders)
Evidence: the same slot of the same vtable in our compiled J3DMatBlock.o.
- fn_800BC9C4 -> getTevStageNum__11J3DTevBlockCFv
- fn_800BDC0C -> __dt__21J3DColorBlockLightOffFv
- fn_800BDECC -> __dt__21J3DTexGenBlockPatchedFv
- fn_800BE63C -> __dt__11J3DTevBlockFv
- fn_800BECC8 -> countDLSize__11J3DTevBlockFv
- fn_800BECFC -> load__11J3DTevBlockFv
- fn_800BF31C -> getNBTScale__14J3DTexGenBlockFv
- fn_800BF424 -> patch__11J3DTevBlockFv
- fn_800BF628 -> diff__10J3DPEBlockFUl
- fn_800BF9B4 -> reset__10J3DPEBlockFP10J3DPEBlock
- fn_800BF9BC -> reset__11J3DTevBlockFP11J3DTevBlock
- fn_800BFD78 -> patch__10J3DPEBlockFv
- fn_800BFD7C -> diffFog__10J3DPEBlockFv
- fn_800BFD80 -> diffBlend__10J3DPEBlockFv
- fn_800BFD84 -> setFog__10J3DPEBlockFP6J3DFog
- fn_800BFD88 -> setFog__10J3DPEBlockF6J3DFog
- fn_800BFD8C -> getFog__10J3DPEBlockFv
- fn_800BFD94 -> setAlphaComp__10J3DPEBlockFRC12J3DAlphaComp
- fn_800BFD98 -> setAlphaComp__10J3DPEBlockFPC12J3DAlphaComp
- fn_800BFD9C -> getAlphaComp__10J3DPEBlockFv
- fn_800BFDA4 -> setBlend__10J3DPEBlockFRC8J3DBlend
- fn_800BFDA8 -> setBlend__10J3DPEBlockFPC8J3DBlend
- fn_800BFDAC -> getBlend__10J3DPEBlockFv
- fn_800BFDB4 -> setZMode__10J3DPEBlockF8J3DZMode
- fn_800BFDB8 -> setZMode__10J3DPEBlockFPC8J3DZMode
- fn_800BFDBC -> getZMode__10J3DPEBlockFv
- fn_800BFDC4 -> setZCompLoc__10J3DPEBlockFUc
- fn_800BFDC8 -> setZCompLoc__10J3DPEBlockFPCUc
- fn_800BFDCC -> getZCompLoc__10J3DPEBlockCFv
- fn_800BFDD4 -> setDither__10J3DPEBlockFUc
- fn_800BFDD8 -> setDither__10J3DPEBlockFPCUc
- fn_800BFDDC -> getDither__10J3DPEBlockCFv
- fn_800BFDE4 -> getFogOffset__10J3DPEBlockCFv
- fn_800BFDEC -> setFogOffset__10J3DPEBlockFUl
- fn_800BFEB0 -> setNBTScale__14J3DTexGenBlockF11J3DNBTScale
- fn_800BFEB4 -> setNBTScale__14J3DTexGenBlockFPC11J3DNBTScale
- fn_800BFEE4 -> setAmbColor__13J3DColorBlockFUl10J3DGXColor
- fn_800BFEE8 -> setAmbColor__13J3DColorBlockFUlPC10J3DGXColor
- fn_800BFEEC -> getAmbColor__13J3DColorBlockFUl
- fn_800BFF0C -> setLight__13J3DColorBlockFUlP11J3DLightObj
- fn_800BFF10 -> getLight__13J3DColorBlockFUl
- fn_800C20E8 -> getTexNo__11J3DTevBlockCFUl
- fn_800C7648 -> getTevKColor__11J3DTevBlockFUl
- fn_800C7650 -> getTevColor__11J3DTevBlockFUl
- fn_800C7658 -> setTexNo__11J3DTevBlockFUlUs
- fn_800C9CF4 -> setIndTevStage__11J3DTevBlockFUl14J3DIndTevStage
- fn_800C9CF8 -> setTevKAlphaSel__11J3DTevBlockFUlUc
- fn_800C9CFC -> setTevKColorSel__11J3DTevBlockFUlUc
- fn_800C9D00 -> setTevSwapModeTable__11J3DTevBlockFUl19J3DTevSwapModeTable
- fn_800C9D04 -> setTevColor__11J3DTevBlockFUl13J3DGXColorS10
- fn_800C9D08 -> setTevKColor__11J3DTevBlockFUl10J3DGXColor
- fn_800C9D0C -> getTevStage__11J3DTevBlockFUl
- fn_800C9D14 -> setTevStage__11J3DTevBlockFUl11J3DTevStage
- fn_800C9D18 -> setTevOrder__11J3DTevBlockFUl11J3DTevOrder
- fn_800C9D1C -> setTevStageNum__11J3DTevBlockFUc
- fn_800CAAA8 -> setTevRegOffset__11J3DTevBlockFUl
- fn_800CAAAC -> setTexNoOffset__11J3DTevBlockFUl
- fn_800CBD44 -> diffTexNo__11J3DTevBlockFv
- fn_800CBD48 -> diffTevReg__11J3DTevBlockFv
- fn_800CBD4C -> diffTexCoordScale__11J3DTevBlockFv
- fn_800CBD50 -> diffTevStage__11J3DTevBlockFv
- fn_800CBD54 -> diffTevStageIndirect__11J3DTevBlockFv
- fn_800CBD58 -> patchTexNo__11J3DTevBlockFv
- fn_800CBD5C -> patchTevReg__11J3DTevBlockFv
- fn_800CBD60 -> patchTexNoAndTexCoordScale__11J3DTevBlockFv
- fn_800CBD64 -> setTexNo__11J3DTevBlockFUlPCUs
- fn_800CBD68 -> setTevOrder__11J3DTevBlockFUlPC11J3DTevOrder
- fn_800CBD6C -> getTevOrder__11J3DTevBlockFUl
- fn_800CBD74 -> setTevColor__11J3DTevBlockFUlPC13J3DGXColorS10
- fn_800CBD78 -> setTevKColor__11J3DTevBlockFUlPC10J3DGXColor
- fn_800CBD7C -> setTevKColorSel__11J3DTevBlockFUlPCUc
- fn_800CBD80 -> getTevKColorSel__11J3DTevBlockFUl
- fn_800CBD88 -> setTevKAlphaSel__11J3DTevBlockFUlPCUc
- fn_800CBD8C -> getTevKAlphaSel__11J3DTevBlockFUl
- fn_800CBD94 -> setTevStageNum__11J3DTevBlockFPCUc
- fn_800CBD98 -> setTevStage__11J3DTevBlockFUlPC11J3DTevStage
- fn_800CBD9C -> setTevSwapModeInfo__11J3DTevBlockFUl18J3DTevSwapModeInfo
- fn_800CBDA0 -> setTevSwapModeInfo__11J3DTevBlockFUlPC18J3DTevSwapModeInfo
- fn_800CBDA4 -> setTevSwapModeTable__11J3DTevBlockFUlPC19J3DTevSwapModeTable
- fn_800CBDA8 -> getTevSwapModeTable__11J3DTevBlockFUl
- fn_800CBDB0 -> setIndTevStage__11J3DTevBlockFUlPC14J3DIndTevStage
- fn_800CBDB4 -> getIndTevStage__11J3DTevBlockFUl
- fn_800CBDBC -> getTexNoOffset__11J3DTevBlockCFv
- fn_800CBDC4 -> getTevRegOffset__11J3DTevBlockCFv
- lbl_804634D4 -> j3dDefaultIndTexMtxInfo, lbl_80463510 -> j3dDefaultFogInfo (J3DMaterial
  __ct__12J3DIndTexMtxFv and J3DMaterialFactory::newFog differ only by these names now).
- 0x800E48EC..0x800E49DC (and 0x800E4870) sit in J3DShapeTable.cpp's split but are tp's
  J3DStruct.cpp operators; I named three of them (commit 1be38d9), coordinator may move that.

## Next
- Name the externals above (owner of J3DMaterial.cpp range), then see which base-class functions
  FSA's J3DMatBlock really defines, to drop the four extra base vtables.

## Wave 8 (w8-j3d), 2026-11-03
Baseline: total: 4501 / 11561 functions, 808092 / 4442088 bytes of code (18.19 %).
Unit: 408 of 427 functions at 100 %, 19 below (.text 99.64 %, .data 93.44 %). Unchanged; FIXME comments added
in J3DMatBlock.cpp. Stays NonMatching.

### 15 setters (96 %), codegen, not blocked by another unit
Target `slwi r4,r4,2; lwz r5,0(r5); addi r0,r4,off; stwx r5,r3,r0`; ours loads into r0 and adds into r4.
Scratch harness (unit flags), variants of the 4-byte Info copy:
- `__memcpy` operator= (void or returning ref), derived Ord::operator= calling Info's, local pointer
  `Ord* p = &m[i]; *p = o`, direct `__memcpy(&m[i], &o, 4)`, explicit `m[i].Info::operator=(o)`: all r0 form.
- implicit copy of u8[4] struct, `#pragma pack(2)`, aligned(4) attribute, member-wise u8 copy (tww's
  J3DIndTexOrderInfo/J3DTexCoordInfo style): 4x lbz/stb.
- tww's J3DTevOrderInfo (u8 fields, ALIGN_DECL(2), no operator=): lhz/lbz.
- a real u32 member (`u32 x`, bitfields, union with u32) through operator= or implicit copy: folds to
  `add; stw r4,0xc(r3)`.
- `Info* p = &m[i]; p->x = o.x` / `u32* p = &m[i].x; *p = o.x` with a u32 member: matches, but needs the
  Info struct to be a u32 (it is accessed as u8 fields everywhere), so not real.
- `*(u32*)this = *(u32*)&other`: matches (tp; tp labels it a fakematch for J3DTexCoord/J3DIndTexOrder/
  J3DIndTexCoordScale; tp's J3DTevOrderInfo::operator= uses the same cast unlabelled). Not used.
The `stwx` + r5 reuse appears only when the store goes through a pointer the compiler can't fold to
`this+off` and the value is a typed word load, never with __memcpy.

### 4 initialize functions (TevBlock2 97.6 %, 16 94.7 %, 4 86.1 %, Patched 82.1 %), codegen
FSA reads j3dDefaultTevColor's second word through a base register (`li rX, sym@sda21; lwz 0x4(rX)`),
which costs one more saved register (stmw r27 / r21) and shifts the allocation. Variants:
- J3DGXColorS10 `__memcpy` operator= (current), copy-ctor + assignment, by-value parameter: `lwz sym+4@sda21`.
- GXColorS10 struct copy, member-wise r/g/b/a, tww's `mColor = color`: 4x lha/sth with the base register.
- tp's `((u32*)this)[0] = ((u32*)&color)[0]; [1] = [1]`: matches (tp: "Fakematch? ... might be real").
  Not used.

### .data 93.44 %: tooling, not source
Our object emits the weak vtables `__vt__10J3DPEBlock`, `__vt__11J3DIndBlock`, `__vt__14J3DTexGenBlock`,
`__vt__13J3DColorBlock` (readelf: OBJECT WEAK), because the derived dtors inline the base dtors
(the target's dtors do too: `__dt__13J3DPEBlockOpaFv` stores `__vt__10J3DPEBlock`). In FSA those vtables
sit in J3DMaterial.cpp's .data (0x8049A4D0-0x8049A6D8), which is earlier in link order, so MWLD kept
J3DMaterial's copy and dropped J3DMatBlock's. That is the weak-duplicate case of CLAUDE.md, for .data
instead of extab: honest_report.py only strips weak extab entries, so .data can't reach 100 % from
source. SHARED-EDIT (tooling, coordinator): extend the weak-dedup evidence to WEAK OBJECTs that the
map lists as UNREFERENCED DUPLICATE for this object.

### Is J3DMatBlock blocked by another unit?
No: the 19 functions are codegen; .data is the tool limitation above.

## Wave 9 crack pass (w9-crack), 2026-11-02
Goal: a natural (non-`*(u32*)`) source form for the 15 setters and 4 initialize functions. Result: none
exists; the layout of FSA's own objects rules out every u32-typed Info. Unit unchanged at 408 / 427
(FIXME wording updated with the evidence below). No header change kept.

### Decisive evidence (target asm)
- `setIndTexOrder__15J3DIndBlockFull`: `addi r0,r4,0x5`, so `mIndTexOrder` sits at offset 0x5: J3DIndTexOrder has
  alignment 1. `setTexCoord__21J3DTexGenBlockPatched`: `mulli r4,r4,6` (J3DTexCoord stride 6, alignment <= 2).
  `J3DTevBlock1::load/initialize`: `mTevOrder` at 0xA (alignment <= 2). So no Info can contain a u32 (union or
  bitfield) and the target's `lwz` is a u32-typed access to byte storage: a cast. `__memcpy` is the only other
  word copy and always gives the r0 form.
- tp ShieldD (debug) symbols.txt: `__as__15J3DTevOrderInfoFRC15J3DTevOrderInfo`, `__as__23J3DIndTexCoordScaleInfo...`,
  `__as__15J3DTexCoordInfo...`, `__as__18J3DIndTexOrderInfo...` all size 0xC: a user-declared Info::operator= that
  is one word copy (`lwz r0; stw r0; blr`). Checked with GC/2.6 at -O0: the cast and `__memcpy` both give 0xC;
  pikmin2/mkdd's field-wise operator= gives 0x34. Consistent with tp's cast, rules out field-wise.
- J3DGXColorS10: base is `_GXColorS10` (ShieldD `__as__13J3DGXColorS10FRC11_GXColorS10`), s16-only, so the two
  `lwz` (second through a base register) also need a cast; `__memcpy` gives `sym+4@sda21`.

### Variants tried this pass (scratch harness with the unit's flags; then the real build where noted)
Harness reproduces target with tp's cast, and ours with `__memcpy`.
- anon union {u8 fields; u32 w} with `w = o.w` operator= (void / returning ref / this->w / by value): folds to `add; stw 0xc`.
- union Info as a member of Ord; plain `u32 w` Info; Info with u32 bitfields, implicit copy: fold to `add; stw`.
- u32-typed Info (union, plain u32, bitfields) + `J3DTevOrder::operator=(const J3DTevOrder&) { *(Info*)this = o; }`
  or `{ getTevOrderInfo() = o; }`: matches in the harness. REAL BUILD (all four Infos as `u32 x:8` bitfields, derived
  operator= via base): 13 setters reach 100 %, but layout breaks: TevBlock1 mTevOrder moves 0xA->0xC, IndBlockFull
  mIndTexOrder 0x5->0x8, TexCoord stride 6->8; 50 functions below 100 %. Reverted. Ruled out.
- aligned(4) attribute on the struct or first member, u16+u8+u8 layout, `*(Info*)&m[i] = *o` with u8 Info,
  `*(m + i) = *o`: byte/halfword copies.
- `Ord(const Info&)` ctor + `m[i] = Ord(*o)`: stack temp.
- `set(const Ord&)` with `__memcpy`, `memcpy` (real call), `__memcpy(&m[i], o, 4)`, `const Ord&` param,
  `int` index: all r0 form.
- field-wise operator= in either order: 4x lbz/stb.
- GXColorS10: union {GXColorS10; u32 w[2]} with `w[0]=..; w[1]=..` or a 2-iteration loop matches the base-register
  form in the harness, but J3DGXColorS10 derives from SDK `_GXColorS10` (ShieldD), so this is the same punning.
  `*(GXColorS10*)this = o`: 4x lha/sth.
Not tried: permuter (not allowed). Recommendation: the cast is almost certainly the original; whether to admit it
is a policy decision, not a search problem.

## w11-j3dcast, 2026-11-02: word-copy exception tried, then rejected
tp's u32-cast operator= in J3DTevOrderInfo, J3DTexCoordInfo, J3DIndTexOrderInfo, J3DIndTexCoordScaleInfo and
J3DGXColorS10(const GXColorS10&) took the unit to 431 / 431 (and J3DMaterialFactory to 120 / 120). Review showed
`__memcpy` gives the same ShieldD `__as__` sizes (0xC, 0x14) at -O0 and tp's debug build uses it, so the debug map
doesn't support the cast; the user reverted to the ban (docs/COORDINATOR.md, "Evaluated and rejected").
Headers back to `__memcpy` with tp file:line citations; FIXMEs restored. Unit back to 412 / 431, .data 93.44 %
(four weak base vtables, 0x1BC, UNREFERENCED DUPLICATE from J3DMaterial.o in the trial link; plus
`__vt__21J3DColorBlockLightOff` 99.24 %, a trailing padding word in symbols.txt's size). Report identical to matching.
J3DMaterialFactory can't flip: modifyPatchedCurrentMtx needs the same cast (99.66 %), its FIXME stands.
