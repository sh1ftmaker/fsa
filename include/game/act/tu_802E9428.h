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
    void fn_802E94F4();
    bool fn_802E9884(const JGeometry::TVec2f* pos);
    /* 0x230 */ u8 field_0x230;
    /* 0x231 */ u8 field_0x231;
    /* 0x232 */ u8 field_0x232[2];
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ u8 field_0x238[4];
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ ActorBase_804A5770* field_0x240; // the actor the camera follows (its field_0x3c is read as a vector)
    /* 0x244 */ JGeometry::TVec3f field_0x244; // target focus
    /* 0x250 */ f32 field_0x250; // current focus x
    /* 0x254 */ f32 field_0x254; // current focus y
    /* 0x258 */ f32 field_0x258;
    /* 0x25C */ f32 field_0x25c;
    /* 0x260 */ f32 field_0x260; // focus limits: min x, min y, max x, max y
    /* 0x264 */ f32 field_0x264;
    /* 0x268 */ f32 field_0x268;
    /* 0x26C */ f32 field_0x26c;
    /* 0x270 */ f32 field_0x270; // value being faded
    /* 0x274 */ f32 field_0x274; // its previous target
    /* 0x278 */ f32 field_0x278; // step per frame
    /* 0x27C */ s32 field_0x27c;
    /* 0x280 */ s32 field_0x280; // level within the stage (GameView field_0x8)
    /* 0x284 */ s32 field_0x284; // stage (GameView field_0x4)
    /* 0x288 */ u16 field_0x288;
    /* 0x28A */ u16 field_0x28a;
    /* 0x28C */ u8 field_0x28c;
    /* 0x28D */ u8 field_0x28d;
    /* 0x28E */ u8 field_0x28e[2];
    /* 0x290 */ u8* field_0x290; // the walk direction byte (0-3, 0xFF ends)
    /* 0x294 */ u8 field_0x294;
    /* 0x295 */ u8 field_0x295;
    /* 0x296 */ u8 field_0x296;
    /* 0x297 */ u8 field_0x297;
    /* 0x298 */ u8 field_0x298;
    /* 0x299 */ u8 field_0x299;
    /* 0x29A */ u8 field_0x29a;
    /* 0x29B */ u8 field_0x29b;
}; // Size: 0x29C

#endif
