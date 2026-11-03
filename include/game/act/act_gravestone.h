#ifndef GAME_ACT_ACT_GRAVESTONE_H
#define GAME_ACT_ACT_GRAVESTONE_H

// Code: game/act_gravestone.cpp.

#include "game/ActorBase.h"

// The GRVE actor ("Gravestone", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80202168, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A99C0.
class Gravestone_804A99C0 : public ActorBase_804A5770 {
public:
    Gravestone_804A99C0() {}
    /* 0x08 */ virtual ~Gravestone_804A99C0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x1C]; // not yet known
}; // Size: 0x24C

#endif
