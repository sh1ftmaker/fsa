#ifndef GAME_ACT_ACT_WARP_CANNON_H
#define GAME_ACT_ACT_WARP_CANNON_H

// Code: game/act_warp_cannon.cpp.

#include "game/ActorBase.h"
#include "game/SpriteAnim.h"

// The WPCN actor ("Warp cannon", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80131AF0, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049EAE0.
class WarpCannon_8049EAE0 : public ActorBase_804A5770 {
public:
    WarpCannon_8049EAE0() {}
    /* 0x08 */ virtual ~WarpCannon_8049EAE0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ SpriteAnim_801D267C mAnim;
    /* 0x268 */ u8 field_0x268[0x34]; // not yet known
}; // Size: 0x29C

#endif
