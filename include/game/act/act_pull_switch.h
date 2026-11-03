#ifndef GAME_ACT_ACT_PULL_SWITCH_H
#define GAME_ACT_ACT_PULL_SWITCH_H

// Code: game/act_pull_switch.cpp.

#include "game/ActorBase.h"

// The PLSW actor ("Pull switch", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80202480, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A7908.
class PullSwitch_804A7908 : public ActorBase_804A5770 {
public:
    PullSwitch_804A7908() {}
    /* 0x08 */ virtual ~PullSwitch_804A7908();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x20]; // not yet known
}; // Size: 0x250

#endif
