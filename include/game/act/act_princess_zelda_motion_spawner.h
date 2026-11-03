#ifndef GAME_ACT_ACT_PRINCESS_ZELDA_MOTION_SPAWNER_H
#define GAME_ACT_ACT_PRINCESS_ZELDA_MOTION_SPAWNER_H

// Code: game/act_princess_zelda_motion_spawner.cpp.

#include "game/ActorBase.h"

// The ZDCO actor ("Princess Zelda motion object spawner",
// docs/names/actors.tsv, med confidence): the spawner constructs it with
// fn_80200728, which stores this vtable (config/G4SE01/game_units.tsv). Named
// after its vtable, 0x804A375C.
class PrincessZeldaMotionObjectSpawner_804A375C : public ActorBase_804A5770 {
public:
    PrincessZeldaMotionObjectSpawner_804A375C() {}
    /* 0x08 */ virtual ~PrincessZeldaMotionObjectSpawner_804A375C();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x48]; // not yet known
}; // Size: 0x278

#endif
