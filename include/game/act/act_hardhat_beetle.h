#ifndef GAME_ACT_ACT_HARDHAT_BEETLE_H
#define GAME_ACT_ACT_HARDHAT_BEETLE_H

// Code: game/act_hardhat_beetle.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A06C0.
class cls_804A06C0 : public ActorBase_804A5770 {
public:
    cls_804A06C0() {}
    /* 0x08 */ virtual ~cls_804A06C0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x54]; // not yet known
}; // Size: 0x284

#endif
