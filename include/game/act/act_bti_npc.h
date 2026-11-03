#ifndef GAME_ACT_ACT_BTI_NPC_H
#define GAME_ACT_ACT_BTI_NPC_H

// Code: game/act_bti_npc.cpp.

#include "game/ActorBase.h"

// The BTIN actor ("BTI NPC", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_802021E0, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A927C.
class BTINPC_804A927C : public ActorBase_804A5770 {
public:
    BTINPC_804A927C() {}
    /* 0x08 */ virtual ~BTINPC_804A927C();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x2C]; // not yet known
}; // Size: 0x25C

#endif
