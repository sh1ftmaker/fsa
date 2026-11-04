#ifndef GAME_TU_804465B8_H
#define GAME_TU_804465B8_H

// Code: game/tu_804465B8.cpp (the mg_oni.blo screen). Named after its vtable,
// 0x804B1BE8 (only the virtual destructor, fn_80447190, in the slot at 0x8);
// the constructor is fn_8044728C, which stores the object in lbl_8053AF00 and
// builds mScreen and the pane arrays. Same shape as cls_804B1BB0 (tu_804455E0).
// tu_8043A0E8 keeps the object at cls_804B1920's field 0x2A0 (0x34 bytes).

#include "types.h"

class J2DScreen;
class cls_804AF86C;

class cls_804B1BE8 {
public:
    cls_804B1BE8();
    virtual ~cls_804B1BE8();

    // Per slot: the two panes of each slot get the distance from
    // fn_8043D514 / fn_8043D57C as y, the flag bits of field_0x30 switch the
    // slot's pane bytes, and the time pictures (minutes and seconds from
    // field_0x2c, hundredths from field_0x28) are set when the first pane's
    // byte is set. Clears field_0x30 at the end.
    void fn_804465B8();
    // Calls lbl_8053AB10->fn_8013B490(), then draws mScreen at (0, 0) with
    // lbl_8053AB10's J2DOrthoGraph as the grafcontext.
    void fn_80446D68();
    // Per slot: clears the slot's two bits of field_0x30, sets the ten picture
    // panes of the slot to the first 'TIMG' name and hides
    // nine panes.
    void fn_80446DB0();

    /* 0x00 */ // vtable
    /* 0x04 */ J2DScreen* mScreen; // "mg_oni.blo"
    /* 0x08 */ cls_804AF86C* field_0x8[4];  // panes, created by the constructor
    /* 0x18 */ cls_804AF86C* field_0x18[4]; // panes, created by the constructor
    /* 0x28 */ u8 field_0x28[4]; // per slot: hundredths of a second, clamped to 99
    /* 0x2C */ u8 field_0x2c[4]; // per slot: seconds, shown as minutes and seconds
    /* 0x30 */ u8 field_0x30;    // bit n: slot n's pictures are on; bit n + 4: slot n's pane 0x40 is on
}; // Size: 0x34

// The object at lbl_8053AF00 (game/tu_804465B8.cpp, constructor fn_8044728C;
// the auto split at 0x8053AF00 defines it).
extern cls_804B1BE8* lbl_8053AF00;

#endif
