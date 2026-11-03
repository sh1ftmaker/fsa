#ifndef GAME_ACT_ACT_FAIRY_H
#define GAME_ACT_ACT_FAIRY_H

// Code: game/act_fairy.cpp.

#include "game/ActorBase.h"

// The FARY actor ("Fairy", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_802039D4, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A03B8.
class Fairy_804A03B8 : public ActorBase_804A5770 {
public:
    Fairy_804A03B8() {}
    /* 0x08 */ virtual ~Fairy_804A03B8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x44 */ virtual void vfunc_0x44();
    /* 0x230 */ u8 field_0x230[0x60]; // not yet known
    /* 0x290 */ JPABaseEmitter* mEmitter290; // released by the destructor
    /* 0x294 */ JPABaseEmitter* mEmitter294; // released by the destructor
}; // Size: 0x298

#endif
