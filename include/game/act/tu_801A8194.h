#ifndef GAME_ACT_TU_801A8194_H
#define GAME_ACT_TU_801A8194_H

// Code: game/tu_801A8194.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A2EC0.
class cls_804A2EC0 : public ActorBase_804A5770 {
public:
    cls_804A2EC0() {}
    /* 0x08 */ virtual ~cls_804A2EC0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230; // state: 0, 1, 2
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260; // draw's frame
    /* 0x264 */ s32 field_0x264; // life copy
    /* 0x268 */ u16 field_0x268; // angle
    /* 0x26A */ u8 field_0x26a[2];
    /* 0x26C */ f32 field_0x26c; // draw's scale
    /* 0x270 */ f32 field_0x270; // radius
    /* 0x274 */ f32 field_0x274; // draw's alpha
    /* 0x278 */ f32 field_0x278;
    /* 0x27C */ JGeometry::TVec3f field_0x27c; // target
    /* 0x288 */ JGeometry::TVec3f field_0x288; // the player's last position
    /* 0x294 */ JGeometry::TVec3f field_0x294[40];
}; // Size: 0x474

#endif
