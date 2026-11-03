# JSystem/J3DGraphAnimator/J3DJoint.cpp

Baseline (progress.py first line at start): total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: total: 4785 / 11561 functions, 863716 / 4442088 bytes of code (19.44 %)

Status: **Matching**. 9/9 functions (was 8/9); .text/.rodata/.sbss/.sdata2 100 %;
`link_check.py --unit` SHA-1 OK; gate ok. Full honest report diffed against the baseline:
J3DJoint (.text, .rodata, ctor -> 100 %) and J3DJointFactory (create 75.96 -> 83.07 %) change,
nothing goes down.

## Changes
- J3DJoint ctor: tp's `Vec init = {0.0f, 0.0f, 0.0f}; mMin = init;` (the two 12-byte zero
  aggregates are the unit's .rodata 0x80463570/0x8046357C, copied to the stack and then
  member-wise with lfs/stfs).
- Header layout (tp/tww types, same edit as the J3DJointFactory branch, identical lines):
  J3DJoint::mMin/mMax and J3DJointInitData::mMin/mMax -> `Vec`, getMin/getMax -> `Vec*`.
- symbols.txt: lbl_804633E8 -> j3dDefaultTransformInfo (the ctor copies it into
  mTransformInfo; value {1,1,1 | 0,0,0 | 0,0,0}; size 0x20, first .rodata object of
  J3DTransform.cpp exactly as tww's and mkdd's j3dDefaultTransformInfo).

## SHARED-EDITS
- libs/sdk/include/JSystem/J3D/J3DJoint.h, J3DJointFactory.h (same as J3DJointFactory branch).
- symbols.txt j3dDefaultTransformInfo (placeholder in J3DTransform.cpp's .rodata).
