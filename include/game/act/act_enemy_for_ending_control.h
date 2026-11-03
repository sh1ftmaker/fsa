#ifndef GAME_ACT_ACT_ENEMY_FOR_ENDING_CONTROL_H
#define GAME_ACT_ACT_ENEMY_FOR_ENDING_CONTROL_H

// Code: game/act_enemy_for_ending_control.cpp.

#include "game/ActorBase.h"

// The ENPC actor ("Enemy for ending control", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_80200F24, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AD48C.
class EnemyForEndingControl_804AD48C : public ActorBase_804A5770 {
public:
    EnemyForEndingControl_804AD48C() {}
    /* 0x08 */ virtual ~EnemyForEndingControl_804AD48C();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x74]; // not yet known
}; // Size: 0x2A4

#endif
