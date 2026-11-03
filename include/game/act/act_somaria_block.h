#ifndef GAME_ACT_ACT_SOMARIA_BLOCK_H
#define GAME_ACT_ACT_SOMARIA_BLOCK_H

// Code: game/act_somaria_block.cpp.

#include "game/ActorBase.h"

// The SMRI actor ("Cane of Somaria block", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_80200B78, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AA9F0.
class CaneOfSomariaBlock_804AA9F0 : public ActorBase_804A5770 {
public:
    CaneOfSomariaBlock_804AA9F0() {}
    /* 0x08 */ virtual ~CaneOfSomariaBlock_804AA9F0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ u8 field_0x230[0x28]; // not yet known
}; // Size: 0x258

#endif
