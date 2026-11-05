#ifndef GAME_SPRITEPARAMS_H
#define GAME_SPRITEPARAMS_H

// SpriteParams_801F5AE0, moved out of game/ActorBase.h so that game/Map.h
// (TileMap_804AA858's sprite queues embed it) can use it without ActorBase.h.

#include "types.h"
#include "dolphin/mtx.h"
#include "dolphin/gx/GXStruct.h"
#include "JSystem/JUtility/TColor.h"

// A colour word: tu_802EF76C keeps fn_8018330C's colour in one and splits
// its bytes.
union SpriteParamsColor_801F5AE0 {
    s32 mWord;
    GXColor mColor;
};

// The sprite drawing parameters (no vtable and no out-of-line constructor;
// named after the constructor it is inlined into, ActorBase's at
// 0x801F5AE0). One lives in every actor at 0x120, and draw() overrides
// build others on the stack (act_rat 0x800F3C70: the same stores in the same
// order) to pass to the "SOB " sprite functions (drawSprite_801F06F0, fn_801F02BC,
// fn_801EFD34, fn_801EF830, fn_801EF574), which NULL-check it and read
// 0x13, 0x18, 0x1C and 0x1D (fn_801EF830, 0x801EFA64-0x801EFBB0).
// The colours 0x10 / 0x14 are JUtility::TColor: the constructor stores -1
// to both (TColor's default constructor) before reset() sets them again,
// they are written whole (set) and bytewise, and a copy of the parameters
// copies their four bytes one by one (TileMap_804AA858::fn_802F6298).
struct SpriteParams_801F5AE0 {
    SpriteParams_801F5AE0() {
        reset();
    }
    void reset() {
        for (u8 i = 0; i < 16; i++) {
            field_0x0[i] = i;
        }
        field_0x10.set(0xFFFFFFFF);
        field_0x14.set(0);
        mMtx = NULL;
        field_0x1c = 0;
        field_0x1d = 0;
        field_0x1e = 0;
        field_0x1f = 1;
        field_0x20 = 1;
        field_0x21 = 0;
        field_0x22 = 0;
        field_0x23 = 0;
        field_0x24 = 0;
        field_0x25 = 0;
        field_0x26 = 1;
        field_0x28 = 3;
        field_0x2c = 1;
        field_0x2d = 0;
        field_0x2e = 0;
        field_0x30 = -1;
        field_0x34 = 0;
        field_0x38 = 2;
    }

    /* 0x00 */ u8 field_0x0[16]; // 0, 1, ..., 15
    /* 0x10 */ JUtility::TColor field_0x10; // fn_801EF830 reads a (lbz 0x13)
    /* 0x14 */ JUtility::TColor field_0x14;
    // When set, fn_801EF830 concatenates it with the sprite's scale matrix
    // (PSMTXConcat, 0x801EFBD0) and keeps its translation column; act_rat's
    // draw points it at a stack Mtx when its spawn parameter is nonzero.
    /* 0x18 */ MtxP mMtx;
    /* 0x1C */ s8 field_0x1c; // fn_8023CA88 (s8) in act_sign; -1 in act_blindfold (stb of a shared -1)
    /* 0x1D */ s8 field_0x1d; // -1 in tu_80333958 draw (stb of a shared -1, as 0x1C)
    /* 0x1E */ u8 field_0x1e;
    /* 0x1F */ u8 field_0x1f;
    /* 0x20 */ u8 field_0x20;
    /* 0x21 */ u8 field_0x21;
    /* 0x22 */ u8 field_0x22;
    /* 0x23 */ u8 field_0x23;
    /* 0x24 */ u8 field_0x24;
    /* 0x25 */ u8 field_0x25;
    /* 0x26 */ u8 field_0x26;
    /* 0x28 */ s32 field_0x28;
    /* 0x2C */ u8 field_0x2c;
    /* 0x2D */ u8 field_0x2d;
    /* 0x2E */ u8 field_0x2e;
    /* 0x30 */ s32 field_0x30;
    /* 0x34 */ s32 field_0x34;
    /* 0x38 */ s32 field_0x38;
}; // Size: 0x3C

#endif
