#ifndef GAME_CLS_802150B8_H
#define GAME_CLS_802150B8_H

#include "types.h"
#include "game/cls_804A64A8.h"

// A struct with no vtable, named after its constructor (fn_802150B8,
// game/tu_802144F0.cpp). It holds two particle callbacks (cls_804A64A8) at
// 0x04 and 0x1C and is embedded in act_shadow_link_2 at +0x4EC (the
// constructor is called from fn_801C573C).
// Size: 0x38.
class cls_802150B8 {
public:
    cls_802150B8();

    /* 0x00 */ u32 field_0x0;
    /* 0x04 */ cls_804A64A8 field_0x4;
    /* 0x18 */ u32 field_0x18;
    /* 0x1C */ cls_804A64A8 field_0x1c;
    /* 0x30 */ s32 field_0x30;
    /* 0x34 */ s32 field_0x34;
}; // Size: 0x38

#endif
