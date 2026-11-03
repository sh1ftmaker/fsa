#ifndef GAME_ACT_TU_803DA6B4_H
#define GAME_ACT_TU_803DA6B4_H

// Code: game/tu_803DA6B4.cpp.

#include "types.h"

class J2DScreen;
class J2DPane;

// A pulsing on-screen overlay built from a J2DScreen. Named after its vtable,
// 0x804B04F8 (only the destructor, fn_803DA878); the constructor is
// fn_803DA8FC, which stores the object in lbl_8053ADC8. 7 call sites of
// fn_803DA6B4 in tu_80134C04 (the main loop), which sets field_0x20 and
// field_0x21 through lbl_8053ADC8 (act_battle_royale_end_check's draw
// writes 1 and 3).
class cls_804B04F8 {
public:
    cls_804B04F8();
    virtual ~cls_804B04F8();

    // Draws the screen when field_0x20 is set (and clears it), and advances
    // the pulse timer field_0x1c; called with lbl_8053ADC8 in r3.
    void fn_803DA6B4();

    /* 0x00 */ // vtable
    /* 0x04 */ J2DScreen* mScreen;
    /* 0x08 */ J2DPane* mPane1;   // search("prsstrt1")
    /* 0x0C */ J2DPane* mPane2;   // search("prsstrt2")
    /* 0x10 */ f32 field_0x10;    // mPane2's bounds x
    /* 0x14 */ f32 field_0x14;    // mPane2's bounds y
    /* 0x18 */ f32 field_0x18;    // pulse scale
    /* 0x1C */ s16 field_0x1c;    // pulse timer
    /* 0x1E */ u8 field_0x1e;     // mPane1's alpha
    /* 0x1F */ u8 field_0x1f;     // mPane2's alpha
    /* 0x20 */ u8 field_0x20;
    /* 0x21 */ u8 field_0x21;
    /* 0x22 */ u8 pad_0x22[2];
}; // Size: 0x24

extern cls_804B04F8* lbl_8053ADC8;

#endif
