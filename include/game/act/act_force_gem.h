#ifndef GAME_ACT_ACT_FORCE_GEM_H
#define GAME_ACT_ACT_FORCE_GEM_H

// Code: game/act_force_gem.cpp.

#include "game/ActorBase.h"

// The RUPY actor ("Force Gem", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80204298, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AB6D8.
class ForceGem_804AB6D8 : public ActorBase_804A5770 {
public:
    ForceGem_804AB6D8() {}
    /* 0x08 */ virtual ~ForceGem_804AB6D8();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x40 */ virtual void vfunc_0x40();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    // 0x8032ABE0: lets a player at or near it pick it up. 0x8032B0A4: reacts to the tile
    // under it (water, pit); returns whether it handled the tile.
    void fn_8032ABE0();
    bool fn_8032B0A4();
    // 0x8032B3F8: the fall and bounce; returns false when a tile reaction took over.
    bool fn_8032B3F8();
    // 0x8032B7C0: a player nearby kicks it away.
    void fn_8032B7C0();
    // 0x8032BB90: a player slashing it (Camera test on screen) turns it into a flying gem.
    void fn_8032BB90();
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ u8 field_0x234[4];
    /* 0x238 */ f32 field_0x238;
    /* 0x23C */ u8 field_0x23c[0x248 - 0x23C];
    /* 0x248 */ s32 field_0x248; // nonzero: a fleeing gem (ESRY)
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ u8 field_0x250[0x10];
    /* 0x260 */ s32 field_0x260; // set once it has bounced
    /* 0x264 */ s32 field_0x264; // the player that picked it up
    /* 0x268 */ s32 field_0x268; // kind: indexes the value table
    /* 0x26C */ u8 field_0x26c[8];
    /* 0x274 */ s32 field_0x274;
    /* 0x278 */ u8 field_0x278[5];
    /* 0x27D */ u8 field_0x27d;
    /* 0x27E */ u8 field_0x27e;
    /* 0x27F */ u8 field_0x27f;
    /* 0x280 */ u8 field_0x280; // cleared once vfunc_0x34 reports done (vfunc_0x10)
    /* 0x281 */ u8 field_0x281;
    /* 0x282 */ u8 field_0x282;
    /* 0x283 */ u8 field_0x283;
    /* 0x284 */ JGeometry::TVec3f field_0x284; // out-vector of fn_801F2618
    /* 0x290 */ u8 field_0x290[0x29C - 0x290];
    /* 0x29C */ JGeometry::TVec3f field_0x29c;
}; // Size: 0x2A8

#endif
