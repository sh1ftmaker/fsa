#ifndef GAME_ACT_ACT_STRANGE_BLOCK_H
#define GAME_ACT_ACT_STRANGE_BLOCK_H

// Code: game/act_strange_block.cpp.

#include "game/ActorBase.h"

class JPABaseEmitter;

// The STBL actor ("Strange block", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80201C68, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AC2D8.
class StrangeBlock_804AC2D8 : public ActorBase_804A5770 {
public:
    StrangeBlock_804AC2D8() {}
    /* 0x08 */ virtual ~StrangeBlock_804AC2D8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // 0x8033D264: pushes the block one step in direction `dir` (0-3); returns whether it can move.
    bool fn_8033D264(u8 dir);
    // Code: game/act_strange_block.cpp.
    void fn_8033D4CC();
    /* 0x230 */ JPABaseEmitter* field_0x230; // 0 from init; the emitter of a kind-4 push (fn_8033D4CC)
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ f32 field_0x238[3]; // copy of mPos made by init
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c; // kind: bits 12-15 of field_0x90; act_small_key bounces off it in 0 and 2
    /* 0x260 */ s32 field_0x260; // size (2 or 4)
    /* 0x264 */ u8 field_0x264; // bit 17 of field_0x90
    /* 0x265 */ u8 field_0x265;
    /* 0x266 */ u8 field_0x266[2]; // not yet known
    /* 0x268 */ s32 field_0x268; // (field_0x90 & 0xFFF) + 0x12B
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ u8 field_0x270;
    /* 0x271 */ u8 field_0x271;
    /* 0x272 */ u8 field_0x272;
    /* 0x273 */ u8 field_0x273; // not yet known
}; // Size: 0x274

#endif
