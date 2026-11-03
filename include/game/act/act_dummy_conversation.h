#ifndef GAME_ACT_ACT_DUMMY_CONVERSATION_H
#define GAME_ACT_ACT_DUMMY_CONVERSATION_H

// Code: game/act_dummy_conversation.cpp.

#include "game/ActorBase.h"
#include "game/Message.h"

// The DTLK actor ("Dummy conversation", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_80200C5C, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804B1A48.
class DummyConversation_804B1A48 : public ActorBase_804A5770 {
public:
    DummyConversation_804B1A48() {}
    /* 0x08 */ virtual ~DummyConversation_804B1A48();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    // update: 0 picks the message with the d-pad/buttons, 1 opens it
    // (fn_8038C678), 2 waits for the window to close.
    // Opens the message with the position passed on by value; the message
    // id is read after the position copy (research_byval, rules card).
    bool open(s32 player, s32 layer, s32 serial, const JGeometry::TVec3f& pos, u32 flags) {
        return fn_8038C678(player, mMessage, serial, layer, pos, flags);
    }
    /* 0x230 */ s32 mState;
    /* 0x234 */ u32 mMessage; // message id, starts at 1
}; // Size: 0x238

#endif
