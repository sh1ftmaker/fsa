#ifndef GAME_ACT_TU_80183544_H
#define GAME_ACT_TU_80183544_H

// Code: game/tu_80183544.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A1790.
class cls_804A1790 : public ActorBase_804A5770 {
public:
    cls_804A1790() {}
    /* 0x08 */ virtual ~cls_804A1790();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x18 */ virtual void vfunc_0x18();
    /* 0x20 */ virtual void draw();
    /* 0x44 */ virtual void vfunc_0x44();
    /* 0x230 */ u8 field_0x230[0x40]; // not yet known
    /* 0x270 */ JPABaseEmitter* mEmitter270; // released by the destructor
}; // Size: 0x274

#endif
