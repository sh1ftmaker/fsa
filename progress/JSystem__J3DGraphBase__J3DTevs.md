# JSystem/J3DGraphBase/J3DTevs.cpp

Baseline (progress.py first line at start): total: 4501 / 11561 functions, 808092 / 4442088 bytes of code (18.19 %)
After: total: 4502 / 11561 functions, 808444 / 4442088 bytes of code (18.20 %)

Status: **Matching**. 26/26 functions (was 25/26), .text/.rodata/.data/.sdata2 100 %.
`link_check.py --unit JSystem/J3DGraphBase/J3DTevs.cpp`: links byte-identical; `gate: ok` with the flag flipped.

## What made it match
- makeTexCoordTable (99.89 %): its local `const u8 texMtx[11]` is the anonymous `@978` in .rodata;
  symbols.txt had dtk's placeholder `lbl_8046354C` (0xC with the alignment pad).
  Renamed to `@978` size 0xB scope:local (same convention as the 11k other `@NNN` locals).
- .rodata (96.77 %): `j3dDefaultTexCoordInfo` entries are `GX_TG_MTX2x4` (1), as in tp and tww
  (`GX_MTX2x4`); pikmin2 had `GX_TG_MTX3x4` (0).

## Checked, not changed
- `j3dDefaultIndTevStageInfo` / `j3dDefaultTevSwapMode` keep pikmin2's `__declspec(section ...)`
  (non-const in .rodata/.sdata2). Making both `const` (tp/tww) keeps this unit at 100 % but costs
  three ctors elsewhere: J3DMaterial `__ct__14J3DIndTevStageFv` 100 -> 40.8 %,
  `__ct__11J3DTevStageFv` 100 -> 88.1 %, J3DMaterialFactory `__ct__11J3DTevStageFRC15J3DTevStageInfo`
  100 -> 87.1 % (const lets MWCC schedule the loads differently). Fixing that needs tp's
  J3DTevStage/J3DIndTevStage ctors in the shared J3DTypes.h: a separate task (SHARED-EDIT candidate).

## SHARED-EDITS
None applied. Candidate: const `j3dDefaultIndTevStageInfo` / `j3dDefaultTevSwapMode` (J3DTypes.h) together
with tp's ctor forms, so the two `__declspec(section)` lines can go.
