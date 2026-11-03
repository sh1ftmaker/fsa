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
    /* 0x230 */ u8 field_0x230[0x244]; // not yet known
}; // Size: 0x474

#endif
