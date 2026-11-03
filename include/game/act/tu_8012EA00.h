#ifndef GAME_ACT_TU_8012EA00_H
#define GAME_ACT_TU_8012EA00_H

// Code: game/tu_8012EA00.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x8049E8F8.
class cls_8049E8F8 : public ActorBase_804A5770 {
public:
    cls_8049E8F8() {}
    /* 0x08 */ virtual ~cls_8049E8F8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x230 */ u8 field_0x230[0x10]; // not yet known
}; // Size: 0x240

#endif
