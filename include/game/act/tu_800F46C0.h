#ifndef GAME_ACT_TU_800F46C0_H
#define GAME_ACT_TU_800F46C0_H

// Code: game/tu_800F46C0.cpp.

#include "game/ActorBase.h"

// The IWAG actor ("Boulder spawner", docs/names/actors.tsv, med confidence):
// the spawner constructs it with fn_80202E74, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049C46C.
class BoulderSpawner_8049C46C : public ActorBase_804A5770 {
public:
    BoulderSpawner_8049C46C() {}
    /* 0x08 */ virtual ~BoulderSpawner_8049C46C();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x8]; // not yet known
}; // Size: 0x238

// Named after its vtable, 0x8049C4C0.
class cls_8049C4C0 : public ActorBase_804A5770 {
public:
    cls_8049C4C0() {}
    /* 0x08 */ virtual ~cls_8049C4C0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ u8 field_0x230[0x30]; // not yet known
}; // Size: 0x260

#endif
