#ifndef GAME_ACT_TU_80376FA0_H
#define GAME_ACT_TU_80376FA0_H

// Code: game/tu_80376FA0.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AE7D8.
class cls_804AE7D8 : public ActorBase_804A5770 {
public:
    cls_804AE7D8() {}
    /* 0x08 */ virtual ~cls_804AE7D8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x38]; // not yet known
}; // Size: 0x268

#endif
