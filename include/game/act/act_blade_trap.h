#ifndef GAME_ACT_ACT_BLADE_TRAP_H
#define GAME_ACT_ACT_BLADE_TRAP_H

// Code: game/act_blade_trap.cpp.

#include "game/ActorBase.h"

// The TRAP actor ("Blade Trap", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_8020382C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A04D8.
class BladeTrap_804A04D8 : public ActorBase_804A5770 {
public:
    BladeTrap_804A04D8() {}
    /* 0x08 */ virtual ~BladeTrap_804A04D8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ u8 field_0x230[0x50]; // not yet known
}; // Size: 0x280

#endif
