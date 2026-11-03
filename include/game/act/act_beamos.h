#ifndef GAME_ACT_ACT_BEAMOS_H
#define GAME_ACT_ACT_BEAMOS_H

// Code: game/act_beamos.cpp.

#include "game/EnemyBase.h"

// The BMST actor ("Beamos", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_801BEB44, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A40E0.
class Beamos_804A40E0 : public EnemyBase_804A4808 {
public:
    Beamos_804A40E0() {}
    /* 0x08 */ virtual ~Beamos_804A40E0();
    /* 0x0C */ virtual void init();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x5C */ virtual bool vfunc_0x5C();
    /* 0x60 */ virtual bool vfunc_0x60();
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);
    /* 0x98 */ virtual void vfunc_0x98();
    /* 0x9C */ virtual void vfunc_0x9C(u8 mapHit);
    /* 0x2A8 */ u8 field_0x2a8[0x10]; // not yet known
}; // Size: 0x2B8

#endif
