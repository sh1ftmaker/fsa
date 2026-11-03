#ifndef GAME_ACT_ACT_LEAF_EFFECT_NPC_H
#define GAME_ACT_ACT_LEAF_EFFECT_NPC_H

// Code: game/act_leaf_effect_npc.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AE2E0.
class cls_804AE2E0 : public ActorBase_804A5770 {
public:
    cls_804AE2E0() {}
    /* 0x08 */ virtual ~cls_804AE2E0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x24]; // not yet known
}; // Size: 0x254

#endif
