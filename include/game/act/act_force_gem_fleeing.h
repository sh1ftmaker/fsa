#ifndef GAME_ACT_ACT_FORCE_GEM_FLEEING_H
#define GAME_ACT_ACT_FORCE_GEM_FLEEING_H

// Code: game/act_force_gem_fleeing.cpp.

#include "game/ActorBase.h"

// The ESRY actor ("Fleeing Force Gem", docs/names/actors.tsv, med confidence):
// the spawner constructs it with fn_80202868, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AB170.
class FleeingForceGem_804AB170 : public ActorBase_804A5770 {
public:
    FleeingForceGem_804AB170() {}
    /* 0x08 */ virtual ~FleeingForceGem_804AB170();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ u8 field_0x234[0x6C]; // not yet known
}; // Size: 0x2A0

#endif
