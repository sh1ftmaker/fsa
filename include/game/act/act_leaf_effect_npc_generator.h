#ifndef GAME_ACT_ACT_LEAF_EFFECT_NPC_GENERATOR_H
#define GAME_ACT_ACT_LEAF_EFFECT_NPC_GENERATOR_H

// Code: game/act_leaf_effect_npc_generator.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AE288.
class cls_804AE288 : public ActorBase_804A5770 {
public:
    cls_804AE288() {}
    /* 0x08 */ virtual ~cls_804AE288();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
}; // Size: 0x230

#endif
