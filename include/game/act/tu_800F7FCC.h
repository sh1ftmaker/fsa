#ifndef GAME_ACT_TU_800F7FCC_H
#define GAME_ACT_TU_800F7FCC_H

// Code: game/tu_800F7FCC.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x8049C678.
class cls_8049C678 : public ActorBase_804A5770 {
public:
    cls_8049C678() {}
    /* 0x08 */ virtual ~cls_8049C678();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x40 */ virtual void vfunc_0x40();
    /* 0x230 */ u8 field_0x230[0x20]; // not yet known
}; // Size: 0x250

// Named after its vtable, 0x8049C718.
class cls_8049C718 : public ActorBase_804A5770 {
public:
    cls_8049C718() {}
    /* 0x08 */ virtual ~cls_8049C718();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x44 */ virtual void vfunc_0x44();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ u8 field_0x230[0x88]; // not yet known
}; // Size: 0x2B8

#endif
