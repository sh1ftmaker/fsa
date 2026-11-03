#ifndef GAME_ACT_TU_80210630_H
#define GAME_ACT_TU_80210630_H

// Code: game/tu_80210630.cpp (not written yet; the members below are in it).
// The shared base of the actor in game/tu_8020FE50.cpp: vtable 0x804A61C8.
// Its constructor fn_80210AB4 calls EnemyBase's, stores this vtable and sets
// 0x2A8; the destructor is inline (tu_8020FE50's stores this vtable, then
// EnemyBase's). Named after its vtable.

#include "game/EnemyBase.h"

class cls_804A61C8 : public EnemyBase_804A4808 {
public:
    // 0x80210AB4; the derived constructor calls it.
    cls_804A61C8();
    virtual ~cls_804A61C8() {}
    // 0x80210898: fn_801CEBD0() (the players overlapping), then 0.
    /* 0x5C */ virtual bool vfunc_0x5C();
    // 0x802108BC: EnemyBase's step, then a box built from field_0x54 /
    // field_0x58 and moved to mPos goes to fn_80239914 and to the layer's
    // collision list (fn_8020D8C0).
    /* 0x9C */ virtual void vfunc_0x9C(u8 mapHit);

    // 0x80210630: draws the sprite (fn_802F745C) for direction field_0x98.
    // arg0 is tested for non-zero; arg1 and arg2 are stored as bytes
    // (0x60, 0x61) and arg3 as a byte at 0x58 of its parameter block
    // (tu_8020FE50's draw passes (int)field_0x2a8 in the state 2).
    void fn_80210630(u8 arg0, u8 arg1, u8 arg2, s32 arg3);
    // 0x80210990: writes the four floats of the box for direction `dir`
    // (0-3) to *out, from field_0x54 / field_0x58.
    void fn_80210990(s32 dir, JGeometry::TBox2f* out);
    // 0x80210A30: the same without the direction offset.
    void fn_80210A30(JGeometry::TBox2f* out);
    // 0x80210A6C: moves field_0x2a8 toward a target (fn_801CD950); returns
    // whether it has arrived.
    bool fn_80210A6C();

    /* 0x2A8 */ f32 field_0x2a8;
}; // Size: 0x2AC

extern "C" {
// Owner: game/tu_80210630.cpp. Called with a direction (0-3, field_0x98) and
// multiplied by field_0x58 / field_0x54 into a position offset.
f32 fn_80210AF8(s32 dir);
f32 fn_80210B34(s32 dir);
}

#endif
