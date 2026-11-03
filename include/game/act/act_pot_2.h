#ifndef GAME_ACT_ACT_POT_2_H
#define GAME_ACT_ACT_POT_2_H

// Code: game/act_pot_2.cpp.

#include "game/ActorBase.h"

// The TBO2 actor ("Pot 2", docs/names/actors.tsv, med confidence): the spawner
// constructs it with fn_801FF918, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AA750.
class Pot2_804AA750 : public ActorBase_804A5770 {
public:
    Pot2_804AA750() {}
    /* 0x08 */ virtual ~Pot2_804AA750();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
}; // Size: 0x230

#endif
