#ifndef GAME_ACT_TU_8034AA40_H
#define GAME_ACT_TU_8034AA40_H

// Code: game/tu_8034AA40.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AC8C0.
class cls_804AC8C0 : public ActorBase_804A5770 {
public:
    cls_804AC8C0() {}
    /* 0x08 */ virtual ~cls_804AC8C0();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x60];
    /* 0x290 */ JGeometry::TVec3f field_0x290[4];
    /* 0x2C0 */ u8 field_0x2c0[0x4];
}; // Size: 0x2C4

#endif
