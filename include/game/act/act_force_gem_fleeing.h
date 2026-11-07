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
    /* 0x230 */ s32 field_0x230; // state (0, 4 or 8 after init)
    /* 0x234 */ f32 field_0x234; // 30.0f (init)
    /* 0x238 */ f32 field_0x238; // 1.0f (init)
    /* 0x23C */ f32 field_0x23c; // 0.0f (init)
    /* 0x240 */ s32 field_0x240[5]; // zeroed by init
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c; // random angle 0..65535 (init)
    /* 0x260 */ s32 field_0x260; // 100 (init)
    /* 0x264 */ s32 field_0x264; // -1 (init)
    /* 0x268 */ s32 field_0x268; // field_0x90 & 3
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ s32 field_0x270; // the layer (field_0x4) at init
    /* 0x274 */ s16 field_0x274;
    /* 0x276 */ s16 field_0x276;
    /* 0x278 */ u8 field_0x278;
    /* 0x279 */ u8 field_0x279[3];
    /* 0x27C */ JGeometry::TVec3f field_0x27c;
    /* 0x288 */ JGeometry::TVec3f field_0x288; // the spawn position
    /* 0x294 */ JGeometry::TVec3f field_0x294;
}; // Size: 0x2A0

#endif
