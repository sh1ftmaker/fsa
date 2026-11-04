#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Map.h"

// Layers 0-3 and 4-7 have their own grid arrays; any other layer indexes
// the first array as is.
u16 cls_804AA858::fn_802F9908(s32 layer, s32 index) {
    s32 tile;
    if (layer >= 0 && layer < 4) {
        tile = mLayerGrid[layer][index];
    } else if (layer >= 4 && layer < 8) {
        tile = mLayerGrid2[layer - 4][index];
    } else {
        tile = mLayerGrid[layer][index];
    }
    return tile;
}

void cls_804AA858::fn_802F9974(s32 layer, s32 index, u16 value) {
    if (layer >= 0 && layer < 4) {
        mLayerGrid[layer][index] = value;
    } else if (layer >= 4 && layer < 8) {
        mLayerGrid2[layer - 4][index] = value;
    } else {
        mLayerGrid[layer][index] = value;
    }
}

void cls_804AA858::fn_802FF3B0() {
    if (field_0xe451 != 0 && lbl_8053AAF8->field_0x4 != NULL) {
        fn_802FA3FC();
    }
}

void cls_804AA858::fn_802FF548() {
    fn_802FF798();
}

// Element of the 0x20-element, 0x88-byte array at this + 0xD324 of
// cls_804AA858's constructor (fn_802FFDEC). Its constructor is the only code
// seen for the type, so the class is named after it.
class cls_8030029C {
public:
    cls_8030029C();
    // The constructor ends with these stores after setting 0x2C / 0x30 and
    // the table, so 0x2C and 0x30 are written twice, as in the original.
    void reset() {
        field_0x2c = -1;
        field_0x30 = 0;
        field_0x34 = 0;
        field_0x38 = 0;
        field_0x39 = 0;
        field_0x3a = 0;
        field_0x3b = 1;
        field_0x3c = 1;
        field_0x3d = 0;
        field_0x3e = 0;
        field_0x3f = 0;
        field_0x40 = 0;
        field_0x41 = 0;
        field_0x42 = 1;
        field_0x44 = 3;
        field_0x48 = 1;
        field_0x49 = 0;
        field_0x4a = 0;
        field_0x4c = -1;
        field_0x50 = 0;
        field_0x54 = 2;
    }

    /* 0x00 */ u8 field_0x0[0x1C];
    /* 0x1C */ u8 field_0x1c[0x10]; // 0..15
    /* 0x2C */ s32 field_0x2c;
    /* 0x30 */ s32 field_0x30;
    /* 0x34 */ s32 field_0x34;
    /* 0x38 */ u8 field_0x38;
    /* 0x39 */ u8 field_0x39;
    /* 0x3A */ u8 field_0x3a;
    /* 0x3B */ u8 field_0x3b;
    /* 0x3C */ u8 field_0x3c;
    /* 0x3D */ u8 field_0x3d;
    /* 0x3E */ u8 field_0x3e;
    /* 0x3F */ u8 field_0x3f;
    /* 0x40 */ u8 field_0x40;
    /* 0x41 */ u8 field_0x41;
    /* 0x42 */ u8 field_0x42;
    /* 0x44 */ s32 field_0x44;
    /* 0x48 */ u8 field_0x48;
    /* 0x49 */ u8 field_0x49;
    /* 0x4A */ u8 field_0x4a;
    /* 0x4C */ s32 field_0x4c;
    /* 0x50 */ s32 field_0x50;
    /* 0x54 */ s32 field_0x54;
    /* 0x58 */ u8 field_0x58[0x88 - 0x58];
}; // Size: 0x88

cls_8030029C::cls_8030029C() {
    field_0x2c = -1;
    field_0x30 = -1;
    for (u8 i = 0; i < 0x10; i++) {
        field_0x1c[i] = i;
    }
    reset();
}

// Element of the 0x10-element, 0x28-byte array at this + 0xBF9C of
// cls_804AA858's constructor (fn_802FFDEC); named after its empty
// out-of-line constructor, the only code seen for it.
class cls_803003A8 {
public:
    cls_803003A8();

    /* 0x00 */ u8 field_0x0[0x28];
}; // Size: 0x28

cls_803003A8::cls_803003A8() {}

// Element of the 0x100-element, 0x9C-byte array at this + 0x2398 of
// cls_804AA858's constructor (fn_802FFDEC). Its constructor is the only code
// seen for the type, so the class is named after it.
class cls_803003AC {
public:
    cls_803003AC();
    // As cls_8030029C::reset: the constructor's last stores (0x38 and 0x3C
    // are written twice).
    void reset() {
        field_0x38 = -1;
        field_0x3c = 0;
        field_0x40 = 0;
        field_0x44 = 0;
        field_0x45 = 0;
        field_0x46 = 0;
        field_0x47 = 1;
        field_0x48 = 1;
        field_0x49 = 0;
        field_0x4a = 0;
        field_0x4b = 0;
        field_0x4c = 0;
        field_0x4d = 0;
        field_0x4e = 1;
        field_0x50 = 3;
        field_0x54 = 1;
        field_0x55 = 0;
        field_0x56 = 0;
        field_0x58 = -1;
        field_0x5c = 0;
        field_0x60 = 2;
        field_0x94 = -1;
        field_0x98 = -1;
    }

    /* 0x00 */ u8 field_0x0[0x28];
    /* 0x28 */ u8 field_0x28[0x10]; // 0..15
    /* 0x38 */ s32 field_0x38;
    /* 0x3C */ s32 field_0x3c;
    /* 0x40 */ s32 field_0x40;
    /* 0x44 */ u8 field_0x44;
    /* 0x45 */ u8 field_0x45;
    /* 0x46 */ u8 field_0x46;
    /* 0x47 */ u8 field_0x47;
    /* 0x48 */ u8 field_0x48;
    /* 0x49 */ u8 field_0x49;
    /* 0x4A */ u8 field_0x4a;
    /* 0x4B */ u8 field_0x4b;
    /* 0x4C */ u8 field_0x4c;
    /* 0x4D */ u8 field_0x4d;
    /* 0x4E */ u8 field_0x4e;
    /* 0x50 */ s32 field_0x50;
    /* 0x54 */ u8 field_0x54;
    /* 0x55 */ u8 field_0x55;
    /* 0x56 */ u8 field_0x56;
    /* 0x58 */ s32 field_0x58;
    /* 0x5C */ s32 field_0x5c;
    /* 0x60 */ s32 field_0x60;
    /* 0x64 */ u8 field_0x64[0x94 - 0x64];
    /* 0x94 */ s32 field_0x94;
    /* 0x98 */ s32 field_0x98;
}; // Size: 0x9C

cls_803003AC::cls_803003AC() {
    field_0x38 = -1;
    field_0x3c = -1;
    for (u8 i = 0; i < 0x10; i++) {
        field_0x28[i] = i;
    }
    reset();
}

// Element of the 0x10-element, 4-byte array at this + 0x1D0 of
// cls_804AA858's constructor (fn_802FFDEC); named after its empty
// out-of-line constructor, the only code seen for it.
class cls_803004C0 {
public:
    cls_803004C0();

    /* 0x00 */ u8 field_0x0[0x4];
}; // Size: 0x4

cls_803004C0::cls_803004C0() {}
