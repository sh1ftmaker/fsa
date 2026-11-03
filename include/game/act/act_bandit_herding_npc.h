#ifndef GAME_ACT_ACT_BANDIT_HERDING_NPC_H
#define GAME_ACT_ACT_BANDIT_HERDING_NPC_H

// Code: game/act_bandit_herding_npc.cpp.

#include "game/ActorBase.h"

// The TOOI actor ("Bandit herding NPC", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_8020034C, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804ACB40.
class BanditHerdingNPC_804ACB40 : public ActorBase_804A5770 {
public:
    BanditHerdingNPC_804ACB40() {}
    /* 0x08 */ virtual ~BanditHerdingNPC_804ACB40();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x20]; // not yet known
}; // Size: 0x250

#endif
