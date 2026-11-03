#ifndef GAME_ACT_TU_802F3B74_H
#define GAME_ACT_TU_802F3B74_H

// Code: game/tu_802F3B74.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AA6A0.
class cls_804AA6A0 : public ActorBase_804A5770 {
public:
    cls_804AA6A0() {}
    /* 0x08 */ virtual ~cls_804AA6A0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x230 */ u8 field_0x230[0x10]; // not yet known
}; // Size: 0x240

#endif
