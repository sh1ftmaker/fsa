#ifndef GAME_ACT_ACT_MAGIC_CIRCLE_H
#define GAME_ACT_ACT_MAGIC_CIRCLE_H

// Code: game/act_magic_circle.cpp.

#include "game/ActorBase.h"

// The CIRC actor ("Magic circle", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_802023CC, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A7960.
class MagicCircle_804A7960 : public ActorBase_804A5770 {
public:
    MagicCircle_804A7960() {}
    /* 0x08 */ virtual ~MagicCircle_804A7960();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x44]; // not yet known
    /* 0x274 */ JPABaseEmitter* mEmitter274; // released by the destructor
    /* 0x278 */ JPABaseEmitter* mEmitter278; // released by the destructor
}; // Size: 0x27C

#endif
