#ifndef GAME_ACT_ACT_HAMMER_BULGE_H
#define GAME_ACT_ACT_HAMMER_BULGE_H

// Code: game/act_hammer_bulge.cpp.

#include "game/ActorBase.h"

// The SWEL actor ("Hammer bulge", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80203108, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A88E8.
class HammerBulge_804A88E8 : public ActorBase_804A5770 {
public:
    HammerBulge_804A88E8() {}
    /* 0x08 */ virtual ~HammerBulge_804A88E8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x20]; // not yet known
}; // Size: 0x250

#endif
