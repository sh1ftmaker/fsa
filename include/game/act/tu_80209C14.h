#ifndef GAME_ACT_TU_80209C14_H
#define GAME_ACT_TU_80209C14_H

// Code: game/tu_80209C14.cpp.

#include "game/EnemyBase.h"

// Named after its vtable, 0x804A5D60.
class cls_804A5D60 : public EnemyBase_804A4808 {
public:
    cls_804A5D60() {}
    /* 0x08 */ virtual ~cls_804A5D60();
    /* 0x0C */ virtual void init();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x54 */ virtual void vfunc_0x54();
    /* 0x58 */ virtual bool vfunc_0x58();
    /* 0x5C */ virtual bool vfunc_0x5C();
    /* 0x60 */ virtual bool vfunc_0x60();
    /* 0x74 */ virtual void vfunc_0x74();
    /* 0x88 */ virtual void vfunc_0x88();
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);

    void fn_80209D44(); // sets mFlags bit 0x2000
    /* 0x2A8 */ u8 field_0x2a8[0x28]; // not yet known
}; // Size: 0x2D0

#endif
