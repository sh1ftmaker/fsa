#ifndef GAME_ACT_ACT_POT_2_GENERATOR_H
#define GAME_ACT_ACT_POT_2_GENERATOR_H

// Code: game/act_pot_2_generator.cpp.

#include "game/ActorBase.h"

// The TBGN actor ("Pot 2 generator", docs/names/actors.tsv, med confidence):
// the spawner constructs it with fn_801FF954, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AA6F8.
class Pot2Generator_804AA6F8 : public ActorBase_804A5770 {
public:
    Pot2Generator_804AA6F8() {}
    /* 0x08 */ virtual ~Pot2Generator_804AA6F8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230; // cell index init() walks over each tile layer
}; // Size: 0x234

#endif
