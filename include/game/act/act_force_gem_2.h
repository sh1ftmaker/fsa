#ifndef GAME_ACT_ACT_FORCE_GEM_2_H
#define GAME_ACT_ACT_FORCE_GEM_2_H

// Code: game/act_force_gem_2.cpp.

#include "game/ActorBase.h"

// The RY02 actor ("Force Gem 2", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80203180, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AB850.
class ForceGem2_804AB850 : public ActorBase_804A5770 {
public:
    ForceGem2_804AB850() {}
    /* 0x08 */ virtual ~ForceGem2_804AB850();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ u8 field_0x230[0x54]; // not yet known
    /* 0x284 */ JPABaseEmitter* mEmitter284; // released by the destructor
}; // Size: 0x288

#endif
