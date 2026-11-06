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

    bool fn_8028C364(u8 dir);
    bool fn_8028C518(u8 dir);

    /* 0x230 */ JGeometry::TVec3f field_0x230;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ u8 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ u8 field_0x24c;
    /* 0x24D */ u8 field_0x24d;
    /* 0x24E */ u8 field_0x24e;
    /* 0x24F */ u8 field_0x24f;
}; // Size: 0x250

#endif
