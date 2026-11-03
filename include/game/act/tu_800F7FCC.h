#ifndef GAME_ACT_TU_800F7FCC_H
#define GAME_ACT_TU_800F7FCC_H

// Code: game/tu_800F7FCC.cpp.

#include "game/ActorBase.h"

// A ring that grows from mPos and hits each player once (no vtable;
// named after its lowest-address function). cls_8049C718 has one at 0x290,
// cls_804A6A00 (game/tu_80221B44.cpp) at 0x2BC.
struct cls_800F802C {
    // Grows the ring (by mTimer, 40 or 20 frames by mType) and hits the
    // active players of layer 0 within mRadius (fn_80238548, arg 0xB).
    void fn_800F802C();

    /* 0x00 */ JGeometry::TVec3f mPos;
    /* 0x0C */ u8 mActive;
    /* 0x0D */ u8 mType;
    /* 0x10 */ f32 mRadius;
    /* 0x14 */ u16 mTimer;
    /* 0x16 */ u8 mHitMask; // bit i: player i was hit
}; // Size: 0x18

// 0x800F8210 (game/tu_800F7FCC.cpp, not written yet): starts the ring at pos
// on `layer`; type 0-2 as in fn_800F802C. Called with 2 by cls_804A6AA8
// (game/tu_80221B44.cpp).
extern "C" void fn_800F8210(cls_800F802C* ring, s32 layer, JGeometry::TVec3f pos, u8 type);

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
