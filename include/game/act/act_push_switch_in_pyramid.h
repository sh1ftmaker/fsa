#ifndef GAME_ACT_ACT_PUSH_SWITCH_IN_PYRAMID_H
#define GAME_ACT_ACT_PUSH_SWITCH_IN_PYRAMID_H

// Code: game/act_push_switch_in_pyramid.cpp.

#include "game/ActorBase.h"

// The PSS2 actor ("Push switch in pyramid", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_801FF870, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AE8F4.
class PushSwitchInPyramid_804AE8F4 : public ActorBase_804A5770 {
public:
    PushSwitchInPyramid_804AE8F4() {}
    /* 0x08 */ virtual ~PushSwitchInPyramid_804AE8F4();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0xC]; // not yet known
    /* 0x23C */ s32 field_0x23c; // state: 0 pressing, 1 pushed in one step, 2 done
    /* 0x240 */ u8 field_0x240;  // the player index that must push (1 after init)
    /* 0x241 */ u8 field_0x241;  // how far the switch is pushed in (0-4)
    /* 0x242 */ u8 field_0x242;  // frames the player has pushed
    /* 0x243 */ u8 field_0x243;  // push progress within a step
}; // Size: 0x244

#endif
