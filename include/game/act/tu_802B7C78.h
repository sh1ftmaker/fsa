#ifndef GAME_ACT_TU_802B7C78_H
#define GAME_ACT_TU_802B7C78_H

// Code: game/tu_802B7C78.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A886C.
class cls_804A886C : public ActorBase_804A5770 {
public:
    cls_804A886C() {}
    /* 0x08 */ virtual ~cls_804A886C();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x230 */ u8 field_0x230[0x80]; // not yet known
}; // Size: 0x2B0

#endif
