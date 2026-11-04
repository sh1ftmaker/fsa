#ifndef GAME_ACT_ACT_GIBDO_H
#define GAME_ACT_ACT_GIBDO_H

// Code: game/act_gibdo.cpp.

#include "game/EnemyBase.h"

// The MIRA actor ("Gibdo", docs/names/actors.tsv, med confidence): the spawner
// constructs it with fn_801BA3FC, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A3BF8.
class Gibdo_804A3BF8 : public EnemyBase_804A4808 {
public:
    // Out of line in act_gibdo (0x801BA3FC), which the spawner calls.
    Gibdo_804A3BF8();
    /* 0x08 */ virtual ~Gibdo_804A3BF8();
    /* 0x0C */ virtual void init();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);
    /* 0x98 */ virtual void vfunc_0x98();
    /* 0x9C */ virtual void vfunc_0x9C(u8 mapHit);

    // Out of line in act_gibdo (0x801B9DD4), no other unit calls it: the
    // state change that vfunc_0x24 and vfunc_0x90 make. The argument is the
    // step's mode (0, 1, 2 or 3).
    void fn_801B9DD4(s32 mode);
}; // Size: 0x2A8

#endif
