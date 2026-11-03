#ifndef GAME_CLS_801EBA14_H
#define GAME_CLS_801EBA14_H

#include "types.h"

// A 0x28-byte record with no vtable, named after its constructor
// (__ct__12cls_801EBA14Fv, game/tu_801EB00C.cpp), which sets
// the two counters to 1, the floats and the final halfword. It sits at
// cls_801EB8CC + 0xC (VaatiTimeUp_804ACCC0 0x23C), where
// VaatiTimeUp_804ACCC0::init() copies a stack one into place. Fields not yet
// named.
class cls_801EBA14 {
public:
    cls_801EBA14();

    /* 0x00 */ s32 field_0x0;
    /* 0x04 */ s32 field_0x4;
    /* 0x08 */ f32 field_0x8;
    /* 0x0C */ f32 field_0xc;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ f32 field_0x1c;
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ s16 field_0x24;
}; // Size: 0x28

#endif
