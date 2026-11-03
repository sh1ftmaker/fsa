#ifndef GAME_ACT_ACT_DEADROCK_H
#define GAME_ACT_ACT_DEADROCK_H

// Code: game/act_deadrock.cpp.

#include "game/ActorBase.h"

// The DDRK actor ("Deadrock", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80203F50, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049F498.
class Deadrock_8049F498 : public ActorBase_804A5770 {
public:
    Deadrock_8049F498() {}
    /* 0x08 */ virtual ~Deadrock_8049F498();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x28]; // not yet known
}; // Size: 0x258

#endif
