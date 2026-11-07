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
    /* 0x234 */ s32 field_0x234; // the state before field_0x230 changed
    /* 0x238 */ f32 field_0x238; // circling radius (30)
    /* 0x23C */ f32 field_0x23c;
    /* 0x240 */ f32 field_0x240;
    /* 0x244 */ f32 field_0x244;
    /* 0x248 */ f32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ s32 field_0x264; // circling angle (sincosTable_ index)
    /* 0x268 */ s32 field_0x268; // circling step (100, or -100 for kind 3)
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ s32 field_0x270; // bits 0-2 of field_0x90
    /* 0x274 */ s32 field_0x274; // bit 12
    /* 0x278 */ s32 field_0x278;
    /* 0x27C */ s32 field_0x27c;
    /* 0x280 */ u8 field_0x280;
    /* 0x281 */ u8 field_0x281; // bit 24
    /* 0x282 */ u8 field_0x282; // bit 25
    /* 0x283 */ u8 field_0x283; // bit 26
    /* 0x284 */ u8 field_0x284; // bit 14
    /* 0x285 */ u8 field_0x285; // bit 8
    /* 0x286 */ u8 field_0x286; // bit 10
    /* 0x287 */ u8 field_0x287[1]; // not yet known
    /* 0x288 */ u16 field_0x288;
    /* 0x28A */ u8 field_0x28a[2]; // not yet known
    /* 0x28C */ JGeometry::TVec3f field_0x28c;
    /* 0x298 */ JGeometry::TVec3f field_0x298; // mPos at init
    /* 0x2A4 */ JGeometry::TVec3f field_0x2a4;
    /* 0x2B0 */ JGeometry::TVec3f field_0x2b0;
}; // Size: 0x2BC

#endif
