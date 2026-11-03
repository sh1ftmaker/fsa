#ifndef GAME_ACT_ACT_BURNING_BLOCK_H
#define GAME_ACT_ACT_BURNING_BLOCK_H

// Code: game/act_burning_block.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A7B90.
class cls_804A7B90 : public ActorBase_804A5770 {
public:
    cls_804A7B90() {}
    /* 0x08 */ virtual ~cls_804A7B90();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x230 */ u8 field_0x230[0x14]; // not yet known
}; // Size: 0x244

#endif
