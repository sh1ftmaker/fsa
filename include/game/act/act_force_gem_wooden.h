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
    /* 0x230 */ s32 field_0x230; // state: 2 on tiles 0xC/0x3/0xE, 3 otherwise set (vfunc_0x4C)
    /* 0x234 */ f32 field_0x234; // 30.0f (init)
    /* 0x238 */ f32 field_0x238; // 1.0f (init)
    /* 0x23C */ f32 field_0x23c; // 0.0f (init)
    /* 0x240 */ f32 field_0x240; // 0.6f or 1.0f by field_0x258 (init)
    /* 0x244 */ s32 field_0x244; // 0 (init)
    /* 0x248 */ s32 field_0x248; // bits 16-19 of field_0x90 (init)
    /* 0x24C */ s32 field_0x24c; // 60 * random + 60 (init)
    /* 0x250 */ s32 field_0x250; // 0 (init)
    /* 0x254 */ s32 field_0x254; // -1 (init)
    /* 0x258 */ s32 field_0x258; // bits 0-3 of field_0x90 (init)
    /* 0x25C */ s32 field_0x25c; // 0 (init)
    /* 0x260 */ s32 field_0x260; // bits 22-26 of field_0x90 (init)
    /* 0x264 */ s16 field_0x264; // 0 (init)
    /* 0x268 */ JPABaseEmitter* mEmitter268; // released by the destructor
}; // Size: 0x26C

#endif
