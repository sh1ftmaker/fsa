#ifndef GAME_ACT_ACT_WARP_CANNON_H
#define GAME_ACT_ACT_WARP_CANNON_H

// Code: game/act_warp_cannon.cpp.

#include "game/ActorBase.h"
#include "game/SpriteAnim.h"

// The WPCN actor ("Warp cannon", docs/names/actors.tsv, med confidence): the
// spawner constructs it with __ct__19WarpCannon_8049EAE0Fv, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049EAE0.
class WarpCannon_8049EAE0 : public ActorBase_804A5770 {
public:
    WarpCannon_8049EAE0();
    /* 0x08 */ virtual ~WarpCannon_8049EAE0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // Stores the state in field_0x268 and runs its block: 1, 2, 3, 4, 6, 7 and
    // 8 do something; 0 and 5 nothing (the jump table's default).
    void fn_80130FAC(s32 state);
    /* 0x230 */ SpriteAnim_801D267C mAnim;
    /* 0x268 */ s32 field_0x268; // not yet known; 0 in the constructor
    /* 0x26C */ s32 field_0x26c; // -1 in the constructor
    /* 0x270 */ s32 field_0x270;
    /* 0x274 */ s32 field_0x274;
    /* 0x278 */ s32 field_0x278;
    /* 0x27C */ f32 field_0x27c;
    /* 0x280 */ f32 field_0x280;
    /* 0x284 */ f32 field_0x284;
    /* 0x288 */ f32 field_0x288;
    /* 0x28C */ f32 field_0x28c;
    /* 0x290 */ JGeometry::TVec3f field_0x290; // copied from fn_8023E32C(0), then fn_80230498's position
}; // Size: 0x29C

#endif
