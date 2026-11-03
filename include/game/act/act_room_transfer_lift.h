#ifndef GAME_ACT_ACT_ROOM_TRANSFER_LIFT_H
#define GAME_ACT_ACT_ROOM_TRANSFER_LIFT_H

// Code: game/act_room_transfer_lift.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x8049CC40.
class cls_8049CC40 : public ActorBase_804A5770 {
public:
    cls_8049CC40() {}
    /* 0x08 */ virtual ~cls_8049CC40();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x78]; // not yet known
}; // Size: 0x2A8

#endif
