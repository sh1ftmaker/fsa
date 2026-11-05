#ifndef GAME_ACT_TU_802ED538_H
#define GAME_ACT_TU_802ED538_H

// Code: game/tu_802ED538.cpp.

#include "game/ActorBase.h"
#include "game/act/tu_801F9484_elems.h"

// Named after its vtable, 0x804AA350.
class cls_804AA350 : public ActorBase_804A5770 {
public:
    cls_804AA350() {}
    /* 0x08 */ virtual ~cls_804AA350();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ JPABaseEmitter* mEmitter230; // released by the destructor
    /* 0x234 */ JPABaseEmitter* mEmitter234; // released by the destructor
    /* 0x238 */ JPABaseEmitter* mEmitter238; // released by the destructor
    /* 0x23C */ JPABaseEmitter* mEmitter23c; // released by the destructor
    /* 0x240 */ JPABaseEmitter* mEmitter240; // released by the destructor
    /* 0x244 */ JPABaseEmitter* mEmitter244; // released by the destructor
    /* 0x248 */ JPABaseEmitter* mEmitter248; // released by the destructor
    /* 0x24C */ u8 field_0x24c[0x30];
    /* 0x27C */ Elem_80200544 field_0x27c[4];
    /* 0x2CC */ u8 field_0x2cc[0x4];
}; // Size: 0x2D0

#endif
