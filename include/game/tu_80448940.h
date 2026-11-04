#ifndef GAME_TU_80448940_H
#define GAME_TU_80448940_H

// Code: game/tu_80448940.cpp. Named after its vtable, 0x804B1C90 (only the
// virtual destructor, __dt__12cls_804B1C90Fv, in the slot at 0x8). The screen
// is "mg_geldman.blo". The constructor __ct__12cls_804B1C90Fv stores the
// object in lbl_8053AF18.

#include "types.h"

class J2DScreen;
class cls_804AF86C;

class cls_804B1C90 {
public:
    // Stores itself in lbl_8053AF18 and builds mScreen and the pane arrays.
    cls_804B1C90();
    virtual ~cls_804B1C90();

    // Draws the screen: lbl_8053AB10->fn_8013B490(), then mScreen at (0, 0)
    // with lbl_8053AB10's J2DOrthoGraph as the grafcontext.
    void fn_80449070();
    // Resets the screen: sets the first 'TIMG' name on the 'Pt' pictures of
    // the four slots and of the counter, clears field_0x32's bits, and hides
    // the slot panes and 'Ndata00'.
    void fn_804490B8();
    // Per frame: sets the four slots' fn_80402E08 positions, shows or hides
    // each slot by its bit of field_0x32, updates the slot digits
    // (field_0x2c) and the counter digits (field_0x30 / field_0x31), then
    // clears field_0x32.
    void fn_80448940();

    /* 0x00 */ // vtable
    /* 0x04 */ J2DScreen* mScreen; // "mg_geldman.blo"
    /* 0x08 */ cls_804AF86C* field_0x8; // pane tag 'Ndata00'
    /* 0x0C */ cls_804AF86C* field_0xc[4]; // pane tags from lbl_80490940[i][6]
    /* 0x1C */ cls_804AF86C* field_0x1c[4]; // pane tags from lbl_80490940[i][13]
    /* 0x2C */ u8 field_0x2c[4]; // per slot: 0-99 (clamped); tens and units pick the 'Pt' pictures
    /* 0x30 */ u8 field_0x30; // 0-99 (clamped); tens and units pick 'Pt01'..'Pt02'
    /* 0x31 */ u8 field_0x31; // 0-99 (clamped); tens and units pick 'Pt03'/'Pt04'
    /* 0x32 */ u8 field_0x32; // bit n (n < 4): slot n shown; bit 4: 'Ndata00' shown; cleared at the end of fn_80448940
    /* 0x33 */ u8 pad_0x33;
}; // Size: 0x34 (the constructor's allocation in tu_8043A0E8)

// The object at lbl_8053AF18 (game/tu_80448940.cpp, constructor
// __ct__12cls_804B1C90Fv; the auto split at 0x8053AF18 defines it).
extern cls_804B1C90* lbl_8053AF18;

#endif
