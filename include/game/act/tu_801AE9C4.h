#ifndef GAME_ACT_TU_801AE9C4_H
#define GAME_ACT_TU_801AE9C4_H

// Code: game/tu_801AE9C4.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A3448.
class cls_804A3448 : public ActorBase_804A5770 {
public:
    cls_804A3448() {}
    /* 0x08 */ virtual ~cls_804A3448();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x54]; // not yet known
}; // Size: 0x284

#endif
