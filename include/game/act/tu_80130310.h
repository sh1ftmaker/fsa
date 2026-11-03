#ifndef GAME_ACT_TU_80130310_H
#define GAME_ACT_TU_80130310_H

// Code: game/tu_80130310.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x8049EA88.
class cls_8049EA88 : public ActorBase_804A5770 {
public:
    cls_8049EA88() {}
    /* 0x08 */ virtual ~cls_8049EA88();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    /* 0x230 */ u8 field_0x230[0x20]; // not yet known
}; // Size: 0x250

#endif
