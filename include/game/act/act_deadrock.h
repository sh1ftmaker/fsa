#ifndef GAME_ACT_ACT_DEADROCK_H
#define GAME_ACT_ACT_DEADROCK_H

// Code: game/act_deadrock.cpp.

#include "game/ActorBase.h"

// The DDRK actor ("Deadrock", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80203F50, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049F498.
class Deadrock_8049F498 : public ActorBase_804A5770 {
public:
    Deadrock_8049F498() {}
    /* 0x08 */ virtual ~Deadrock_8049F498();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230;      // state: init sets 0; draw tests 3 and 4
    /* 0x234 */ s32 field_0x234;      // zeroed by init
    /* 0x238 */ s32 field_0x238;      // zeroed by init
    /* 0x23C */ s32 field_0x23c;      // zeroed by init; draw tests bit 0 in state 4
    /* 0x240 */ s32 field_0x240;      // zeroed by init
    /* 0x244 */ s32 field_0x244;      // zeroed by init; draw's frame base
    /* 0x248 */ s32 field_0x248;      // zeroed by init
    /* 0x24C */ s32 field_0x24c;      // zeroed by init
    /* 0x250 */ s32 field_0x250;      // set to 1 by init
    /* 0x254 */ s32 field_0x254;      // not yet known
}; // Size: 0x258

#endif
