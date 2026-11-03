#ifndef GAME_ACT_ACT_ENEMY_SPAWN_AREA_H
#define GAME_ACT_ACT_ENEMY_SPAWN_AREA_H

// Code: game/act_enemy_spawn_area.cpp.

#include "game/ActorBase.h"

// The ENBZ actor ("Enemy spawn area", docs/names/actors.tsv, med confidence):
// the spawner constructs it with fn_802010C4, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A2CB0.
class EnemySpawnArea_804A2CB0 : public ActorBase_804A5770 {
public:
    EnemySpawnArea_804A2CB0() {}
    /* 0x08 */ virtual ~EnemySpawnArea_804A2CB0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 mState; // 0: spawn the group, 1: watch it
    /* 0x234 */ u32 mEnemyId; // actor ID to spawn (0: pick from lbl_804A2C80 at random)
    /* 0x238 */ u32 mEnemyParam; // field_0x90 of the spawned enemy
    /* 0x23C */ s32 mMode; // 0: respawn as soon as one is gone, 1: when all are
    /* 0x240 */ s32 mCount; // enemies in the group (bits 12-15 of field_0x90; 0 -> 3)
    /* 0x244 */ s32 mSerials[15]; // serials of the spawned enemies, -1 = none
    /* 0x280 */ s32 mTimer0; // counts down to 0
    /* 0x284 */ s32 mTimer1; // counts down to 0
    /* 0x288 */ s32 mTimer2; // counts down to 0
    /* 0x28C */ s32 field_0x28c; // only cleared by init
    /* 0x290 */ s32 field_0x290; // only cleared by init
    /* 0x294 */ s32 field_0x294; // only cleared by init
}; // Size: 0x298

#endif
