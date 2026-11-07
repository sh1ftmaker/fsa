#ifndef GAME_ACT_ACT_FLYING_TILE_H
#define GAME_ACT_ACT_FLYING_TILE_H

// Code: game/act_flying_tile.cpp.

#include "game/ActorBase.h"

// The FLYO actor ("Flying Tile", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80201DEC, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AC100.
class FlyingTile_804AC100 : public ActorBase_804A5770 {
public:
    FlyingTile_804AC100() {}
    /* 0x08 */ virtual ~FlyingTile_804AC100();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // Drops the contents (a Force Gem, fairy or heart, by field_0x260) at the tile's position.
    void fn_80339820();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ s32 field_0x230; // state (0 and 4 count in vfunc_0x10's scan)
    /* 0x234 */ s16 field_0x234;
    /* 0x236 */ u8 field_0x236[2];
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244; // bits 0-10 of the spawn argument
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c; // kind (bits 16-19 of the spawn argument): picks the tile's size and offset
    /* 0x250 */ s32 field_0x250; // bits 11-15 of the spawn argument
    /* 0x254 */ s32 field_0x254; // bits 20-23 of the spawn argument
    /* 0x258 */ u8 field_0x258;
    /* 0x259 */ u8 field_0x259;
    /* 0x25A */ s16 field_0x25a;
    /* 0x25C */ s16 field_0x25c;
    /* 0x25E */ u8 field_0x25e[2];
    /* 0x260 */ s32 field_0x260; // item class (the drop kind fn_80339820 spawns)
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ s32 field_0x270;
    /* 0x274 */ s32 field_0x274;
    /* 0x278 */ u8 field_0x278;
    /* 0x279 */ u8 field_0x279[3];
    /* 0x27C */ f32 field_0x27c;
    /* 0x280 */ f32 field_0x280;
    /* 0x284 */ f32 field_0x284;
    /* 0x288 */ const char* field_0x288; // the stage background name (GameView fn_802D9C4C)
}; // Size: 0x28C

#endif
