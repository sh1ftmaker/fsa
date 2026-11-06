#ifndef GAME_ACT_ACT_ROOM_TRANSFER_LIFT_H
#define GAME_ACT_ACT_ROOM_TRANSFER_LIFT_H

// Code: game/act_room_transfer_lift.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x8049CC40.
class cls_8049CC40 : public ActorBase_804A5770 {
public:
    cls_8049CC40();
    /* 0x08 */ virtual ~cls_8049CC40();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    inline bool isInside(s32 player) const; // on this actor's layer and inside mBox placed at mPos + (8, 8) (inlined in update)
    /* 0x230 */ void* field_0x230;
    /* 0x234 */ void* field_0x234;
    /* 0x238 */ SpriteParams_801F5AE0 mSpriteParams;
    /* 0x274 */ JGeometry::TVec3f field_0x274;
    /* 0x280 */ f32 field_0x280;
    /* 0x284 */ f32 field_0x284;
    /* 0x288 */ f32 field_0x288;
    /* 0x28c */ f32 field_0x28c;
    /* 0x290 */ s32 field_0x290;
    /* 0x294 */ s32 field_0x294;
    /* 0x298 */ s32 field_0x298;
    /* 0x29c */ s32 field_0x29c;
    /* 0x2a0 */ s32 field_0x2a0;
    /* 0x2a4 */ s32 field_0x2a4;
}; // Size: 0x2A8

#endif
