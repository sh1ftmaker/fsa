#ifndef GAME_ACT_TU_800FF060_H
#define GAME_ACT_TU_800FF060_H

// Code: game/tu_800FF060.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x8049C858.
class cls_8049C858 : public ActorBase_804A5770 {
public:
    cls_8049C858() {}
    /* 0x08 */ virtual ~cls_8049C858();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x7C]; // not yet known
}; // Size: 0x2AC

#endif
