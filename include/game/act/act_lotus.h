#ifndef GAME_ACT_ACT_LOTUS_H
#define GAME_ACT_ACT_LOTUS_H

// Code: game/act_lotus.cpp.

#include "game/ActorBase.h"

// The LOTS actor ("Lotus", docs/names/actors.tsv, med confidence): the spawner
// constructs it with fn_80201C2C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AC330.
class Lotus_804AC330 : public ActorBase_804A5770 {
public:
    Lotus_804AC330() {}
    /* 0x08 */ virtual ~Lotus_804AC330();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x34]; // not yet known
}; // Size: 0x264

#endif
