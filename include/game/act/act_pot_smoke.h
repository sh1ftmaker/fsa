#ifndef GAME_ACT_ACT_POT_SMOKE_H
#define GAME_ACT_ACT_POT_SMOKE_H

// Code: game/act_pot_smoke.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A8418.
class cls_804A8418 : public ActorBase_804A5770 {
public:
    cls_804A8418() {}
    /* 0x08 */ virtual ~cls_804A8418();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x14]; // not yet known
}; // Size: 0x244

#endif
