#ifndef GAME_ACT_ACT_SPECIAL_CONVERSATION_NPC_H
#define GAME_ACT_ACT_SPECIAL_CONVERSATION_NPC_H

// Code: game/act_special_conversation_npc.cpp.

#include "game/ActorBase.h"
#include "game/Message.h"

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

    // The message state machine (fn_803185A8, not yet named): the box test
    // of the message's two states and the cycling of field_0x25c.
    void fn_803185A8();
    // The other state-table entries (lbl_804AAED4's initialiser lbl_804AAE68
    // holds them with fn_803185A8, so they are members taking no arguments);
    // not written yet.
    void fn_80317F18();
    void fn_8031825C();
    void fn_803187F4();
    void fn_80318B84();
    void fn_80318D74();
    void fn_803190E0();
    void fn_803193B4();
    void fn_80319658();

    void updateFacing();

    // fn_8038AD24 takes the position by value. The original loads the message,
    // serial and layer before that copy, so they are parameters here.
    bool talk(u32 player, u32 msg, s32 serial, s32 layer, const JGeometry::TVec3f& pos, u32 flags) {
        return fn_8038AD24(player, msg, serial, layer, pos, flags);
    }
    bool talkC678(u32 player, u32 msg, s32 serial, s32 layer, const JGeometry::TVec3f& pos, u32 flags) {
        return fn_8038C678(player, msg, serial, layer, pos, flags);
    }
    /* 0x230 */ s32 mState; // 0: look for a player, 1: wait for the message to end
    /* 0x234 */ s32 mPlayer; // the player the message is open for; -1 when none
    /* 0x238 */ u32 field_0x238;     // index into the state table (lbl_804AAED4)
    /* 0x23C */ u32 field_0x23c;
    /* 0x240 */ u32 field_0x240;    // 0, 1 or 2: compared against vfunc_0x34()
    /* 0x244 */ u32 field_0x244;    // message for fn_8038AD24
    /* 0x248 */ JGeometry::TBox2f field_0x248; // box moved by mPos for fn_8022FEB8 / fn_8022FF8C
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;    // 0 or 1 toggled, or an index into lbl_804AAF40
    /* 0x260 */ s32 field_0x260;    // frame counter
    /* 0x264 */ s32 field_0x264;    // index into lbl_804AAF40 (0-5)
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ u8 field_0x270;      // 1: run the state table entry this frame
    /* 0x271 */ u8 field_0x271[0x3]; // not yet known
}; // Size: 0x274

#endif
