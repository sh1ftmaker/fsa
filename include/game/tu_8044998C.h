#ifndef GAME_TU_8044998C_H
#define GAME_TU_8044998C_H

// Code: game/tu_8044998C.cpp. Named after its vtable, 0x804B1CC8 (only the
// virtual destructor, fn_8044A418, in the slot at 0x8); the constructor is
// fn_8044A4D4, which stores the object in lbl_8053AF20 and builds the
// "mg_fishing.blo" screen. Same shape as cls_804B1C20 (tu_80447618).

#include "types.h"

class J2DScreen;
class cls_804AF86C;

class cls_804B1CC8 {
public:
    cls_804B1CC8();
    virtual ~cls_804B1CC8();

    // Draws the screen: lbl_8053AB10->fn_8013B490(), then mScreen at (0, 0)
    // with lbl_8053AB10's J2DOrthoGraph as the grafcontext.
    void fn_8044A024();
    // Sets the digit pictures to the first 'TIMG' name and clears the pane
    // bytes 0xB0 of 'Ndata01' and 'Ndata00'.
    void fn_8044A06C();
    // Updates the two-digit counters (field_0x10; field_0x11 and field_0x12)
    // and their digit pictures, shows or hides the panes 'Ndata01' and
    // 'Ndata00' by the bits of field_0x13, then clears field_0x13.
    void fn_8044998C();

    /* 0x00 */ // vtable
    /* 0x04 */ J2DScreen* mScreen; // "mg_fishing.blo"
    /* 0x08 */ cls_804AF86C* mPanes[2]; // pane tags 'Ndata00' and 'Ndata01'
    /* 0x10 */ u8 field_0x10; // 0-99 (clamped); tens and units pick the 'Pf' digit pictures
    /* 0x11 */ u8 field_0x11; // 0-99 (clamped); tens and units pick the 'Pt01'..'Pt02' pictures
    /* 0x12 */ u8 field_0x12; // 0-99 (clamped with field_0x11); tens and units pick 'Pt03'/'Pt04'
    /* 0x13 */ u8 field_0x13; // bit 0: show 'Ndata01' digits; bit 1: show 'Ndata00'; cleared by fn_8044998C
}; // Size: 0x14

// The object at lbl_8053AF20 (game/tu_8044998C.cpp, constructor fn_8044A4D4;
// the auto split at 0x8053AF20 defines it).
extern cls_804B1CC8* lbl_8053AF20;

#endif
