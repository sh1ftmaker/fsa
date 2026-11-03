#ifndef GAME_ACT_TU_8020B354_H
#define GAME_ACT_TU_8020B354_H

// Code: game/tu_8020B354.cpp.

#include "game/EnemyBase.h"

// Named after its vtable, 0x804A5E98.
class cls_804A5E98 : public EnemyBase_804A4808 {
public:
    cls_804A5E98() {}
    /* 0x08 */ virtual ~cls_804A5E98();
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
    /* 0xA0 */ virtual void vfunc_0xA0();
    /* 0x2A8 */ u8 field_0x2a8[0x4]; // not yet known
}; // Size: 0x2AC

#endif
