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
    /* 0x230 */ u8 field_0x230[0x68]; // not yet known
}; // Size: 0x298

#endif
