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
    /* 0x230 */ u8 field_0x230[0x28]; // not yet known
}; // Size: 0x258

#endif
