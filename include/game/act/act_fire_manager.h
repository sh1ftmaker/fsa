#ifndef GAME_ACT_ACT_FIRE_MANAGER_H
#define GAME_ACT_ACT_FIRE_MANAGER_H

// Code: game/act_fire_manager.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AB2B0.
class cls_804AB2B0 : public ActorBase_804A5770 {
public:
    cls_804AB2B0() {}
    /* 0x08 */ virtual ~cls_804AB2B0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x230 */ u8 field_0x230[0x28]; // not yet known
}; // Size: 0x258

#endif
