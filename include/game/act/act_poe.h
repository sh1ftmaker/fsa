#ifndef GAME_ACT_ACT_POE_H
#define GAME_ACT_ACT_POE_H

// Code: game/act_poe.cpp.

#include "game/ActorBase.h"

// The POOH actor ("Poe", docs/names/actors.tsv, med confidence): the spawner
// constructs it with fn_802041E4, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049F330.
class Poe_8049F330 : public ActorBase_804A5770 {
public:
    Poe_8049F330() {}
    /* 0x08 */ virtual ~Poe_8049F330();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x48]; // not yet known
}; // Size: 0x278

#endif
