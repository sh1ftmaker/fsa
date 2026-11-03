#ifndef GAME_ACT_ACT_FOCUS_NPC_H
#define GAME_ACT_ACT_FOCUS_NPC_H

// Code: game/act_focus_npc.cpp.

#include "game/ActorBase.h"

// The FOCS actor ("Focus NPC", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80202994, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A8EA0.
class FocusNPC_804A8EA0 : public ActorBase_804A5770 {
public:
    FocusNPC_804A8EA0() {}
    /* 0x08 */ virtual ~FocusNPC_804A8EA0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x14]; // not yet known
}; // Size: 0x244

#endif
