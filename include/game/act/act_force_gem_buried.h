#ifndef GAME_ACT_ACT_FORCE_GEM_BURIED_H
#define GAME_ACT_ACT_FORCE_GEM_BURIED_H

// Code: game/act_force_gem_buried.cpp.

#include "game/ActorBase.h"

// The BRRY actor ("Buried Force Gem", docs/names/actors.tsv, med confidence):
// the spawner constructs it with fn_802027B4, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AB258.
class BuriedForceGem_804AB258 : public ActorBase_804A5770 {
public:
    BuriedForceGem_804AB258() {}
    /* 0x08 */ virtual ~BuriedForceGem_804AB258();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ u8 field_0x234[0x3C]; // not yet known
}; // Size: 0x270

#endif
