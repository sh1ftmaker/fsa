#ifndef GAME_ACT_ACT_CACTUS_H
#define GAME_ACT_ACT_CACTUS_H

// Code: game/act_cactus.cpp.

#include "game/ActorBase.h"

// The SBTN actor ("Cactus", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80202390, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AB580.
class Cactus_804AB580 : public ActorBase_804A5770 {
public:
    Cactus_804AB580() {}
    /* 0x08 */ virtual ~Cactus_804AB580();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230; // 0 idle, 1 hit (counts field_0x23c up to 7)
    /* 0x234 */ s32 field_0x234; // cleared by init, never read here
    /* 0x238 */ s32 field_0x238; // cleared by init, never read here
    /* 0x23C */ s32 field_0x23c; // hit animation frame, indexes lbl_804718E8
}; // Size: 0x240

#endif
