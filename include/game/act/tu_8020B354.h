#ifndef GAME_ACT_TU_8020B354_H
#define GAME_ACT_TU_8020B354_H

// Code: game/tu_8020B354.cpp.

#include "game/EnemyBase.h"

// Named after its vtable, 0x804A5E98.
class cls_804A5E98 : public EnemyBase_804A4808 {
public:
    cls_804A5E98();
    /* 0x08 */ virtual ~cls_804A5E98();
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
    /* 0xA0 */ virtual void vfunc_0xA0();

    // 0x8020B3E4: like cls_804A5D60::fn_80209CA4 (a hit that landed clears ENEMY_FLAG_NO_HURT).
    void fn_8020B3E4(s32 player, s32 attack, const JGeometry::TVec3f* from, s32 type);
    // 0x8020B434: in state 1, makes the hitbox empty and sets the velocity along field_0x94; state 2.
    void fn_8020B434();
    // 0x8020B500: in state 0, makes the hitbox 16x16 again; state 1.
    void fn_8020B500();

    /* 0x2A8 */ s32 mTargetSerial; // the actor vfunc_0x24 looks up (fn_801F666C); -1 from the constructor
}; // Size: 0x2AC

#endif
