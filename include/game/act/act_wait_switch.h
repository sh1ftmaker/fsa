#ifndef GAME_ACT_ACT_WAIT_SWITCH_H
#define GAME_ACT_ACT_WAIT_SWITCH_H

// Code: game/act_wait_switch.cpp.

#include "game/ActorBase.h"

// The WTSW actor ("Wait switch", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80202DC0, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A8B20.
class WaitSwitch_804A8B20 : public ActorBase_804A5770 {
public:
    WaitSwitch_804A8B20() {}
    /* 0x08 */ virtual ~WaitSwitch_804A8B20();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 mState;        // 0: waiting for the switch, 1: switch seen, 2: done
    /* 0x234 */ s32 mCount;        // field_0x90 & 0x3FFFF, passed to fn_80134DE4; period of mode 6
    /* 0x238 */ s32 mTimer;        // starts at mCount
    /* 0x23C */ s32 mMode;         // (field_0x90 >> 19) & 7: 1-5 move the view bounds, 6 toggles the switch
    /* 0x240 */ s32 mBit;          // (field_0x90 >> 22) & 0x1F: the switch bit to wait for
    /* 0x244 */ s32 mToggle;       // mode 6: the switch value being set
    /* 0x248 */ f32 mValue;        // first argument of the view update, from a table by mMode
    /* 0x24C */ s32 mStepTimer;    // frames until mValue steps (modes 4 and 5)
    /* 0x250 */ s32 mStepPeriod;
    /* 0x254 */ s32 field_0x254;   // (field_0x90 >> 18) & 1, never read
}; // Size: 0x258

#endif
