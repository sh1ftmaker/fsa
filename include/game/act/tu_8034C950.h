#ifndef GAME_ACT_TU_8034C950_H
#define GAME_ACT_TU_8034C950_H

// Code: game/tu_8034C950.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AC950.
class cls_804AC950 : public ActorBase_804A5770 {
public:
    cls_804AC950() {}
    /* 0x08 */ virtual ~cls_804AC950();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ u8 field_0x230[0x15C]; // not yet known
}; // Size: 0x38C

#endif
