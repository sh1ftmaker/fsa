#ifndef GAME_ACT_TU_8020C0A8_H
#define GAME_ACT_TU_8020C0A8_H

// Code: game/tu_8020C0A8.cpp.

#include "game/EnemyBase.h"

// Named after its vtable, 0x804A5F40.
class cls_804A5F40 : public EnemyBase_804A4808 {
public:
    cls_804A5F40();
    /* 0x08 */ virtual ~cls_804A5F40();
    /* 0x0C */ virtual void init();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x88 */ virtual void vfunc_0x88();
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);
    /* 0x98 */ virtual void vfunc_0x98();
    // 0x8020C6E8: picks the nearest player on this layer and switches between the idle,
    // walking and chasing states (mState 0-2).
    void fn_8020C6E8();

    // SHARED-EDIT: carved from field_0x2a8[0x8] (game/tu_8020C0A8.cpp).
    /* 0x2A8 */ s32 field_0x2a8; // the player being chased, -1 for none
    /* 0x2AC */ u8 field_0x2ac;  // frames left before the chase target may change
    /* 0x2AD */ u8 field_0x2ad;
    /* 0x2AE */ s16 field_0x2ae; // heading
}; // Size: 0x2B0

#endif
