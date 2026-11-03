#ifndef GAME_ACT_ACT_ENEMY_GENERATOR_3_H
#define GAME_ACT_ACT_ENEMY_GENERATOR_3_H

// Code: game/act_enemy_generator_3.cpp.

#include "game/ActorBase.h"

// The GEN3 actor ("Enemy generator 3", docs/names/actors.tsv, med confidence):
// the spawner constructs it with fn_80200834, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AA1D0.
class EnemyGenerator3_804AA1D0 : public ActorBase_804A5770 {
public:
    EnemyGenerator3_804AA1D0() {}
    /* 0x08 */ virtual ~EnemyGenerator3_804AA1D0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 mState;     // 0 wait, 1/2 spawn timers, 4 cooldown
    /* 0x234 */ u32 mTypeIndex; // field_0x90 & 0x7F: index of the spawned actor type
    /* 0x238 */ s32 mInterval;  // field_0x90 bits 7-15: frames between spawns
    /* 0x23C */ s32 mCount;     // field_0x90 bits 16-19: spawns per round
    /* 0x240 */ s32 mMode;      // field_0x90 bits 20-23
    /* 0x244 */ s32 mFlag;      // field_0x90 bit 24
    /* 0x248 */ s32 mRemaining; // spawns left in this round
    /* 0x24C */ s32 mTimer;
    /* 0x250 */ u32 mActorId;   // ActorId of the spawned type
}; // Size: 0x254

#endif
