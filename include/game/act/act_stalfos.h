#ifndef GAME_ACT_ACT_STALFOS_H
#define GAME_ACT_ACT_STALFOS_H

// Code: game/act_stalfos.cpp.

#include "game/ActorBase.h"

// The STAL actor ("Stalfos", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80202C8C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A0CD8.
class Stalfos_804A0CD8 : public ActorBase_804A5770 {
public:
    Stalfos_804A0CD8() {}
    /* 0x08 */ virtual ~Stalfos_804A0CD8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x54]; // not yet known
}; // Size: 0x284

#endif
