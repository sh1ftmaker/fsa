#ifndef GAME_ACT_ACT_GRASS_H
#define GAME_ACT_ACT_GRASS_H

// Code: game/act_grass.cpp.

#include "game/ActorBase.h"

// The GRAS actor ("Grass", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_802041A8, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A92D0.
class Grass_804A92D0 : public ActorBase_804A5770 {
public:
    Grass_804A92D0() {}
    /* 0x08 */ virtual ~Grass_804A92D0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x48 */ virtual void vfunc_0x48();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ u8 field_0x230[0x18]; // not yet known
}; // Size: 0x248

#endif
