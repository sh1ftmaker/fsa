# JSystem/J3DGraphLoader/J3DShapeFactory.cpp

Baseline (progress.py first line at start): total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: total: 4785 / 11561 functions, 863900 / 4442088 bytes of code (19.45 %)

Status: **Matching**. 14/14 functions (was 13/14), .text 100 %; `link_check.py --unit`
SHA-1 OK; gate ok. Full honest report diffed against the baseline: only J3DShapeFactory
(.text, create) and J3DShape (.data 0 -> 80 %) change, both up.

## Changes
- create: the target copies mMin/mMax with three lfs/stfs each, i.e. a `Vec` struct copy;
  `JGeometry::TVec3f`'s assignment gives psq_l/psq_st. tww and tp declare
  `Vec mMin, mMax` in both J3DShapeInitData and J3DShape. Header fix:
  - J3DShape.h: J3DShapeInitData::mMin/mMax and J3DShape::mMin/mMax -> `Vec`;
    J3DShape::getMin/getMax return `Vec*`.
  - J3DShapeFactory.h: getMin/getMax(int) return `Vec&`.
  - J3DShape.cpp initialize: tww/tp's `mMin.x = 0.0f; ...` (TVec3f::set no longer exists on
    Vec); initialize stays 100 %.
- symbols.txt: lbl_8049A428 -> __vt__8J3DShape (create stores it; same edit is on the
  J3DShape branch, identical line).

## SHARED-EDITS
- libs/sdk/include/JSystem/J3D/J3DShape.h, J3DShapeFactory.h (header layout types, above).
- libs/jsystem/JSystem/J3DGraphBase/J3DShape.cpp initialize (J3DShape unit, also mine this
  wave on unit/JSystem__J3DGraphBase__J3DShape; the hunks don't overlap).
- symbols.txt __vt__8J3DShape (identical to the J3DShape branch's line).
