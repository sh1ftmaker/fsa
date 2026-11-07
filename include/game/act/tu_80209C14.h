#ifndef GAME_ACT_TU_80209C14_H
#define GAME_ACT_TU_80209C14_H

// Code: game/tu_80209C14.cpp.

#include "game/EnemyBase.h"

// Named after its vtable, 0x804A5D60.
class cls_804A5D60 : public EnemyBase_804A4808 {
public:
    cls_804A5D60();
    /* 0x08 */ virtual ~cls_804A5D60();
    /* 0x0C */ virtual void init();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x54 */ virtual void vfunc_0x54();
    /* 0x58 */ virtual bool vfunc_0x58();
    /* 0x5C */ virtual u8 vfunc_0x5C();
    /* 0x60 */ virtual u8 vfunc_0x60();
    /* 0x74 */ virtual void vfunc_0x74();
    /* 0x88 */ virtual void vfunc_0x88();
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);

    // 0x8020AA78 (plain symbol name): enters `state`.
    void fn_8020AA78(s32 state);
    void fn_80209D44(); // sets mFlags bit 0x2000
    // 0x80209D20: unless mFlags bit 0x2000 is set, sets bit 0x4000 and stores
    // `dir` at 0x2AC and `steps` at 0x2B0.
    void fn_80209D20(s32 dir, u8 steps);
    // 0x80209CF4: unless mFlags bit 0x2000 is set, sets bit 0x8000 and copies
    // *target to 0x2C8.
    void fn_80209CF4(const JGeometry::TVec2f* target);
    // 0x80209CA4. Applies a hit of `attack` from `from` with strength 0 and
    // `type` by `player`; a hit that landed clears ENEMY_FLAG_NO_HURT.
    void fn_80209CA4(s32 player, s32 attack, const JGeometry::TVec3f* from, s32 type);
    /* 0x2A8 */ s32 mTargetSerial; // the actor vfunc_0x24 looks up (findActorBySerial_801F666C)
    /* 0x2AC */ s32 field_0x2ac; // fn_80209D20's dir (the constructor clears it)
    /* 0x2B0 */ u8 field_0x2b0; // fn_80209D20's steps (the constructor sets 1)
    /* 0x2B1 */ u8 field_0x2b1;
    /* 0x2B4 */ f32 field_0x2b4;
    /* 0x2B8 */ f32 field_0x2b8;
    /* 0x2BC */ f32 field_0x2bc;
    /* 0x2C0 */ JGeometry::TVec2f field_0x2c0; // vfunc_0x54: mSpawnPos - mPos (x, y)
    /* 0x2C8 */ JGeometry::TVec2f field_0x2c8; // fn_80209CF4's target
}; // Size: 0x2D0

#endif
