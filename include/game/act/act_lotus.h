#ifndef GAME_ACT_ACT_LOTUS_H
#define GAME_ACT_ACT_LOTUS_H

// Code: game/act_lotus.cpp.

#include "game/ActorBase.h"

// The LOTS actor ("Lotus", docs/names/actors.tsv, med confidence): the spawner
// constructs it with fn_80201C2C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AC330.
class Lotus_804AC330 : public ActorBase_804A5770 {
public:
    Lotus_804AC330() {}
    /* 0x08 */ virtual ~Lotus_804AC330();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ JGeometry::TVec3f field_0x234;
    /* 0x240 */ f32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ u16 field_0x25c;
    /* 0x25E */ u8 field_0x25e[4];
    /* 0x262 */ u8 field_0x262;
}; // Size: 0x264

#endif
