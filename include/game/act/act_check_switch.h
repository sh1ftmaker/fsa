#ifndef GAME_ACT_ACT_CHECK_SWITCH_H
#define GAME_ACT_ACT_CHECK_SWITCH_H

// Code: game/act_check_switch.cpp.

#include "game/ActorBase.h"

// The CKSW actor ("Check switch", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80202BF8, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A8BD0.
class CheckSwitch_804A8BD0 : public ActorBase_804A5770 {
public:
    CheckSwitch_804A8BD0() {}
    /* 0x08 */ virtual ~CheckSwitch_804A8BD0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 mState; // 0: waiting for the switch, 1: switch seen
    /* 0x234 */ s32 mMode;  // field_0x90 & 0x1F, set in init(): 0 despawn, 1 latch, 2 remove the actor (clears mAlive)
}; // Size: 0x238

#endif
