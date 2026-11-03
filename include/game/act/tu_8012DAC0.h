#ifndef GAME_ACT_TU_8012DAC0_H
#define GAME_ACT_TU_8012DAC0_H

// Code: game/tu_8012DAC0.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x8049E848.
class cls_8049E848 : public ActorBase_804A5770 {
public:
    cls_8049E848() {}
    /* 0x08 */ virtual ~cls_8049E848();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x230 */ u8 field_0x230[0x18]; // not yet known
}; // Size: 0x248

#endif
