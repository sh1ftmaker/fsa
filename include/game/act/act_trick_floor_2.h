#ifndef GAME_ACT_ACT_TRICK_FLOOR_2_H
#define GAME_ACT_ACT_TRICK_FLOOR_2_H

// Code: game/act_trick_floor_2.cpp.

#include "game/ActorBase.h"

// The TFL2 actor ("Trick floor 2", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_801FFA80, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AA628.
class TrickFloor2_804AA628 : public ActorBase_804A5770 {
public:
    TrickFloor2_804AA628() {}
    /* 0x08 */ virtual ~TrickFloor2_804AA628();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x24]; // not yet known
}; // Size: 0x254

#endif
