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
    /* 0x230 */ u8 field_0x230[0x10]; // not yet known
}; // Size: 0x240

#endif
