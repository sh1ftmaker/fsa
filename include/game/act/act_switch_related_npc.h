#ifndef GAME_ACT_ACT_SWITCH_RELATED_NPC_H
#define GAME_ACT_ACT_SWITCH_RELATED_NPC_H

// Code: game/act_switch_related_npc.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A8C28.
class cls_804A8C28 : public ActorBase_804A5770 {
public:
    cls_804A8C28() {}
    /* 0x08 */ virtual ~cls_804A8C28();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x38]; // not yet known
}; // Size: 0x268

#endif
