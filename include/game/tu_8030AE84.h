#ifndef GAME_TU_8030AE84_H
#define GAME_TU_8030AE84_H

// Code: game/tu_8030AE84.cpp. The class whose vtable is at 0x804AA8D8 (no
// name or owner is known yet; named after its vtable). Its only virtual
// function so far is the destructor, which the vtable needs to hold 0x10
// bytes.

#include "types.h"

class cls_804AA8D8 {
public:
    // Constructor (fn_8030BDF8, now __ct__12cls_804AA8D8Fv): stores the
    // vtable, then sets the fields below to their initial values. Built by
    // a plain `new` in game/tu_8023FBF0.cpp.
    cls_804AA8D8();
    /* 0x08 */ virtual ~cls_804AA8D8();

    /* 0x004 */ u8 field_0x4[0x284 - 0x4];
    /* 0x284 */ u8 field_0x284;
    /* 0x285 */ u8 field_0x285[0x288 - 0x285];
    /* 0x288 */ s32 field_0x288;
    /* 0x28C */ s32 field_0x28c;
    /* 0x290 */ u8 field_0x290[0x2B0 - 0x290];
    /* 0x2B0 */ s32 field_0x2b0;
    /* 0x2B4 */ s32 field_0x2b4;
    /* 0x2B8 */ u16 field_0x2b8;
    /* 0x2BA */ s16 field_0x2ba;
    /* 0x2BC */ u8 field_0x2bc;
};

#endif
