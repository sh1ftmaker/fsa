#ifndef GAME_ACT_TU_802E6EC4_H
#define GAME_ACT_TU_802E6EC4_H

// Code: game/tu_802E6EC4.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A9E50.
class cls_804A9E50 : public ActorBase_804A5770 {
public:
    cls_804A9E50() {}
    /* 0x08 */ virtual ~cls_804A9E50();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x14]; // not yet known
}; // Size: 0x244

#endif
