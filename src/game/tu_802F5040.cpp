#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Map.h"

void cls_804AA858::fn_802FF548() {
    fn_802FF798();
}

void cls_804AA858::fn_802FF3B0() {
    if (field_0xe451 != 0 && lbl_8053AAF8->field_0x4 != NULL) {
        fn_802FA3FC();
    }
}

// Element constructor of cls_804AA858's constructor (fn_802FFDEC): the
// 0x28-byte array at this + 0xBF9C, 0x10 elements. Its element class is
// not identified yet, and the constructor has no body.
extern "C" void fn_803003A8() {}

// Element constructor of the 0x10-element, 4-byte array at this + 0x1D0 of
// cls_804AA858's constructor (fn_802FFDEC). The element type is not identified
// yet and its constructor has no body.
extern "C" void fn_803004C0() {}

// Element of the 0x20-element, 0x88-byte array at this + 0xD324 of
// cls_804AA858's constructor (fn_802FFDEC). Its constructor is the only code
// seen for the type, so the class is named after it.
class cls_8030029C {
public:
    cls_8030029C();

    /* 0x00 */ u8 field_0x0[0x1C];
    /* 0x1C */ u8 field_0x1c[0x10];
    /* 0x2C */ s32 field_0x2c;
    /* 0x30 */ s32 field_0x30;
    /* 0x34 */ s32 field_0x34;
    /* 0x38 */ u8 field_0x38[0x44 - 0x38];
    /* 0x44 */ s32 field_0x44;
    /* 0x48 */ u8 field_0x48[0x4C - 0x48];
    /* 0x4C */ s32 field_0x4c;
    /* 0x50 */ s32 field_0x50;
    /* 0x54 */ s32 field_0x54;
}; // Size: 0x88

cls_8030029C::cls_8030029C() {
    // Matching-only: 0x2C is stored twice (-1 here and after the table), as
    // the original does; 0x30 is overwritten below.
    field_0x2c = -1;
    field_0x30 = -1;
    for (u8 i = 0; i < 0x10; i++) {
        field_0x1c[i] = i;
    }
    field_0x2c = -1;
    field_0x30 = 0;
    field_0x34 = 0;
    field_0x38[0] = 0;
    field_0x38[1] = 0;
    field_0x38[2] = 0;
    field_0x38[3] = 1;
    field_0x38[4] = 1;
    field_0x38[5] = 0;
    field_0x38[6] = 0;
    field_0x38[7] = 0;
    field_0x38[8] = 0;
    field_0x38[9] = 0;
    field_0x38[10] = 1;
    field_0x44 = 3;
    field_0x48[0] = 1;
    field_0x48[1] = 0;
    field_0x48[2] = 0;
    field_0x4c = -1;
    field_0x50 = 0;
    field_0x54 = 2;
}

// Element of the 0x100-element, 0x9C-byte array at this + 0x2398 of
// cls_804AA858's constructor (fn_802FFDEC). Its constructor is the only code
// seen for the type, so the class is named after it.
class cls_803003AC {
public:
    cls_803003AC();

    /* 0x00 */ u8 field_0x0[0x28];
    /* 0x28 */ u8 field_0x28[0x10];
    /* 0x38 */ s32 field_0x38;
    /* 0x3C */ s32 field_0x3c;
    /* 0x40 */ s32 field_0x40;
    /* 0x44 */ u8 field_0x44;
    /* 0x45 */ u8 field_0x45;
    /* 0x46 */ u8 field_0x46;
    /* 0x47 */ u8 field_0x47;
    /* 0x48 */ u8 field_0x48;
    /* 0x49 */ u8 field_0x49[0x4E - 0x49];
    /* 0x4E */ u8 field_0x4e;
    /* 0x50 */ s32 field_0x50;
    /* 0x54 */ u8 field_0x54;
    /* 0x55 */ u8 field_0x55[0x58 - 0x55];
    /* 0x58 */ s32 field_0x58;
    /* 0x5C */ s32 field_0x5c;
    /* 0x60 */ s32 field_0x60;
    /* 0x64 */ u8 field_0x64[0x94 - 0x64];
    /* 0x94 */ s32 field_0x94;
    /* 0x98 */ s32 field_0x98;
}; // Size: 0x9C

cls_803003AC::cls_803003AC() {
    // Matching-only: 0x38 is stored twice (-1 here and after the table), and
    // 0x3C is stored twice too, as the original does.
    field_0x38 = -1;
    field_0x3c = -1;
    for (u8 i = 0; i < 0x10; i++) {
        field_0x28[i] = i;
    }
    field_0x38 = -1;
    field_0x3c = 0;
    field_0x40 = 0;
    field_0x44 = 0;
    field_0x45 = 0;
    field_0x46 = 0;
    field_0x47 = 1;
    field_0x48 = 1;
    field_0x49[0] = 0;
    field_0x49[1] = 0;
    field_0x49[2] = 0;
    field_0x49[3] = 0;
    field_0x49[4] = 0;
    field_0x4e = 1;
    field_0x50 = 3;
    field_0x54 = 1;
    field_0x55[0] = 0;
    field_0x55[1] = 0;
    field_0x58 = -1;
    field_0x5c = 0;
    field_0x60 = 2;
    field_0x94 = -1;
    field_0x98 = -1;
}

// Layers 0-3 and 4-7 are separate branches in the original, with the rest
// after them; all three store the same way.
void cls_804AA858::fn_802F9974(s32 layer, s32 index, u16 value) {
    if (layer >= 0 && layer < 4) {
        mLayerGrid[layer][index] = value;
    } else if (layer >= 4 && layer < 8) {
        mLayerGrid[layer][index] = value;
    } else {
        mLayerGrid[layer][index] = value;
    }
}

// Layers 0-3 and 4-7 are separate branches in the original, with the rest
// after them; all three load the same way into one result.
u16 cls_804AA858::fn_802F9908(s32 layer, s32 index) {
    s32 tile;
    if (layer >= 0 && layer < 4) {
        tile = mLayerGrid[layer][index];
    } else if (layer >= 4 && layer < 8) {
        tile = mLayerGrid[layer][index];
    } else {
        tile = mLayerGrid[layer][index];
    }
    return tile;
}
