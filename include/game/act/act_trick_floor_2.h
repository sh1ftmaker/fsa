#ifndef GAME_ACT_ACT_TRICK_FLOOR_2_H
#define GAME_ACT_ACT_TRICK_FLOOR_2_H

// Code: game/act_trick_floor_2.cpp.

#include "game/ActorBase.h"

// The TFL2 actor ("Trick floor 2", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_801FFA80, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AA628.
class TrickFloor2_804AA628 : public ActorBase_804A5770 {
public:
    TrickFloor2_804AA628() {}
    /* 0x08 */ virtual ~TrickFloor2_804AA628();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230; // cleared by init
    /* 0x234 */ s32 mType; // field_0x90 & 0xF; picks the box and the tile-block counts
    /* 0x238 */ s32 field_0x238; // (field_0x90 >> 4) & 0xF
    /* 0x23C */ s32 field_0x23c; // 1: only players passing fn_80234F68 count; 2: stays down
    /* 0x240 */ s32 field_0x240; // (field_0x90 >> 12) & 0xFFF
    /* 0x244 */ s32 field_0x244; // 20 after init
    /* 0x248 */ f32 field_0x248; // 0, or -0.06 for types 4 and 5
    /* 0x24C */ f32 field_0x24c; // sink speed of states 6 and 7
    /* 0x250 */ u16 field_0x250; // the tile under the floor, read by update's state 0
    /* 0x252 */ s16 field_0x252; // 12 when gGameRoot_8053AAF8->field_0x7f == 1, else 13
}; // Size: 0x254

#endif
