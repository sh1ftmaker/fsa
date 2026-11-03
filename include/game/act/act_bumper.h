#ifndef GAME_ACT_ACT_BUMPER_H
#define GAME_ACT_ACT_BUMPER_H

// Code: game/act_bumper.cpp.

#include "game/ActorBase.h"

// The BOYO actor ("Bumper", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80203B78, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A0260.
class Bumper_804A0260 : public ActorBase_804A5770 {
public:
    Bumper_804A0260() {}
    /* 0x08 */ virtual ~Bumper_804A0260();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x48]; // not yet known
}; // Size: 0x278

#endif
