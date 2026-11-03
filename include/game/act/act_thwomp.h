#ifndef GAME_ACT_ACT_THWOMP_H
#define GAME_ACT_ACT_THWOMP_H

// Code: game/act_thwomp.cpp.

#include "game/ActorBase.h"

// The DSIN actor ("Thwomp", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_802022E0, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AB5D8.
class Thwomp_804AB5D8 : public ActorBase_804A5770 {
public:
    Thwomp_804AB5D8() {}
    /* 0x08 */ virtual ~Thwomp_804AB5D8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x18]; // not yet known
}; // Size: 0x248

#endif
