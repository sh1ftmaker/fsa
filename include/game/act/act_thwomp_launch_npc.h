#ifndef GAME_ACT_ACT_THWOMP_LAUNCH_NPC_H
#define GAME_ACT_ACT_THWOMP_LAUNCH_NPC_H

// Code: game/act_thwomp_launch_npc.cpp.

#include "game/ActorBase.h"

// The JPEN actor ("NPC launched by Thwomp", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_80201F8C, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AC010.
class NPCLaunchedByThwomp_804AC010 : public ActorBase_804A5770 {
public:
    NPCLaunchedByThwomp_804AC010() {}
    /* 0x08 */ virtual ~NPCLaunchedByThwomp_804AC010();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x54]; // not yet known
}; // Size: 0x284

#endif
