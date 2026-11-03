#ifndef GAME_ACT_TU_8034A6B0_H
#define GAME_ACT_TU_8034A6B0_H

// Code: game/tu_8034A6B0.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AC868.
class cls_804AC868 : public ActorBase_804A5770 {
public:
    cls_804AC868() {}
    /* 0x08 */ virtual ~cls_804AC868();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x8]; // not yet known
}; // Size: 0x238

#endif
