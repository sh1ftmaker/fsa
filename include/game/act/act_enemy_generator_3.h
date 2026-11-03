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
    /* 0x230 */ u8 field_0x230[0x24]; // not yet known
}; // Size: 0x254

#endif
