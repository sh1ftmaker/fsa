#ifndef GAME_ACT_ACT_FISH_H
#define GAME_ACT_ACT_FISH_H

// Code: game/act_fish.cpp.

#include "game/ActorBase.h"

// The FISH actor ("Fish", docs/names/actors.tsv, med confidence): the spawner
// constructs it with fn_80201504, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AC658.
class Fish_804AC658 : public ActorBase_804A5770 {
public:
    Fish_804AC658() {}
    /* 0x08 */ virtual ~Fish_804AC658();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    // 0x80346330: finds the player the fish swims away from (field_0x270, with its heading in
    // field_0x268 / field_0x26c and distance in field_0x264); true when there is one.
    bool fn_80346330();
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c; // sprite frame offset
    /* 0x250 */ s16 field_0x250;
    /* 0x252 */ u16 field_0x252; // heading angle (draw)
    /* 0x254 */ f32 field_0x254;
    /* 0x258 */ f32 field_0x258;
    /* 0x25C */ f32 field_0x25c;
    /* 0x260 */ f32 field_0x260;
    /* 0x264 */ f32 field_0x264;
    /* 0x268 */ u16 field_0x268;
    /* 0x26A */ u8 pad_0x26a[2];
    /* 0x26C */ s32 field_0x26c; // -1 at init
    /* 0x270 */ s32 field_0x270; // -1 at init
}; // Size: 0x274

#endif
