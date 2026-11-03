#ifndef GAME_ACT_TU_80166BD0_H
#define GAME_ACT_TU_80166BD0_H

// Code: game/tu_80166BD0.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A0668.
class cls_804A0668 : public ActorBase_804A5770 {
public:
    cls_804A0668() {}
    /* 0x08 */ virtual ~cls_804A0668();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x44 */ virtual void vfunc_0x44();
    /* 0x230 */ u8 field_0x230[0x3C]; // not yet known
    /* 0x26C */ JPABaseEmitter* mEmitter26c; // released by the destructor
}; // Size: 0x270

#endif
