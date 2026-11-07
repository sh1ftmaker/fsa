#ifndef GAME_ACT_TU_8028DD78_H
#define GAME_ACT_TU_8028DD78_H

// Code: game/tu_8028DD78.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A7A68.
class cls_804A7A68 : public ActorBase_804A5770 {
public:
    cls_804A7A68() {}
    /* 0x08 */ virtual ~cls_804A7A68();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ JGeometry::TVec3f field_0x238[4];
    /* 0x268 */ s32 field_0x268;
}; // Size: 0x26C

#endif
