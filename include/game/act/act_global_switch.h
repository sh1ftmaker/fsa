#ifndef GAME_ACT_ACT_GLOBAL_SWITCH_H
#define GAME_ACT_ACT_GLOBAL_SWITCH_H

// Code: game/act_global_switch.cpp.

#include "game/ActorBase.h"

// The GLBS actor ("Global switch", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80200E58, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AA060.
class GlobalSwitch_804AA060 : public ActorBase_804A5770 {
public:
    GlobalSwitch_804AA060() {}
    /* 0x08 */ virtual ~GlobalSwitch_804AA060();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
}; // Size: 0x230

#endif
