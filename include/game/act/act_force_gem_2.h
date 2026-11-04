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
    /* 0x230 */ s32 mState; // 5 or 6 once vfunc_0x4C has found a special tile
    /* 0x234 */ f32 mRadius; // 30.0f: the orbit radius (init's sin / cos)
    /* 0x238 */ f32 field_0x238; // 1.0f
    /* 0x23C */ f32 field_0x23c; // 0.0f
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248; // zeroed where mState is set to 6
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 mAngle; // random 0..65535 (init); sin / cos index
    /* 0x258 */ s32 mTimer; // 100 (init), -100 in init's case 3
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260; // -1 (init)
    /* 0x264 */ s32 field_0x264; // field_0x90 & 7
    /* 0x268 */ u16 field_0x268;
    /* 0x26A */ u8 field_0x26a[2];
    /* 0x26C */ JGeometry::TVec3f field_0x26c; // zeroed by init
    /* 0x278 */ JGeometry::TVec3f field_0x278; // copy of mPos made by init
    /* 0x284 */ JPABaseEmitter* mEmitter284; // released by the destructor
}; // Size: 0x288

#endif
