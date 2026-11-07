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
    // fn_8030B774 (this unit): run each frame by the owner's update fn_80253E6C.
    void fn_8030B774();
    // Mangled fn_8030AE84__12cls_804AA8D8FPQ29JGeometry8TBox2<f> (this unit).
    // Writes the box of 240x160 around the two position words (0x290, 0x294).
    void fn_8030AE84(JGeometry::TBox2f* out);
    // Code in this unit (mangled fn_8030B97C__12cls_804AA8D8Fv).
    void fn_8030B97C();
    // Mangled fn_8030B084__12cls_804AA8D8FlQ29JGeometry8TVec3<f> (this unit).
    // The position is passed by value: every caller (tu_8023FBF0) copies a
    // whole TVec3f (psq x/y + z) to its stack and passes its address; only
    // x and y are read.
    bool fn_8030B084(s32 index, JGeometry::TVec3f pos);
    // Mangled fn_8030B110__12cls_804AA8D8FlQ29JGeometry8TVec3<f> (this
    // unit). The positive form of fn_8030B084: the player must be active and
    // index must be the one at 0x28C; indices below 8 pass outright, others
    // must lie inside the bounds at 0x2A0-0x2AC. Position by value as above.
    bool fn_8030B110(s32 index, JGeometry::TVec3f pos);
    // Mangled fn_8030AF10__12cls_804AA8D8FlPCQ29JGeometry8TBox2<f>b (this
    // unit). Tests the box against the bounds at 0x290/0x294 (240x160):
    // with overlap set, any part of the box inside them passes; otherwise
    // the whole box must be inside. Index rules as fn_8030B084.
    bool fn_8030AF10(s32 index, const JGeometry::TBox2f* box, bool overlap);
    // Mangled fn_8030B570__12cls_804AA8D8FPlPl. Writes two s32 values through
    // its out pointers and returns an s32 that fn_8030BD3C keeps at 0x2B4.
    s32 fn_8030B570(s32* outA, s32* outB);
    // fn_8030B1E8 (this unit): updates the line shifts for `layer` in
    // `mode` (0 / 6 clear them; 4, 5 and others time them by 0x60, 3, 8 frames)
    // and returns field_0x4 (TileMap_804AA858::fn_802FB69C passes it to fn_802FAC10).
    s32* fn_8030B1E8(u8 layer, u8 mode, u16 scroll);

    // A horizontal shift per screen line (160); fn_8030B1E8 returns it.
    /* 0x004 */ s32 field_0x4[160];
    /* 0x284 */ u8 field_0x284;
    /* 0x285 */ u8 field_0x285[0x288 - 0x285];
    /* 0x288 */ s32 field_0x288;
    /* 0x28C */ s32 field_0x28c;
    /* 0x290 */ s32 field_0x290;
    /* 0x294 */ s32 field_0x294;
    /* 0x298 */ s32 field_0x298;
    /* 0x29C */ s32 field_0x29c;
    /* 0x2A0 */ s32 field_0x2a0; // upper x bound (fn_8030B110)
    /* 0x2A4 */ s32 field_0x2a4; // lower x bound
    /* 0x2A8 */ s32 field_0x2a8; // upper y bound
    /* 0x2AC */ s32 field_0x2ac; // lower y bound

    /* 0x2B0 */ s32 field_0x2b0;
    /* 0x2B4 */ s32 field_0x2b4;
    /* 0x2B8 */ u16 field_0x2b8;
    /* 0x2BA */ s16 field_0x2ba;
    /* 0x2BC */ u8 field_0x2bc;
};

#endif
