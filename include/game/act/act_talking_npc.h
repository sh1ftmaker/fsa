#ifndef GAME_ACT_ACT_TALKING_NPC_H
#define GAME_ACT_ACT_TALKING_NPC_H

// Code: game/act_talking_npc.cpp.

#include "game/ActorBase.h"
#include "game/Message.h"

// TALK ("Talking NPC", docs/names/actors.tsv). Named after its vtable
// (0x804AF548); the spawner allocates 0x23C bytes (__nw__ before
// fn_802033F4, its out-of-line constructor).
class TalkingNPC_804AF548 : public ActorBase_804A5770 {
public:
    TalkingNPC_804AF548() {}
    /* 0x08 */ virtual ~TalkingNPC_804AF548(); // fn_80394134
    /* 0x0C */ virtual void init();   // fn_8039427C
    /* 0x14 */ virtual void update(); // fn_80394198
    /* 0x20 */ virtual void draw();   // fn_80394194, empty

    // fn_8038AD24 takes the position by value; the serial is read inside so it
    // is loaded after the position copy (RULES_CARD, by-value message calls).
    bool talk(u32 player, u32 msg, s32 layer, const JGeometry::TVec3f& pos, u32 flags) {
        return fn_8038AD24(player, msg, field_0x234, layer, pos, flags);
    }

    /* 0x230 */ s32 mState; // 0: open the message, 1: wait for it to end
    /* 0x234 */ s32 field_0x234; // mSerial, as fn_8038AD24's serial
    /* 0x238 */ s16 field_0x238; // 0x78-frame timeout of state 1
}; // Size: 0x23C

#endif
