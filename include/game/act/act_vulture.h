#ifndef GAME_ACT_ACT_VULTURE_H
#define GAME_ACT_ACT_VULTURE_H

// Code: game/act_vulture.cpp.

#include "game/EnemyBase.h"

// The TNDR actor ("Vulture", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_801C0AD8, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A4230.
class Vulture_804A4230 : public EnemyBase_804A4808 {
public:
    Vulture_804A4230() {}
    /* 0x08 */ virtual ~Vulture_804A4230();
    /* 0x0C */ virtual void init();
    /* 0x20 */ virtual void draw();
    /* 0x58 */ virtual bool vfunc_0x58();
    /* 0x5C */ virtual bool vfunc_0x5C();
    /* 0x60 */ virtual bool vfunc_0x60();
    /* 0x74 */ virtual void vfunc_0x74();
    /* 0x88 */ virtual void vfunc_0x88();
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);
    /* 0x94 */ virtual void vfunc_0x94();
    /* 0x2A8 */ u8 field_0x2a8[0x28]; // not yet known
}; // Size: 0x2D0

#endif
