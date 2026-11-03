#ifndef GAME_ACT_TU_80209C14_H
#define GAME_ACT_TU_80209C14_H

// Code: game/tu_80209C14.cpp.

#include "game/EnemyBase.h"

// Named after its vtable, 0x804A5D60.
class cls_804A5D60 : public EnemyBase_804A4808 {
public:
    cls_804A5D60() {}
    /* 0x08 */ virtual ~cls_804A5D60();
    /* 0x0C */ virtual void init();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x54 */ virtual void vfunc_0x54();
    /* 0x58 */ virtual bool vfunc_0x58();
    /* 0x5C */ virtual bool vfunc_0x5C();
    /* 0x60 */ virtual bool vfunc_0x60();
    /* 0x74 */ virtual void vfunc_0x74();
    /* 0x88 */ virtual void vfunc_0x88();
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);

    void fn_80209D44(); // sets mFlags bit 0x2000
    // 0x80209CA4. Applies a hit of `attack` from `from` with strength 0 and
    // `type` by `player`; a hit that landed clears ENEMY_FLAG_NO_HURT.
    void fn_80209CA4(s32 player, s32 attack, const JGeometry::TVec3f* from, s32 type);
    /* 0x2A8 */ s32 mTargetSerial; // the actor vfunc_0x24 looks up (fn_801F666C)
    /* 0x2AC */ u8 field_0x2ac[0x14]; // not yet known
    /* 0x2C0 */ JGeometry::TVec2f field_0x2c0; // vfunc_0x54: mSpawnPos - mPos (x, y)
    /* 0x2C8 */ u8 field_0x2c8[8]; // not yet known
}; // Size: 0x2D0

#endif
