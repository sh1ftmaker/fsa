#ifndef GAME_ACT_ACT_WARP_H
#define GAME_ACT_ACT_WARP_H

// Code: game/act_warp.cpp.

#include "game/ActorBase.h"

// The WARP actor ("Warp", docs/names/actors.tsv, med confidence): the spawner
// constructs it with fn_802040B8, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A6DC8.
class Warp_804A6DC8 : public ActorBase_804A5770 {
public:
    Warp_804A6DC8() {}
    /* 0x08 */ virtual ~Warp_804A6DC8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x1C]; // not yet known
    /* 0x24C */ JPABaseEmitter* mEmitter24c; // released by the destructor
    /* 0x250 */ JPABaseEmitter* mEmitter250; // released by the destructor
    /* 0x254 */ u8 field_0x254[0x38]; // not yet known
}; // Size: 0x28C

#endif
