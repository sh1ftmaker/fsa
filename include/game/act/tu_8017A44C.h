#ifndef GAME_ACT_TU_8017A44C_H
#define GAME_ACT_TU_8017A44C_H

// Code: game/tu_8017A44C.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A1160.
class cls_804A1160 : public ActorBase_804A5770 {
public:
    cls_804A1160() {}
    /* 0x08 */ virtual ~cls_804A1160();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ u8 field_0x230[0x30]; // not yet known
}; // Size: 0x260

#endif
