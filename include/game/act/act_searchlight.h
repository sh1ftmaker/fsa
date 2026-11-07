#ifndef GAME_ACT_ACT_SEARCHLIGHT_H
#define GAME_ACT_ACT_SEARCHLIGHT_H

// Code: game/act_searchlight.cpp.

#include "game/ActorBase.h"

// The SHLT actor ("Searchlight", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_802028A4, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A0FF8.
class Searchlight_804A0FF8 : public ActorBase_804A5770 {
public:
    Searchlight_804A0FF8() {}
    /* 0x08 */ virtual ~Searchlight_804A0FF8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x2F8 - 0x230]; // not yet known
    /* 0x2F8 */ s32 field_0x2f8;
    /* 0x2FC */ s32 field_0x2fc;
    /* 0x300 */ s32 field_0x300;
    /* 0x304 */ s32 field_0x304;
    /* 0x308 */ s32 field_0x308;
    /* 0x30C */ s32 field_0x30c;
    /* 0x310 */ s32 field_0x310;
    /* 0x314 */ s32 field_0x314;
    /* 0x318 */ s32 field_0x318;
    /* 0x31C */ s32 field_0x31c;
    /* 0x320 */ s32 field_0x320;
    /* 0x324 */ s32 field_0x324;
    /* 0x328 */ s32 field_0x328; // bits 4-7 of field_0x90
    /* 0x32C */ s32 field_0x32c; // bits 8-11
    /* 0x330 */ s32 field_0x330; // bits 12-15
    /* 0x334 */ s32 field_0x334; // bits 16-23
    /* 0x338 */ s32 field_0x338;
    /* 0x33C */ s32 field_0x33c;
    /* 0x340 */ u16 field_0x340; // beam angle (an index into the sin/cos table)
    /* 0x342 */ u8 field_0x342[2]; // not yet known
    /* 0x344 */ f32 field_0x344; // the beam radius
    /* 0x348 */ f32 field_0x348; // 50.0f after init
    /* 0x34C */ JGeometry::TVec3f field_0x34c; // the beam's top
    // 0x34C-0x37C are separate members, not a TVec3f[4]: the original spawner's inline constructor
    // has no construct-array call for this range, which an array of TVec3f would add.
    /* 0x358 */ JGeometry::TVec3f field_0x358;
    /* 0x364 */ JGeometry::TVec3f field_0x364;
    /* 0x370 */ JGeometry::TVec3f field_0x370;
}; // Size: 0x37C

#endif
