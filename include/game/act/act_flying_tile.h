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
    /* 0x234 */ u8 field_0x234[0x244 - 0x234]; // not yet known
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c; // type: 1 and 5 select how field_0x260 is remapped in fn_80339820
    /* 0x250 */ u8 field_0x250[0x260 - 0x250]; // not yet known
    /* 0x260 */ s32 field_0x260; // item class (the drop kind fn_80339820 spawns)
    /* 0x264 */ u8 field_0x264[0x26C - 0x264]; // not yet known
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ u8 field_0x270[4];
    /* 0x274 */ s32 field_0x274;
    /* 0x278 */ u8 field_0x278;
    /* 0x279 */ u8 field_0x279[0x28C - 0x279]; // not yet known
}; // Size: 0x28C

#endif
