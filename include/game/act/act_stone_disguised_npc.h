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
    /* 0x230 */ s32 mState; // update(): 0 find the player, 1 slide, 2 wait, 3 chase
    /* 0x234 */ f32 field_0x234; // target velocity x (+-2 or 0)
    /* 0x238 */ f32 field_0x238; // target velocity y (+-2 or 0)
    /* 0x23C */ f32 field_0x23c; // zeroed, never read
    /* 0x240 */ f32 field_0x240; // velocity step rate: 0.75 from init, 1.0 once chasing
    /* 0x244 */ f32 field_0x244; // chase speed (4.0)
    /* 0x248 */ s32 field_0x248; // wait timer
    /* 0x24C */ s32 field_0x24c; // player found by fn_8023AF14, -1 for none
}; // Size: 0x250

#endif
