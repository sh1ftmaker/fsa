#ifndef GAME_ACT_ACT_FORCE_GEM_H
#define GAME_ACT_ACT_FORCE_GEM_H

// Code: game/act_force_gem.cpp.

#include "game/ActorBase.h"

// The RUPY actor ("Force Gem", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80204298, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AB6D8.
class ForceGem_804AB6D8 : public ActorBase_804A5770 {
public:
    ForceGem_804AB6D8() {}
    /* 0x08 */ virtual ~ForceGem_804AB6D8();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x40 */ virtual void vfunc_0x40();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ u8 field_0x230[0x78]; // not yet known
}; // Size: 0x2A8

#endif
