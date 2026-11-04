#ifndef GAME_ACT_ACT_ICE_WALL_H
#define GAME_ACT_ACT_ICE_WALL_H

// Code: game/act_ice_wall.cpp.

#include "game/ActorBase.h"
#include "game/SpriteAnim.h"

// The ICWL actor ("Ice wall", docs/names/actors.tsv, med confidence): the
// spawner constructs it with its constructor (0x8011E6E0, game_units.tsv).
// Named after its vtable, 0x8049DE18.
class IceWall_8049DE18 : public ActorBase_804A5770 {
public:
    IceWall_8049DE18();
    /* 0x08 */ virtual ~IceWall_8049DE18();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238; // (field_0x90 >> 4) & 0xF
    /* 0x23C */ s32 field_0x23c; // (field_0x90 >> 8) & 0xFF
    /* 0x240 */ f32 field_0x240;
    /* 0x244 */ SpriteAnim_801D267C mSprite;
    // The emitter the destructor releases with fn_801EE434.
    /* 0x27C */ JPABaseEmitter* field_0x27c;
}; // Size: 0x280

#endif
