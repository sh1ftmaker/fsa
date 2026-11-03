#ifndef GAME_ACT_TU_801BD8C0_H
#define GAME_ACT_TU_801BD8C0_H

// Code: game/tu_801BD8C0.cpp. An EnemyBase_804A4808 with two effects (ids
// 0x19E and 0x19C) that follow it, named after its vtable (0x804A4038);
// the spawner (0x801FC538, actor FBOB) allocates 0x2B0 bytes and calls the
// constructor 0x801BDBF0. act_fire_bar looks one up in the actor manager
// and calls fn_801BD8C0 on it (0x801BD2EC, 0x801BD5EC) with a position and
// the 16-bit angle at its own 0x2FC.

#include "types.h"
#include "game/EnemyBase.h"

class JPABaseEmitter;

class cls_804A4038 : public EnemyBase_804A4808 {
public:
    cls_804A4038();
    virtual ~cls_804A4038();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    // 0x801BD8C0, called by act_fire_bar. Moves the actor to *pos and the
    // effects with it; the first effect is turned about y by fn_801CB7CC
    // of `angle`.
    void fn_801BD8C0(const JGeometry::TVec3f* pos, s16 angle);

    /* 0x2A8 */ JPABaseEmitter* mEmitters[2];
}; // Size: 0x2B0

#endif
