#ifndef GAME_ACT_ACT_ARMOS_H
#define GAME_ACT_ACT_ARMOS_H

// Code: game/act_armos.cpp.

#include "game/ActorBase.h"

// The AMOS actor ("Armos", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80204220, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049F260.
class Armos_8049F260 : public ActorBase_804A5770 {
public:
    Armos_8049F260() {}
    /* 0x08 */ virtual ~Armos_8049F260();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0xC8]; // not yet known
    /* 0x2F8 */ s32 field_0x2f8; // init: 3, or 0 / 4 by field_0x94
    /* 0x2FC */ s32 field_0x2fc; // a countdown
    /* 0x300 */ s32 field_0x300;
    /* 0x304 */ s32 field_0x304;
    /* 0x308 */ s32 field_0x308; // init: spawn parameter bits 8-15
    /* 0x30C */ s32 field_0x30c;
    /* 0x310 */ s32 field_0x310;
    /* 0x314 */ s32 field_0x314;
    /* 0x318 */ s32 field_0x318; // draw: added to the frame
    /* 0x31C */ s32 field_0x31c; // init: spawn parameter bit 4 (draw: the bigger sprite)
    // Plain Vec (not TVec3f): see act_cucco.h on constructor calls in inline constructors.
    /* 0x320 */ Vec field_0x320; // the last grid-snapped position
}; // Size: 0x32C

#endif
