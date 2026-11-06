#ifndef GAME_ACT_ACT_FORCE_GEM_WINGED_H
#define GAME_ACT_ACT_FORCE_GEM_WINGED_H

// Code: game/act_force_gem_winged.cpp.

#include "game/ActorBase.h"

// The HNRY actor ("Winged Force Gem", docs/names/actors.tsv, med confidence):
// the spawner constructs it with fn_8020291C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AB098.
class WingedForceGem_804AB098 : public ActorBase_804A5770 {
public:
    WingedForceGem_804AB098() {}
    /* 0x08 */ virtual ~WingedForceGem_804AB098();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ u8 field_0x234[0x281 - 0x234]; // not yet known
    /* 0x281 */ u8 field_0x281;
    /* 0x282 */ u8 field_0x282[0x2BC - 0x282]; // not yet known
}; // Size: 0x2BC

#endif
