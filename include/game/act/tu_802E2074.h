#ifndef GAME_ACT_TU_802E2074_H
#define GAME_ACT_TU_802E2074_H

// Code: game/tu_802E2074.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A9BC8.
class cls_804A9BC8 : public ActorBase_804A5770 {
public:
    cls_804A9BC8() {}
    /* 0x08 */ virtual ~cls_804A9BC8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x20]; // not yet known
}; // Size: 0x250

#endif
