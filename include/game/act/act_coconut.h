#ifndef GAME_ACT_ACT_COCONUT_H
#define GAME_ACT_ACT_COCONUT_H

// Code: game/act_coconut.cpp.

#include "game/ActorBase.h"

// The YASI actor ("Coconut", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_802021A4, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A9968.
class Coconut_804A9968 : public ActorBase_804A5770 {
public:
    Coconut_804A9968() {}
    /* 0x08 */ virtual ~Coconut_804A9968();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x48 */ virtual void vfunc_0x48();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ u8 field_0x230[0x8]; // not yet known
}; // Size: 0x238

#endif
