#ifndef GAME_ACT_ACT_POE_H
#define GAME_ACT_ACT_POE_H

// Code: game/act_poe.cpp.

#include "game/ActorBase.h"

// The POOH actor ("Poe", docs/names/actors.tsv, med confidence): the spawner
// constructs it with fn_802041E4, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049F330.
class Poe_8049F330 : public ActorBase_804A5770 {
public:
    Poe_8049F330() {}
    /* 0x08 */ virtual ~Poe_8049F330();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230; // draw() draws only for 0 or 1
    /* 0x234 */ s32 field_0x234;
    // Nine counters, zeroed by init(). [7] is tested by draw(); [8] picks a
    // column of lbl_8049F2B8 in init().
    /* 0x238 */ s32 field_0x238[9];
    /* 0x25C */ s32 field_0x25c; // draw(): a frame index, offset by 0x84 or 6
    /* 0x260 */ JGeometry::TVec3f field_0x260; // a copy of mPos, made by init()
    /* 0x26C */ u16 field_0x26c; // an angle: init() takes its sin and cos
    /* 0x26E */ u8 field_0x26e[2]; // not yet known
    /* 0x270 */ f32 field_0x270; // draw(): byte 0x13 of the sprite parameters
    /* 0x274 */ u8 field_0x274[0x278 - 0x274]; // not yet known
}; // Size: 0x278

#endif
