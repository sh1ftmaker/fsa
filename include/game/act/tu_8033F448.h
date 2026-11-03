#ifndef GAME_ACT_TU_8033F448_H
#define GAME_ACT_TU_8033F448_H

// Code: game/tu_8033F448.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AC388.
class cls_804AC388 : public ActorBase_804A5770 {
public:
    cls_804AC388() {}
    /* 0x08 */ virtual ~cls_804AC388();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x10]; // not yet known
}; // Size: 0x240

#endif
