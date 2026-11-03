#ifndef GAME_ACT_ACT_SHUTTER_H
#define GAME_ACT_ACT_SHUTTER_H

// Code: game/act_shutter.cpp.

#include "game/ActorBase.h"

// The SHTR actor ("Shutter", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80203998, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A8118.
class Shutter_804A8118 : public ActorBase_804A5770 {
public:
    Shutter_804A8118() {}
    /* 0x08 */ virtual ~Shutter_804A8118();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x18]; // not yet known
}; // Size: 0x248

#endif
