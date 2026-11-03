#ifndef GAME_ACT_ACT_VAATI_TIME_UP_H
#define GAME_ACT_ACT_VAATI_TIME_UP_H

// Code: game/act_vaati_time_up.cpp.

#include "game/ActorBase.h"
#include "game/cls_801EB8CC.h"

// The TUGF actor ("Vaati (time-up)", docs/names/actors.tsv). Named after its
// vtable, 0x804ACCC0 (set by the constructor at 0x8020009C).
class VaatiTimeUp_804ACCC0 : public ActorBase_804A5770 {
public:
    VaatiTimeUp_804ACCC0() {}
    /* 0x08 */ virtual ~VaatiTimeUp_804ACCC0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ cls_801EB8CC field_0x230;
    /* 0x284 */ u8 field_0x284[0x290 - 0x284]; // not yet known
    /* 0x290 */ f32 field_0x290; // centre of mViewBox, x (init)
    /* 0x294 */ f32 field_0x294; // centre of mViewBox, y, plus 96
    /* 0x298 */ f32 field_0x298; // (f.x - i.x) - 96
    /* 0x29C */ f32 field_0x29c; // (f.y - i.y) - 96
    /* 0x2A0 */ f32 field_0x2a0;
    /* 0x2A4 */ f32 field_0x2a4;
    /* 0x2A8 */ JGeometry::TBox2f mViewBox;   // copy of the view's visible box
    /* 0x2B8 */ s32 field_0x2b8;
    /* 0x2BC */ s32 field_0x2bc;
    /* 0x2C0 */ s32 field_0x2c0;
    /* 0x2C4 */ s32 field_0x2c4;
    /* 0x2C8 */ s32 field_0x2c8; // index into the sprite frame table in draw()
    /* 0x2CC */ s16 field_0x2cc;
    /* 0x2CE */ s16 field_0x2ce;
    /* 0x2D0 */ s16 field_0x2d0;
    /* 0x2D4 */ union { // a colour word, read as bytes or as one u32
                    u32 mWord;
                    GXColor mColor;
                } field_0x2d4; // from fn_8018330C
    /* 0x2D8 */ f32 field_0x2d8;     // colour channels as floats
    /* 0x2DC */ f32 field_0x2dc;
    /* 0x2E0 */ f32 field_0x2e0;
    /* 0x2E4 */ f32 field_0x2e4;
    /* 0x2E8 */ f32 field_0x2e8;
    /* 0x2EC */ f32 field_0x2ec;
    /* 0x2F0 */ f32 field_0x2f0;
    /* 0x2F4 */ f32 field_0x2f4;
    /* 0x2F8 */ u8 field_0x2f8;
};

#endif
