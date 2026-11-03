# JSystem/J3DGraphBase/J3DStruct.cpp

New unit (w12-follow). Matching: 4/4 functions, links byte-identical.

- Source from zeldaret/tp's J3DStruct.cpp: the four operator= (J3DTexMtxInfo,
  J3DIndTexMtxInfo, J3DFogInfo, J3DNBTScaleInfo). tp's setEffectMtx and the
  operator== are dead-stripped in FSA and omitted.
- gekko_ps_copy3/6/16 are inline asm. mkdd's JMath.h (libs/sdk/include) had only
  copy12; the three were copied from tp's JMath.h (licensed) into it.
- J3DTextureSRTInfo needs tp's inline asm operator= (psq_l/psq_st, a 4-byte
  `*(u32*)&mRotation` copy, psq_l/psq_st): FSA shows exactly that lwz/stw at 0x18.
  It now lives in a new libs/sdk/include/JSystem/J3D/J3DStruct.h (tp's header
  name), included from J3DTypes.h, which drops its own definition.

SHARED-EDITS: libs/sdk/include/JSystem/JMath/JMath.h, J3DTypes.h, new J3DStruct.h,
configure.py Object line.
