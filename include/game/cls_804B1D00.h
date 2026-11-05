#ifndef GAME_CLS_804B1D00_H
#define GAME_CLS_804B1D00_H

// Code: game/tu_8044A828.cpp. Named after its vtable, 0x804B1D00 (only the
// virtual destructor, fn_8044B3FC, in the slot at 0x8). The constructor
// fn_8044B4B8 stores the object in lbl_8053AF28 and builds the
// "mg_volleyball.blo" screen with two panes. Same shape as cls_804B1CC8
// (game/tu_8044998C.cpp).

#include "types.h"

class J2DScreen;
class PaneCtrl_804AF86C;

class cls_804B1D00 {
public:
    cls_804B1D00();
    virtual ~cls_804B1D00();

    // Draws the screen: lbl_8053AB10->fn_8013B490(), then mScreen at (0, 0)
    // with lbl_8053AB10's J2DOrthoGraph as the grafcontext.
    void fn_8044AF90();
    // Clears bits 0 and 1 of field_0x14, sets the 'Pf' and 'Pt' digit pictures
    // to the first 'TIMG' name, then clears the pane bytes 0xB0 of 'Ndata01'
    // and 'Ndata00'.
    void fn_8044AFD8();
    // Updates the counter digits: field_0x10 (0-999, clamped) on the 'Pf'
    // pictures and field_0x12 / field_0x13 (0-99) on the 'Pt' pictures, shown
    // by the flags in field_0x14, and the two moveFromCentre_80402E08 panes. Clears field_0x14.
    void fn_8044A828();

    /* 0x00 */ // vtable
    /* 0x04 */ J2DScreen* mScreen; // "mg_volleyball.blo"
    /* 0x08 */ PaneCtrl_804AF86C* mPanes[2];
    /* 0x10 */ u16 field_0x10; // 0-999 (clamped); hundreds, tens and units pick the 'Pf' digit pictures
    /* 0x12 */ u8 field_0x12; // 0-99 (clamped; fn_8044A828 also sets field_0x13 to 99 then); tens and units pick 'Pt01'..'Pt02'
    /* 0x13 */ u8 field_0x13; // 0-99 (clamped); tens and units pick 'Pt03'/'Pt04'
    /* 0x14 */ u8 field_0x14; // bit 0: show 'Pf' digits; bit 1: show 'Pt' digits; cleared by fn_8044A828
};

// The object at lbl_8053AF28 (game/tu_8044A828.cpp, constructor fn_8044B4B8;
// the auto split at 0x8053AF28 defines it).
extern cls_804B1D00* lbl_8053AF28;

#endif
