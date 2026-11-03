#ifndef GAME_ACT_ACT_GRAVESTONE_H
#define GAME_ACT_ACT_GRAVESTONE_H

// Code: game/act_gravestone.cpp.

#include "game/ActorBase.h"

// The GRVE actor ("Gravestone", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80202168, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A99C0.
class Gravestone_804A99C0 : public ActorBase_804A5770 {
public:
    Gravestone_804A99C0() {}
    /* 0x08 */ virtual ~Gravestone_804A99C0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230; // state: 0 idle, 1 player standing on it, 2 tile rewrite, 3 falling, 4 done
    /* 0x234 */ s32 field_0x234; // field_0x90 & 0x3F: index into the tile table (update case 2)
    /* 0x238 */ s32 field_0x238; // the player on it, -1 for none
    /* 0x23C */ f32 field_0x23c; // frame counter
    /* 0x240 */ JGeometry::TVec3f field_0x240; // mPos when the rewrite starts
}; // Size: 0x24C

#endif
