#ifndef GAME_ACT_ACT_BURNING_BLOCK_H
#define GAME_ACT_ACT_BURNING_BLOCK_H

// Code: game/act_burning_block.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A7B90.
class BurningBlockWoodenCrate_804A7B90 : public ActorBase_804A5770 {
public:
    BurningBlockWoodenCrate_804A7B90() {}
    /* 0x08 */ virtual ~BurningBlockWoodenCrate_804A7B90();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x230 */ JGeometry::TVec3f field_0x230; // mPos at the last grid snap
    /* 0x23C */ s32 field_0x23c;               // frames to wait after a push (8)
    /* 0x240 */ u16 field_0x240;               // tile code under the block (fn_80226850)
    // 0x242..0x244: padding
}; // Size: 0x244

#endif
