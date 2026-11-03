#ifndef GAME_ACT_ACT_TRICK_FLOOR_H
#define GAME_ACT_ACT_TRICK_FLOOR_H

// Code: game/act_trick_floor.cpp.

#include "game/ActorBase.h"

// The TFLR actor ("Trick floor", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_802035D4, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A83A0.
class TrickFloor_804A83A0 : public ActorBase_804A5770 {
public:
    TrickFloor_804A83A0() {}
    /* 0x08 */ virtual ~TrickFloor_804A83A0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x24]; // not yet known
}; // Size: 0x254

#endif
