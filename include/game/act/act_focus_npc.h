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
    /* 0x230 */ s32 mState;     // 0 waiting, 1 on (counts down), 2 waits for the camera only
    /* 0x234 */ s32 mSwitch;    // init: field_0x90 bits 22-26; 0 = no switch needed
    /* 0x238 */ u8 field_0x238[4]; // not yet known
    /* 0x23C */ s32 mDuration;  // init: field_0x90 & 0xFFF; 0 starts in state 2
    /* 0x240 */ s32 mTimer;     // counts down from mDuration in state 1
}; // Size: 0x244

#endif
