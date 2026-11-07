#ifndef GAME_ACT_ACT_FORCE_GEM_SPAWNER_H
#define GAME_ACT_ACT_FORCE_GEM_SPAWNER_H

// Code: game/act_force_gem_spawner.cpp.

#include "game/ActorBase.h"

// The GOLD actor ("Force Gem spawner", docs/names/actors.tsv, med confidence):
// the spawner constructs it with fn_80203F14, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AB660.
class ForceGemSpawner_804AB660 : public ActorBase_804A5770 {
public:
    ForceGemSpawner_804AB660() {}
    /* 0x08 */ virtual ~ForceGemSpawner_804AB660();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230; // state
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ f32 field_0x244;
    /* 0x248 */ f32 field_0x248; // draw scale
    /* 0x24C */ JPABaseEmitter* mEmitter24c; // released by the destructor
    /* 0x250 */ JPABaseEmitter* mEmitter250; // released by the destructor
    /* 0x254 */ JPABaseEmitter* mEmitter254; // released by the destructor
}; // Size: 0x258

#endif
