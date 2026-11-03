#ifndef GAME_ACT_ACT_STONE_DISGUISED_NPC_H
#define GAME_ACT_ACT_STONE_DISGUISED_NPC_H

// Code: game/act_stone_disguised_npc.cpp.

#include "game/ActorBase.h"

// The SSTN actor ("NPC disguised as a stone", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_8020337C, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A8750.
class NPCDisguisedAsAStone_804A8750 : public ActorBase_804A5770 {
public:
    NPCDisguisedAsAStone_804A8750() {}
    /* 0x08 */ virtual ~NPCDisguisedAsAStone_804A8750();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x230 */ u8 field_0x230[0x20]; // not yet known
}; // Size: 0x250

#endif
