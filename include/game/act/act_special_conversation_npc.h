#ifndef GAME_ACT_ACT_SPECIAL_CONVERSATION_NPC_H
#define GAME_ACT_ACT_SPECIAL_CONVERSATION_NPC_H

// Code: game/act_special_conversation_npc.cpp.

#include "game/ActorBase.h"

// The SNPC actor ("Special conversation NPC", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_801FFC0C, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AAFB8.
class SpecialConversationNPC_804AAFB8 : public ActorBase_804A5770 {
public:
    SpecialConversationNPC_804AAFB8() {}
    /* 0x08 */ virtual ~SpecialConversationNPC_804AAFB8();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x44]; // not yet known
}; // Size: 0x274

#endif
