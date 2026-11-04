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
    // The words init zeroes or sets (all stw); their meaning is not known yet.
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
}; // Size: 0x250

#endif
