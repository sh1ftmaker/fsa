#ifndef GAME_ACT_ACT_ARMOS_H
#define GAME_ACT_ACT_ARMOS_H

// Code: game/act_armos.cpp.

#include "game/ActorBase.h"

// The AMOS actor ("Armos", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80204220, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049F260.
class Armos_8049F260 : public ActorBase_804A5770 {
public:
    Armos_8049F260() {}
    /* 0x08 */ virtual ~Armos_8049F260();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0xFC]; // not yet known
}; // Size: 0x32C

#endif
