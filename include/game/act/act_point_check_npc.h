#ifndef GAME_ACT_ACT_POINT_CHECK_NPC_H
#define GAME_ACT_ACT_POINT_CHECK_NPC_H

// Code: game/act_point_check_npc.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A81C8.
class PointCheckNPC_804A81C8 : public ActorBase_804A5770 {
public:
    PointCheckNPC_804A81C8() {}
    /* 0x08 */ virtual ~PointCheckNPC_804A81C8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x14]; // not yet known
}; // Size: 0x244

#endif
