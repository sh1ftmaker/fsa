#ifndef GAME_ACT_TU_80333958_H
#define GAME_ACT_TU_80333958_H

// Code: game/tu_80333958.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AB968.
class cls_804AB968 : public ActorBase_804A5770 {
public:
    cls_804AB968() {}
    /* 0x08 */ virtual ~cls_804AB968();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ u8 field_0x230[0x18]; // not yet known
}; // Size: 0x248

#endif
