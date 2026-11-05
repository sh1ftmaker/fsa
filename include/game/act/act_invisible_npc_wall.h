#ifndef GAME_ACT_ACT_INVISIBLE_NPC_WALL_H
#define GAME_ACT_ACT_INVISIBLE_NPC_WALL_H

// Code: game/act_invisible_npc_wall.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AE780.
class InvisibleNPCWallForLink_804AE780 : public ActorBase_804A5770 {
public:
    InvisibleNPCWallForLink_804AE780() {}
    /* 0x08 */ virtual ~InvisibleNPCWallForLink_804AE780();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x230 */ s32 field_0x230; // top 5 bits of field_0x90
}; // Size: 0x234

#endif
