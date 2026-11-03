#ifndef GAME_ACT_ACT_TEKTITE_H
#define GAME_ACT_ACT_TEKTITE_H

// Code: game/act_tektite.cpp.

#include "game/ActorBase.h"

// The TKTA actor ("Tektite", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80203C2C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049FD50.
class Tektite_8049FD50 : public ActorBase_804A5770 {
public:
    Tektite_8049FD50() {}
    /* 0x08 */ virtual ~Tektite_8049FD50();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x230 */ u8 field_0x230[0x34]; // not yet known
}; // Size: 0x264

#endif
