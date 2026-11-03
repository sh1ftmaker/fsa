#ifndef GAME_ACT_ACT_NPC_RIDE_SWITCH_H
#define GAME_ACT_ACT_NPC_RIDE_SWITCH_H

// Code: game/act_npc_ride_switch.cpp.

#include "game/ActorBase.h"

// The RINS actor ("NPC ride switch", docs/names/actors.tsv, med confidence):
// the spawner constructs it with fn_801FF7F8, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AD220.
class NPCRideSwitch_804AD220 : public ActorBase_804A5770 {
public:
    NPCRideSwitch_804AD220() {}
    /* 0x08 */ virtual ~NPCRideSwitch_804AD220();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230; // cleared by init
}; // Size: 0x234

#endif
