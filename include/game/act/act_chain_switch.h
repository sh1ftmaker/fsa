#ifndef GAME_ACT_ACT_CHAIN_SWITCH_H
#define GAME_ACT_ACT_CHAIN_SWITCH_H

// Code: game/act_chain_switch.cpp.

#include "game/ActorBase.h"

// The CHSW actor ("Chain switch", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80200388, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AA3E8.
class ChainSwitch_804AA3E8 : public ActorBase_804A5770 {
public:
    ChainSwitch_804AA3E8() {}
    /* 0x08 */ virtual ~ChainSwitch_804AA3E8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x14]; // not yet known
}; // Size: 0x244

#endif
