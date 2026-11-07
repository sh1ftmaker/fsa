#ifndef GAME_ACT_TU_802E9428_H
#define GAME_ACT_TU_802E9428_H

// Code: game/tu_802E9428.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AA0FC.
class UnknownENDM_804AA0FC : public ActorBase_804A5770 {
public:
    UnknownENDM_804AA0FC() {}
    /* 0x08 */ virtual ~UnknownENDM_804AA0FC();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    bool fn_802E948C(f32 target, s32 frames);
    /* 0x230 */ u8 field_0x230;
    /* 0x231 */ u8 field_0x231;
    /* 0x232 */ u8 field_0x232[2];
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ u8 field_0x238[4];
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ u8 field_0x244[0x2C];
    /* 0x270 */ f32 field_0x270; // value being faded
    /* 0x274 */ f32 field_0x274; // its previous target
    /* 0x278 */ f32 field_0x278; // step per frame
    /* 0x27C */ u8 field_0x27c[4];
    /* 0x280 */ s32 field_0x280; // level within the stage (GameView field_0x8)
    /* 0x284 */ s32 field_0x284; // stage (GameView field_0x4)
    /* 0x288 */ u8 field_0x288[5];
    /* 0x28D */ u8 field_0x28d;
    /* 0x28E */ u8 field_0x28e[6];
    /* 0x294 */ u8 field_0x294;
    /* 0x295 */ u8 field_0x295;
    /* 0x296 */ u8 field_0x296[2];
    /* 0x298 */ u8 field_0x298;
    /* 0x299 */ u8 field_0x299;
    /* 0x29A */ u8 field_0x29a;
    /* 0x29B */ u8 field_0x29b;
}; // Size: 0x29C

#endif
