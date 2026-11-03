#ifndef GAME_ACT_ACT_FORCE_GEM_RAIN_H
#define GAME_ACT_ACT_FORCE_GEM_RAIN_H

// Code: game/act_force_gem_rain.cpp.

#include "game/ActorBase.h"

// The RYRN actor ("Force Gem rain", docs/names/actors.tsv, med confidence):
// the spawner constructs it with fn_80203A4C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A8010.
class ForceGemRain_804A8010 : public ActorBase_804A5770 {
public:
    ForceGemRain_804A8010() {}
    /* 0x08 */ virtual ~ForceGemRain_804A8010();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x20]; // not yet known
}; // Size: 0x250

#endif
