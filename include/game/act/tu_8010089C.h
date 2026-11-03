#ifndef GAME_ACT_TU_8010089C_H
#define GAME_ACT_TU_8010089C_H

// Code: game/tu_8010089C.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x8049C8EC.
class cls_8049C8EC : public ActorBase_804A5770 {
public:
    cls_8049C8EC() {}
    /* 0x08 */ virtual ~cls_8049C8EC();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x44 */ virtual void vfunc_0x44();
    /* 0x230 */ u8 field_0x230[0x64]; // not yet known
}; // Size: 0x294

#endif
