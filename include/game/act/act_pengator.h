#ifndef GAME_ACT_ACT_PENGATOR_H
#define GAME_ACT_ACT_PENGATOR_H

// Code: game/act_pengator.cpp.

#include "game/ActorBase.h"

// The FAGN actor ("Pengator", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_8020221C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A1888.
class Pengator_804A1888 : public ActorBase_804A5770 {
public:
    Pengator_804A1888() {}
    /* 0x08 */ virtual ~Pengator_804A1888();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x18 */ virtual void vfunc_0x18();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x34]; // not yet known
}; // Size: 0x264

#endif
