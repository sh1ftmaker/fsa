#ifndef GAME_ACT_TU_801F9484_ELEMS_H
#define GAME_ACT_TU_801F9484_ELEMS_H

// Array elements of actor classes the spawner constructs (game/tu_801F9484.cpp): structs whose
// empty constructor (an out-of-line `blr`, run by __construct_array) the constructors call per
// element. Named after that constructor's address; the contents are not known.

#include "types.h"
#include "JSystem/JGeometry.h"

struct Elem_801FFCDC {
    Elem_801FFCDC() {}
    u8 data[0x14];
};

// One per player of cls_804AA350 (tu_802ED538).
struct Elem_80200544 {
    Elem_80200544() {}
    /* 0x00 */ JGeometry::TVec3f mPos;
    /* 0x0C */ f32 mDist;
    /* 0x10 */ s16 mAngle;
    /* 0x12 */ u8 mActive;
    /* 0x13 */ u8 pad_0x13;
};

struct Elem_80200A80 {
    Elem_80200A80() {}
    u8 field_0x0; // 1 after cls_804AC950::init
    u8 field_0x1[3];
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
    s32 field_0x10;
};

struct Elem_80200A84 {
    Elem_80200A84() {}
    s32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
    s32 field_0x10;
    s32 field_0x14; // -1 after cls_804AC950::init
};

// A recorded chaser trail point (cls_804A0738): two (x, y) records and their layers.
struct Elem_80203594 {
    Elem_80203594() {}
    f32 x0;
    f32 y0;
    f32 x1;
    f32 y1;
    s32 layer0;
    s32 layer1;
};

#endif
