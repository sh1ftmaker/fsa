#ifndef GAME_TU_8030AE84_H
#define GAME_TU_8030AE84_H

// Code: game/tu_8030AE84.cpp. The class whose vtable is at 0x804AA8D8 (no
// name or owner is known yet; named after its vtable). Its only virtual
// function so far is the destructor, which the vtable needs to hold 0x10
// bytes.

#include "types.h"
#include "JSystem/JGeometry.h"

class cls_804AA8D8 {
public:
    // Constructor (fn_8030BDF8, now __ct__12cls_804AA8D8Fv): stores the
    // vtable, then sets the fields below to their initial values. Built by
    // a plain `new` in game/tu_8023FBF0.cpp.
    cls_804AA8D8();
    /* 0x08 */ virtual ~cls_804AA8D8();

    // Mangled fn_8030BD3C__12cls_804AA8D8Fl (game/tu_8030AE84.cpp): stores the
    // player, then the callees below.
    void fn_8030BD3C(s32 player);
    // Mangled fn_8030AE84__12cls_804AA8D8FPQ29JGeometry8TBox2<f> (this unit).
    // Writes the box of 240x160 around the two position words (0x290, 0x294).
    void fn_8030AE84(JGeometry::TBox2f* out);
    // Code in this unit, not yet written (mangled fn_8030B97C__12cls_804AA8D8Fv).
    void fn_8030B97C();
    // Mangled fn_8030B084__12cls_804AA8D8FlPCQ29JGeometry8TVec2<f> (this unit).
    // Called from game/tu_8023FBF0.cpp with the position copied to the stack
    // as an 8-byte x/y pair, so it takes a TVec2f pointer.
    bool fn_8030B084(s32 index, const JGeometry::TVec2f* pos);
    // Mangled fn_8030B570__12cls_804AA8D8FPlPl. Writes two s32 values through
    // its out pointers and returns an s32 that fn_8030BD3C keeps at 0x2B4.
    s32 fn_8030B570(s32* outA, s32* outB);

    /* 0x004 */ u8 field_0x4[0x284 - 0x4];
    /* 0x284 */ u8 field_0x284;
    /* 0x285 */ u8 field_0x285[0x288 - 0x285];
    /* 0x288 */ s32 field_0x288;
    /* 0x28C */ s32 field_0x28c;
    /* 0x290 */ s32 field_0x290;
    /* 0x294 */ s32 field_0x294;
    /* 0x298 */ s32 field_0x298;
    /* 0x29C */ s32 field_0x29c;
    /* 0x2A0 */ u8 field_0x2a0[0x2B0 - 0x2A0];
    /* 0x2B0 */ s32 field_0x2b0;
    /* 0x2B4 */ s32 field_0x2b4;
    /* 0x2B8 */ u16 field_0x2b8;
    /* 0x2BA */ s16 field_0x2ba;
    /* 0x2BC */ u8 field_0x2bc;
};

#endif
