#ifndef GAME_ACT_ACT_LIFT_H
#define GAME_ACT_ACT_LIFT_H

// Code: game/act_lift.cpp.

#include "game/ActorBase.h"

// The LIFT actor ("Lift", docs/names/actors.tsv, high confidence): the spawner
// constructs it with fn_80203144, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A6F6C.
class Lift_804A6F6C : public ActorBase_804A5770 {
public:
    Lift_804A6F6C() {}
    /* 0x08 */ virtual ~Lift_804A6F6C();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x10]; // not yet known
}; // Size: 0x240

#endif
