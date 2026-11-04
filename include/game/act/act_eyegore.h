#ifndef GAME_ACT_ACT_EYEGORE_H
#define GAME_ACT_ACT_EYEGORE_H

// Code: game/act_eyegore.cpp.

#include "game/ActorBase.h"

// The EGOL actor ("Eyegore", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80200870, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A3548.
class Eyegore_804A3548 : public ActorBase_804A5770 {
public:
    Eyegore_804A3548() {}
    /* 0x08 */ virtual ~Eyegore_804A3548();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x44 */ virtual void vfunc_0x44();
    /* 0x230 */ s32 field_0x230;      // state (update's switch); init and vfunc_0x44 zero it
    // Countdowns, each stepped down by one per update(); update() and
    // vfunc_0x44 set them. init zeroes all four.
    /* 0x234 */ s32 mTimers[4];
    /* 0x244 */ s32 field_0x244;      // zeroed by init and vfunc_0x44
    /* 0x248 */ s32 field_0x248;      // zeroed by init
    /* 0x24C */ s32 field_0x24c;      // zeroed by init
    /* 0x250 */ s32 field_0x250;      // init sets 0x12; vfunc_0x44 sets field_0x98 + 0x10
    /* 0x254 */ s32 field_0x254;      // init copies mLife here; vfunc_0x44 tracks mLife
    /* 0x258 */ s32 field_0x258;      // init sets 2; counts down in vfunc_0x44
    /* 0x25C */ JGeometry::TVec3f field_0x25c; // velocity per frame; zeroed by init
}; // Size: 0x268

#endif
