#ifndef GAME_ACT_ACT_SEARCHLIGHT_H
#define GAME_ACT_ACT_SEARCHLIGHT_H

// Code: game/act_searchlight.cpp.

#include "game/ActorBase.h"

// The SHLT actor ("Searchlight", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_802028A4, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A0FF8.
class Searchlight_804A0FF8 : public ActorBase_804A5770 {
public:
    Searchlight_804A0FF8() {}
    /* 0x08 */ virtual ~Searchlight_804A0FF8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x14C]; // not yet known
}; // Size: 0x37C

#endif
