#ifndef GAME_ACT_TU_801688AC_H
#define GAME_ACT_TU_801688AC_H

// Code: game/tu_801688AC.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A0738.
class cls_804A0738 : public ActorBase_804A5770 {
public:
    cls_804A0738() {}
    /* 0x08 */ virtual ~cls_804A0738();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x150]; // not yet known
}; // Size: 0x380

#endif
