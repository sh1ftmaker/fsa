#ifndef GAME_ACT_ACT_SAND_CRAB_H
#define GAME_ACT_ACT_SAND_CRAB_H

// Code: game/act_sand_crab.cpp.

#include "game/ActorBase.h"

// The PEAC actor ("Sand Crab", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80204004, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049F3B0.
class SandCrab_8049F3B0 : public ActorBase_804A5770 {
public:
    SandCrab_8049F3B0() {}
    /* 0x08 */ virtual ~SandCrab_8049F3B0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x1C]; // not yet known
}; // Size: 0x24C

#endif
