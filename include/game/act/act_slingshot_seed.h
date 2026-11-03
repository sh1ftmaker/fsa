#ifndef GAME_ACT_ACT_SLINGSHOT_SEED_H
#define GAME_ACT_ACT_SLINGSHOT_SEED_H

// Code: game/act_slingshot_seed.cpp.

#include "game/ActorBase.h"

// The PCDM actor ("Slingshot seed", docs/names/actors.tsv, med confidence):
// the spawner constructs it with fn_80202688, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AB3A8.
class SlingshotSeed_804AB3A8 : public ActorBase_804A5770 {
public:
    SlingshotSeed_804AB3A8() {}
    /* 0x08 */ virtual ~SlingshotSeed_804AB3A8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ u8 field_0x230[0x3C]; // not yet known
    /* 0x26C */ JPABaseEmitter* mEmitter26c; // released by the destructor
    /* 0x270 */ u8 field_0x270[0xC]; // not yet known
}; // Size: 0x27C

#endif
