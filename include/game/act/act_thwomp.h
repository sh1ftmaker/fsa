#ifndef GAME_ACT_ACT_THWOMP_H
#define GAME_ACT_ACT_THWOMP_H

// Code: game/act_thwomp.cpp.

#include "game/ActorBase.h"

// The DSIN actor ("Thwomp", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_802022E0, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AB5D8.
class Thwomp_804AB5D8 : public ActorBase_804A5770 {
public:
    Thwomp_804AB5D8() {}
    /* 0x08 */ virtual ~Thwomp_804AB5D8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230; // state: 0 waiting, 1 falling, 2 landed, 3 rising
    /* 0x234 */ s32 field_0x234; // frames left in state 2
    /* 0x238 */ u8 field_0x238[4]; // per player: fn_80239718 result
    /* 0x23C */ JGeometry::TVec3f field_0x23c; // spawn position (init copies mPos)
}; // Size: 0x248

#endif
