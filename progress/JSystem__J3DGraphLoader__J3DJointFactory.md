# JSystem/J3DGraphLoader/J3DJointFactory.cpp

Baseline (progress.py first line at start): total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %) (no function newly at 100 %)

Status: NonMatching. 2/3 functions; .text 81.66 -> 87.08 %. Full honest report diffed against
the baseline: only J3DJointFactory (create 75.96 -> 83.07 %) and J3DJoint (__ct__8J3DJointFv
45.29 -> 55.98 %, .text 94.40 -> 95.49 %) change, both up.

## Changes
- Header layout (tp/tww types): J3DJoint::mMin/mMax and J3DJointInitData::mMin/mMax are `Vec`
  (target copies them with three lfs/stfs; TVec3f's operator= gives psq). getMin/getMax
  return `Vec*`.

## Resistant
- create__15J3DJointFactoryFi (83.07 %), blocked by codegen: FSA copies J3DTransformInfo
  member-wise as `psq_l/psq_st 0x0; lfs/stfs 0x8; lwz/stw 0xc; lha/sth 0x10; psq_l/psq_st 0x14;
  lfs/stfs 0x1c` (TVec3f scale/translate, word + half rotation). Ours (Vec/S16Vec operator=)
  gives 3x lfs/stfs, 3x lha/sth, 3x lfs/stfs. tp reaches it only with an asm
  J3DTransformInfo::operator= and a `*(s32*)` TVec3<s16> copy (both rejected here). Note that
  J3DJoint's ctor copies j3dDefaultTransformInfo as a plain 8-word block copy, so a single
  J3DTransformInfo::operator= has to give both. FIXME in source.

## SHARED-EDITS
- libs/sdk/include/JSystem/J3D/J3DJoint.h, J3DJointFactory.h (types above).
