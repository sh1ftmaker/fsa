#ifndef GAME_ACT_ACT_THWOMP_LAUNCH_NPC_2_H
#define GAME_ACT_ACT_THWOMP_LAUNCH_NPC_2_H

// Code: game/act_thwomp_launch_npc_2.cpp.

#include "game/ActorBase.h"

// The JPE2 actor ("NPC launched by Thwomp 2", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_801FFA08, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AD1C8.
class NPCLaunchedByThwomp2_804AD1C8 : public ActorBase_804A5770 {
public:
    NPCLaunchedByThwomp2_804AD1C8() {}
    /* 0x08 */ virtual ~NPCLaunchedByThwomp2_804AD1C8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x54]; // not yet known
}; // Size: 0x284

#endif
