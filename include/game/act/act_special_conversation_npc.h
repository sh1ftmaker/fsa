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
    /* 0x230 */ u8 field_0x230[0x8]; // not yet known
    /* 0x238 */ s32 field_0x238;     // index into the state table (lbl_804AAED4)
    /* 0x23C */ u8 field_0x23C[0x4]; // not yet known
    /* 0x240 */ u32 field_0x240;    // 0, 1 or 2: compared against vfunc_0x34()
    /* 0x244 */ u8 field_0x244[0x2C]; // not yet known
    /* 0x270 */ u8 field_0x270;      // 1: run the state table entry this frame
    /* 0x271 */ u8 field_0x271[0x3]; // not yet known
}; // Size: 0x274

#endif
