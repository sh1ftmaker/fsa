#ifndef GAME_ACT_ACT_CACTUS_H
#define GAME_ACT_ACT_CACTUS_H

// Code: game/act_cactus.cpp.

#include "game/ActorBase.h"

// The SBTN actor ("Cactus", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80202390, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AB580.
class Cactus_804AB580 : public ActorBase_804A5770 {
public:
    Cactus_804AB580() {}
    /* 0x08 */ virtual ~Cactus_804AB580();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x10]; // not yet known
}; // Size: 0x240

#endif
