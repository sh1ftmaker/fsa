#ifndef GAME_ACT_TU_801F9484_ELEMS_H
#define GAME_ACT_TU_801F9484_ELEMS_H

// Array elements of actor classes the spawner constructs (game/tu_801F9484.cpp): structs whose
// empty constructor (an out-of-line `blr`, run by __construct_array) the constructors call per
// element. Named after that constructor's address; the contents are not known.

#include "types.h"

struct Elem_801FFCDC {
    Elem_801FFCDC() {}
    u8 data[0x14];
};

struct Elem_80200544 {
    Elem_80200544() {}
    u8 data[0x14];
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

struct Elem_80203594 {
    Elem_80203594() {}
    u8 data[0x18];
};

#endif
