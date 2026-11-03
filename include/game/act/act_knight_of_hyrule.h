#ifndef GAME_ACT_ACT_KNIGHT_OF_HYRULE_H
#define GAME_ACT_ACT_KNIGHT_OF_HYRULE_H

// Code: game/act_knight_of_hyrule.cpp.

#include "game/ActorBase.h"

// The JWLK actor ("Knight of Hyrule", docs/names/actors.tsv, med confidence):
// the spawner constructs it with fn_8020041C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AABA8.
class KnightOfHyrule_804AABA8 : public ActorBase_804A5770 {
public:
    KnightOfHyrule_804AABA8() {}
    /* 0x08 */ virtual ~KnightOfHyrule_804AABA8();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x60]; // not yet known
}; // Size: 0x290

#endif
