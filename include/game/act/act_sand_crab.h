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
    /* 0x230 */ s32 field_0x230; // state: 0 and 2 wait, 1 walks (in update)
    /* 0x234 */ s32 field_0x234; // counts down; reloads to 8 (2 in state 2)
    /* 0x238 */ s32 field_0x238; // counts down; state timer
    /* 0x23C */ s32 field_0x23c; // counts down; update does nothing until 0
    /* 0x240 */ s32 field_0x240; // 0 or 1, flips each time field_0x234 reloads; draw frame
    /* 0x244 */ s32 field_0x244; // only init writes it (0)
    /* 0x248 */ s32 field_0x248; // only init writes it (0)
}; // Size: 0x24C

#endif
