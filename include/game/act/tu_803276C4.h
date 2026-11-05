#ifndef GAME_ACT_TU_803276C4_H
#define GAME_ACT_TU_803276C4_H

// Code: game/tu_803276C4.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AB528.
class cls_804AB528 : public ActorBase_804A5770 {
public:
    cls_804AB528() {}
    /* 0x08 */ virtual ~cls_804AB528();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x3C];
    /* 0x26C */ JGeometry::TVec3f field_0x26c[4];
}; // Size: 0x29C

#endif
