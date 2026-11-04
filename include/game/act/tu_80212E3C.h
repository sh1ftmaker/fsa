#ifndef GAME_ACT_TU_80212E3C_H
#define GAME_ACT_TU_80212E3C_H

// Code: game/tu_80212E3C.cpp.

#include "game/EnemyBase.h"

// Named after its vtable, 0x804A6340.
class cls_804A6340 : public EnemyBase_804A4808 {
public:
    cls_804A6340();
    /* 0x08 */ virtual ~cls_804A6340();
    /* 0x0C */ virtual void init();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x44 */ virtual void vfunc_0x44();
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);
    /* 0x94 */ virtual void vfunc_0x94();
}; // Size: 0x2A8

#endif
