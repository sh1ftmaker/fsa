# JSystem/JParticle/JPAMath.cpp

Baseline (progress.py first line at start): total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: total: 4785 / 11561 functions, 863536 / 4442088 bytes of code (19.44 %)

Status: NonMatching, 5/6 functions (w12-follow). The `(Vec*)&vec` alias in JPAGetDirMtx was
a fakematch (review: only defeats CSE of vec.z; tp/tww/Petari have no cast) and is gone;
JPAGetDirMtx now has tp's body at 95.19 % with a FIXME. progress 4805 -> 4804 functions.

## Changes
- splits.txt: JPAMath .text ends at 0x800EF320 (was 0x800EF368); JPAParticle.cpp .text starts
  at 0x800EF320. The function at 0x800EF320 is __dt__19JPAParticleCallBackFv (stores
  __vt__19JPAParticleCallBack, deleting dtor), the first function of JPAParticle.cpp in mkdd
  and pikmin2 (both splits.txt) and in tp; FSA's JPAParticle.cpp source already defines it
  out of line (`JPAParticleCallBack::~JPAParticleCallBack() { }`) as its first function.
  JPAParticle (Matching) stays 100 % and the link SHA-1 is OK (gate ok).
- symbols.txt: fn_800EF320 -> __dt__19JPAParticleCallBackFv; lbl_8053BE10 (placeholder)
  0x8 -> 0x4 (one float 32.0, rest padding).

## Resistant
- JPAGetDirMtx: matches only through the pointer cast. The target loads vec.z a second time for
  m[2][2] (`lfs f0, 0x8(r30)` at 0xBC) instead of reusing the register; with plain `vec.z`
  (tp's body) MWCC CSEs it and every FP register shifts by one (95.19 %). tp's own file needs
  a dummy `floatDummyFunc` for its constant order, so no clean donor form. Tried: tp's body,
  constructor instead of set(). Blocked by codegen. Now tp's body (honest 95.19 %).
- JPAConvertFixToFloat (`return 1.0f;` stub, dead-stripped) is what puts 1.0f first in .sdata2;
  also a pre-existing placement stub, same as tp's floatDummyFunc.

## SHARED-EDITS
- splits.txt JPAParticle.cpp .text start 0x800EF368 -> 0x800EF320 (unit is Matching, still OK).
