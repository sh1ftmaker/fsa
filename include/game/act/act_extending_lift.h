#ifndef GAME_ACT_ACT_EXTENDING_LIFT_H
#define GAME_ACT_ACT_EXTENDING_LIFT_H

// Code: game/act_extending_lift.cpp.

#include "game/ActorBase.h"

// The LIF2 actor ("Extending lift", docs/names/actors.tsv, med confidence):
// the spawner constructs it with fn_802027F0, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A78B0.
class ExtendingLift_804A78B0 : public ActorBase_804A5770 {
public:
    ExtendingLift_804A78B0() {}
    /* 0x08 */ virtual ~ExtendingLift_804A78B0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x18]; // not yet known
}; // Size: 0x248

#endif
