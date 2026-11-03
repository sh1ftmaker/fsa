#ifndef GAME_ACT_ACT_BLOCK_H
#define GAME_ACT_ACT_BLOCK_H

// Code: game/act_block.cpp.

#include "game/ActorBase.h"

// The BLCK actor ("Block", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80203FC8, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A7A10.
class Block_804A7A10 : public ActorBase_804A5770 {
public:
    Block_804A7A10() {}
    /* 0x08 */ virtual ~Block_804A7A10();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ u8 field_0x230[0x20]; // not yet known
}; // Size: 0x250

#endif
