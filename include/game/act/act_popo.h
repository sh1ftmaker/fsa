#ifndef GAME_ACT_ACT_POPO_H
#define GAME_ACT_ACT_POPO_H

// Code: game/act_popo.cpp.

#include "game/ActorBase.h"

// The POPO actor ("Popo", docs/names/actors.tsv, high confidence): the spawner
// constructs it with fn_80203234, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A0868.
class Popo_804A0868 : public ActorBase_804A5770 {
public:
    Popo_804A0868() {}
    /* 0x08 */ virtual ~Popo_804A0868();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x2C]; // not yet known
}; // Size: 0x25C

#endif
