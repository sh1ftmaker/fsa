#ifndef GAME_ACT_ACT_OCTOROK_H
#define GAME_ACT_ACT_OCTOROK_H

// Code: game/act_octorok.cpp.

#include "game/ActorBase.h"

// The OKTA actor ("Octorok", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_8020425C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049F208.
class Octorok_8049F208 : public ActorBase_804A5770 {
public:
    Octorok_8049F208() {}
    /* 0x08 */ virtual ~Octorok_8049F208();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x58]; // not yet known
}; // Size: 0x288

#endif
