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
    /* 0x230 */ s32 mState;
    /* 0x234 */ s32 field_0x234; // 0 at init
    /* 0x238 */ s32 field_0x238; // 0 at init
    /* 0x23C */ s32 field_0x23c; // -1 at init
    /* 0x240 */ s32 field_0x240; // -1 at init
    /* 0x244 */ u8 field_0x244;  // 0 at init
    /* 0x245 */ u8 field_0x245;  // 0 at init; vfunc_0x4C returns 0 while it is set
    /* 0x246 */ u8 field_0x246;  // 0 at init
    /* 0x247 */ u8 field_0x247;  // 0 at init
    /* 0x248 */ u8 field_0x248;  // 0 at init
    /* 0x249 */ u8 field_0x249;  // 0 at init
    /* 0x24C */ s32 field_0x24c; // draw's sprite frame base (+ 0x2FF); 0 at init
    /* 0x250 */ s32 field_0x250; // 0 at init
    /* 0x254 */ s32 field_0x254; // 0 at init
}; // Size: 0x258

#endif
