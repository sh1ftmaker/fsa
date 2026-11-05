#ifndef GAME_ACT_TU_80343A70_H
#define GAME_ACT_TU_80343A70_H

// Code: game/tu_80343A70.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AC600.
class cls_804AC600 : public ActorBase_804A5770 {
public:
    cls_804AC600() {}
    /* 0x08 */ virtual ~cls_804AC600();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x230 */ u8 field_0x230[0x74];
    /* 0x2A4 */ JGeometry::TVec3f field_0x2a4[7];
    /* 0x2F8 */ JGeometry::TVec3f field_0x2f8[7];
    /* 0x34C */ JGeometry::TVec3f field_0x34c[7];
}; // Size: 0x3A0

#endif
