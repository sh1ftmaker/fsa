#ifndef GAME_ACT_ACT_OFF_SWITCH_H
#define GAME_ACT_ACT_OFF_SWITCH_H

// Code: game/act_off_switch.cpp.

#include "game/ActorBase.h"

// The OFFS actor ("OFF switch", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80201BB4, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A9B70.
class OFFSwitch_804A9B70 : public ActorBase_804A5770 {
public:
    OFFSwitch_804A9B70() {}
    /* 0x08 */ virtual ~OFFSwitch_804A9B70();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230;  // update state: 0 idle, 1 waiting for release
    /* 0x234 */ s32 field_0x234;  // field_0x90 & 0x1F: behaviour (0-4), switched in update
    /* 0x238 */ s32 field_0x238;  // (field_0x90 >> 22) & 0x1F: the bit passed to GameView::fn_802D8050
    /* 0x23C */ bool field_0x23c; // last value written for behaviour 2 (the toggle)
}; // Size: 0x240

#endif
