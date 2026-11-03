#ifndef GAME_ACT_ACT_TERRORPIN_H
#define GAME_ACT_ACT_TERRORPIN_H

// Code: game/act_terrorpin.cpp.

#include "game/EnemyBase.h"

// The PAME actor ("Terrorpin", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_801B94C8, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A3B10.
class Terrorpin_804A3B10 : public EnemyBase_804A4808 {
public:
    Terrorpin_804A3B10() {}
    /* 0x08 */ virtual ~Terrorpin_804A3B10();
    /* 0x0C */ virtual void init();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x44 */ virtual void vfunc_0x44();
    /* 0x5C */ virtual bool vfunc_0x5C();
    /* 0x60 */ virtual bool vfunc_0x60();
    /* 0x68 */ virtual u8 vfunc_0x68();
    /* 0x6C */ virtual void vfunc_0x6C();
    /* 0x7C */ virtual void vfunc_0x7C();
    /* 0x88 */ virtual void vfunc_0x88();
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);
    /* 0x94 */ virtual void vfunc_0x94();
    /* 0x2A8 */ u8 field_0x2a8[0x4]; // not yet known
}; // Size: 0x2AC

#endif
