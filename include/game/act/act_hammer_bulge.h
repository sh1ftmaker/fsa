#ifndef GAME_ACT_ACT_HAMMER_BULGE_H
#define GAME_ACT_ACT_HAMMER_BULGE_H

// Code: game/act_hammer_bulge.cpp.

#include "game/ActorBase.h"

// The SWEL actor ("Hammer bulge", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80203108, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A88E8.
class HammerBulge_804A88E8 : public ActorBase_804A5770 {
public:
    HammerBulge_804A88E8() {}
    /* 0x08 */ virtual ~HammerBulge_804A88E8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23c */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ f32 field_0x244;
    /* 0x248 */ f32 field_0x248;
    /* 0x24c */ u8 field_0x24c;
    /* 0x24d */ u8 field_0x24d;
    /* 0x24e */ u8 field_0x24e[2];
}; // Size: 0x250

#endif
