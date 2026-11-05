#ifndef GAME_ACT_TU_8036FB60_H
#define GAME_ACT_TU_8036FB60_H

// Code: game/tu_8036FB60.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AE4B0.
class cls_804AE4B0 : public ActorBase_804A5770 {
public:
    cls_804AE4B0() {}
    /* 0x08 */ virtual ~cls_804AE4B0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x34];
    /* 0x264 */ JGeometry::TVec3f field_0x264[4];
    /* 0x294 */ JGeometry::TVec3f field_0x294[4];
    /* 0x2C4 */ JGeometry::TVec3f field_0x2c4[4];
    /* 0x2F4 */ JGeometry::TVec3f field_0x2f4[4];
    /* 0x324 */ u8 field_0x324[0x9C];
}; // Size: 0x3C0

#endif
