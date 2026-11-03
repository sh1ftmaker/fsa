#ifndef GAME_ACT_ACT_FALLING_FLOOR_H
#define GAME_ACT_ACT_FALLING_FLOOR_H

// Code: game/act_falling_floor.cpp.

#include "game/ActorBase.h"

// The FLYK actor ("Falling floor", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80202408, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A9078.
class FallingFloor_804A9078 : public ActorBase_804A5770 {
public:
    FallingFloor_804A9078() {}
    /* 0x08 */ virtual ~FallingFloor_804A9078();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x14]; // not yet known
}; // Size: 0x244

#endif
