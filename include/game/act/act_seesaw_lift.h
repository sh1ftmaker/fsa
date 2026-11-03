#ifndef GAME_ACT_ACT_SEESAW_LIFT_H
#define GAME_ACT_ACT_SEESAW_LIFT_H

// Code: game/act_seesaw_lift.cpp.

#include "game/ActorBase.h"

// The SSLF actor ("Seesaw lift", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_8020113C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AD348.
class SeesawLift_804AD348 : public ActorBase_804A5770 {
public:
    SeesawLift_804AD348() {}
    /* 0x08 */ virtual ~SeesawLift_804AD348();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x44]; // not yet known
}; // Size: 0x274

#endif
