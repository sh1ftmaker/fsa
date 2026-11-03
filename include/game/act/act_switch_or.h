#ifndef GAME_ACT_ACT_SWITCH_OR_H
#define GAME_ACT_ACT_SWITCH_OR_H

// Code: game/act_switch_or.cpp.

#include "game/ActorBase.h"

// The SWOR actor ("Switch OR", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_802031F8, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A8800.
class SwitchOR_804A8800 : public ActorBase_804A5770 {
public:
    SwitchOR_804A8800() {}
    /* 0x08 */ virtual ~SwitchOR_804A8800();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x8]; // not yet known
}; // Size: 0x238

#endif
