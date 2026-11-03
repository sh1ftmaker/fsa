#ifndef GAME_ACT_ACT_TALKING_NPC_H
#define GAME_ACT_ACT_TALKING_NPC_H

// Code: game/act_talking_npc.cpp.

#include "game/ActorBase.h"

// The TALK actor ("Talking NPC", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_802033F4, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AF548.
class TalkingNPC_804AF548 : public ActorBase_804A5770 {
public:
    TalkingNPC_804AF548() {}
    /* 0x08 */ virtual ~TalkingNPC_804AF548();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0xC]; // not yet known
}; // Size: 0x23C

#endif
