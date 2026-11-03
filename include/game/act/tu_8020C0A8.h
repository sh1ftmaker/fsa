#ifndef GAME_ACT_TU_8020C0A8_H
#define GAME_ACT_TU_8020C0A8_H

// Code: game/tu_8020C0A8.cpp.

#include "game/EnemyBase.h"

// Named after its vtable, 0x804A5F40.
class cls_804A5F40 : public EnemyBase_804A4808 {
public:
    cls_804A5F40() {}
    /* 0x08 */ virtual ~cls_804A5F40();
    /* 0x0C */ virtual void init();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x88 */ virtual void vfunc_0x88();
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);
    /* 0x98 */ virtual void vfunc_0x98();
    /* 0x2A8 */ u8 field_0x2a8[0x8]; // not yet known
}; // Size: 0x2B0

#endif
