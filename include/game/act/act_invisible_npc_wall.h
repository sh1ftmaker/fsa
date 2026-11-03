#ifndef GAME_ACT_ACT_INVISIBLE_NPC_WALL_H
#define GAME_ACT_ACT_INVISIBLE_NPC_WALL_H

// Code: game/act_invisible_npc_wall.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AE780.
class cls_804AE780 : public ActorBase_804A5770 {
public:
    cls_804AE780() {}
    /* 0x08 */ virtual ~cls_804AE780();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x230 */ u8 field_0x230[0x4]; // not yet known
}; // Size: 0x234

#endif
