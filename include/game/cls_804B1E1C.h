#ifndef GAME_CLS_804B1E1C_H
#define GAME_CLS_804B1E1C_H

// A rupee-digit sprite pool: vtable 0x804B1E1C (only the destructor), the
// singleton is stored in lbl_8053AF50. It holds one J2DPicture per digit image
// "kk_rupyNum_00.bti".."kk_rupyNum_09.bti" plus "rupy_num_exclamation.bti"
// (main2D.arc, GameState_804B0FE0 field_0x20) and 128 queued sprites that
// fn_804556A4 draws through the digit's picture.
// Code: game/tu_8045560C.cpp; tu_8043A0E8's object keeps it at 0x2BC and
// calls fn_8045560C on it (act_score also uses lbl_8053AF50).

#include "types.h"
#include "dolphin/mtx.h"
#include "JSystem/JUtility/TColor.h"

class J2DPicture;

class cls_804B1E1C {
public:
    cls_804B1E1C();
    virtual ~cls_804B1E1C();

    void fn_8045560C();
    void fn_804556A4();
    void fn_80455770(f32 scale, u8 digit, const Vec* pos, JUtility::TColor color);

    // One queued sprite (constructor fn_80455A74 sets the colour to -1).
    struct Sprite {
        Sprite() : color(0xFFFFFFFF) {}

        /* 0x00 */ f32 x;
        /* 0x04 */ f32 y;
        /* 0x08 */ f32 size;
        /* 0x0C */ JUtility::TColor color;
        /* 0x10 */ u8 digit;
        /* 0x11 */ u8 active;
    }; // Size: 0x14

    /* 0x00 */ // vtable
    /* 0x04 */ J2DPicture* mDigits[11];
    /* 0x30 */ Sprite mSprites[128];
}; // Size: 0xA30

extern cls_804B1E1C* lbl_8053AF50;

#endif
