#ifndef GAME_ACT_TU_803489A4_H
#define GAME_ACT_TU_803489A4_H

// Code: game/tu_803489A4.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AC7B8.
class BeeFormation_804AC7B8 : public ActorBase_804A5770 {
public:
    BeeFormation_804AC7B8() {}
    /* 0x08 */ virtual ~BeeFormation_804AC7B8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x74]; // not yet known
}; // Size: 0x2A4

#endif
