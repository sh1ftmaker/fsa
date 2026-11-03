#ifndef GAME_ACT_ACT_SPOTLIGHT_SE_H
#define GAME_ACT_ACT_SPOTLIGHT_SE_H

// Code: game/act_spotlight_se.cpp.

#include "game/ActorBase.h"

// The SLSD actor ("Spotlight SE", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80203340, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A85E0.
class SpotlightSE_804A85E0 : public ActorBase_804A5770 {
public:
    SpotlightSE_804A85E0() {}
    /* 0x08 */ virtual ~SpotlightSE_804A85E0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x4]; // not yet known
}; // Size: 0x234

#endif
