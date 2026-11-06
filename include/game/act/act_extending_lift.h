#ifndef GAME_ACT_ACT_EXTENDING_LIFT_H
#define GAME_ACT_ACT_EXTENDING_LIFT_H

// Code: game/act_extending_lift.cpp.

#include "game/ActorBase.h"

struct ResTIMG;

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

    bool isPlayerOn(s32 player);

    /* 0x230 */ f32 field_0x230;
    /* 0x234 */ f32 field_0x234;
    /* 0x238 */ f32 field_0x238;
    /* 0x23C */ f32 field_0x23c;
    /* 0x240 */ const ResTIMG* field_0x240;
    /* 0x244 */ u8 field_0x244;
    /* 0x245 */ u8 field_0x245[3];
}; // Size: 0x248

#endif
