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
    void fn_8030EA00();
    bool fn_8030EA0C() const;
    // Inline (no symbol; three copies in update): the tile under the centre of the 0x80 box;
    // on a pit / water tile (3, 0xD, 0xE, 0x3B) it stops the block, sets mState 6 (sinking) and
    // records the tile in field_0x23c, returning false.
    bool checkGround();
    /* 0x230 */ s32 mState;
    /* 0x234 */ s32 field_0x234; // 0 at init
    /* 0x238 */ s32 field_0x238; // 0 at init
    /* 0x23C */ s32 field_0x23c; // -1 at init
    /* 0x240 */ s32 field_0x240; // -1 at init
    /* 0x244 */ u8 field_0x244;  // 0 at init
    /* 0x245 */ u8 field_0x245;  // 0 at init; vfunc_0x4C returns 0 while it is set
    // Per player (update indexes it, lbzx): set once fn_8023B0F4 reports the player clear of
    // the block's box; 0 at init.
    /* 0x246 */ u8 field_0x246[4];
    /* 0x24C */ s32 field_0x24c; // draw's sprite frame base (+ 0x2FF); 0 at init
    /* 0x250 */ s32 field_0x250; // 0 at init
    /* 0x254 */ s32 field_0x254; // 0 at init
}; // Size: 0x258

#endif
