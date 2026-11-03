#ifndef GAME_ACT_TU_80184D64_H
#define GAME_ACT_TU_80184D64_H

// Code: game/tu_80184D64.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A18E0.
class cls_804A18E0 : public ActorBase_804A5770 {
public:
    cls_804A18E0() {}
    /* 0x08 */ virtual ~cls_804A18E0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x160]; // not yet known
}; // Size: 0x390

#endif
