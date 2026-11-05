#ifndef GAME_ACT_TU_80212E3C_H
#define GAME_ACT_TU_80212E3C_H

// Code: game/tu_80212E3C.cpp.

#include "game/EnemyBase.h"

// Named after its vtable, 0x804A6340.
class cls_804A6340 : public EnemyBase_804A4808 {
public:
    cls_804A6340();
    /* 0x08 */ virtual ~cls_804A6340();
    /* 0x0C */ virtual void init();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x44 */ virtual void vfunc_0x44();
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);
    /* 0x94 */ virtual void vfunc_0x94();

    // Placeholder names (see docs/PLAN.md). fn_80212F14 is not virtual: it is
    // only in an extabindex entry. fn_80213774 takes a mode argument (0 or 1).
    void fn_80212F14();
    // 0x80212ECC: fn_80213774(0) when mFlags has 0x4000, else fn_80213774(2)
    // in mState 2. act_quicksand (fn_80210D90) calls it, in its state 1, on the
    // actor its serial names.
    void fn_80212ECC();
    // Spawn setup: sinks mPos.y by 1, copies mPos to mSpawnPos, sets flags
    // 0x2200 and mLife 1000, sets field_0x54 to 1.4f and scales mBox2 by it.
    void fn_80212F5C();
    void fn_80213774(int mode);
}; // Size: 0x2A8

#endif
