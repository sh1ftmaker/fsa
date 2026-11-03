#ifndef GAME_ACT_ACT_CARROT_H
#define GAME_ACT_ACT_CARROT_H

// Code: game/act_carrot.cpp.

#include "game/ActorBase.h"

// The NNJN actor ("Carrot", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80201414, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AC6B0.
class Carrot_804AC6B0 : public ActorBase_804A5770 {
public:
    Carrot_804AC6B0() {}
    /* 0x08 */ virtual ~Carrot_804AC6B0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ u8 field_0x230[0x44]; // not yet known
}; // Size: 0x274

#endif
