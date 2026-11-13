#ifndef GAME_CLS_802157B8_H
#define GAME_CLS_802157B8_H

#include "types.h"

// A flag word, defined in game/tu_802157B8.cpp. No vtable; named after its
// first method. Embedded in the shadow Link actor (act_shadow_link_2, +0x524);
// bits 0x8, 0x10, 0x20 pick the attack.
class cls_802157B8 {
public:
    cls_802157B8() { fn_80215864(); }
    void fn_802157B8(u8);
    void fn_80215864();

    /* 0x0 */ u32 field_0x0;
};

#endif
