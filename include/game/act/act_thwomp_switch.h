#ifndef GAME_ACT_ACT_THWOMP_SWITCH_H
#define GAME_ACT_ACT_THWOMP_SWITCH_H

// Code: game/act_thwomp_switch.cpp.

#include "game/ActorBase.h"

// The DOSN actor ("Thwomp switch", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80203018, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A8940.
class ThwompSwitch_804A8940 : public ActorBase_804A5770 {
public:
    ThwompSwitch_804A8940() {}
    /* 0x08 */ virtual ~ThwompSwitch_804A8940();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x4]; // not yet known
}; // Size: 0x234

#endif
