#ifndef GAME_ACT_ACT_FORCE_GEM_WOODEN_H
#define GAME_ACT_ACT_FORCE_GEM_WOODEN_H

// Code: game/act_force_gem_wooden.cpp.

#include "game/ActorBase.h"

// The WDRY actor ("Wooden Force Gem", docs/names/actors.tsv, med confidence):
// the spawner constructs it with fn_8020282C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AB1F0.
class WoodenForceGem_804AB1F0 : public ActorBase_804A5770 {
public:
    WoodenForceGem_804AB1F0() {}
    /* 0x08 */ virtual ~WoodenForceGem_804AB1F0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ u8 field_0x230[0x38]; // not yet known
    /* 0x268 */ JPABaseEmitter* mEmitter268; // released by the destructor
}; // Size: 0x26C

#endif
