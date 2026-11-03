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
    /* 0x230 */ u8 field_0x230[0x38]; // not yet known
}; // Size: 0x268

#endif
