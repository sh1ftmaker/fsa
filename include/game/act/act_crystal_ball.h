#ifndef GAME_ACT_ACT_CRYSTAL_BALL_H
#define GAME_ACT_ACT_CRYSTAL_BALL_H

// Code: game/act_crystal_ball.cpp.

#include "game/ActorBase.h"

// The CRTL crystal ball (docs/names/actors.tsv). Named after its vtable,
// 0x804AB308; the destructor (0x80323708) releases its two emitters.
class CrystalBall_804AB308 : public ActorBase_804A5770 {
public:
    /* 0x08 */ virtual ~CrystalBall_804AB308();

    // 0x230..0x2C3: not identified yet (0x278 is read by vfunc_0x40).
    /* 0x230 */ u8 field_0x230[0x94];
    /* 0x2C4 */ JPABaseEmitter* field_0x2c4; // released by the destructor
    /* 0x2C8 */ JPABaseEmitter* field_0x2c8; // released by the destructor
};

#endif
