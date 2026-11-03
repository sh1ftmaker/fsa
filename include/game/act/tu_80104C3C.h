#ifndef GAME_ACT_TU_80104C3C_H
#define GAME_ACT_TU_80104C3C_H

// Code: game/tu_80104C3C.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x8049CB58.
class cls_8049CB58 : public ActorBase_804A5770 {
public:
    cls_8049CB58() {}
    /* 0x08 */ virtual ~cls_8049CB58();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x20]; // not yet known
}; // Size: 0x250

#endif
