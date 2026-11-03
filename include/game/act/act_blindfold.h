#ifndef GAME_ACT_ACT_BLINDFOLD_H
#define GAME_ACT_ACT_BLINDFOLD_H

// Code: game/act_blindfold.cpp.

#include "game/ActorBase.h"

// The BLDF actor ("Blindfold", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80202958, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A8EF8.
class Blindfold_804A8EF8 : public ActorBase_804A5770 {
public:
    Blindfold_804A8EF8() {}
    /* 0x08 */ virtual ~Blindfold_804A8EF8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x18]; // not yet known
}; // Size: 0x248

#endif
