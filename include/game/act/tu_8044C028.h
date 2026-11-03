#ifndef GAME_ACT_TU_8044C028_H
#define GAME_ACT_TU_8044C028_H

// Code: game/tu_8044C028.cpp.

#include "types.h"

class J2DScreen;

// A prologue overlay built from two J2DScreens ("op_prologue.blo" and
// "op_prologue_m.blo"). Named after its vtable, 0x804B1D20 (only the
// destructor, fn_8044C1CC); the constructor is fn_8044C260, which stores the
// object in lbl_8053AF38. tu_801286C0 calls fn_8044C028 through
// lbl_8053AF38; the object at 0x2B8 of tu_8043A0E8's object gets
// fn_8044C038 and fn_8044C14C.
class cls_804B1D20 {
public:
    cls_804B1D20();
    virtual ~cls_804B1D20();

    // Sets the shown flag and the offset field_0x14 (f1).
    void fn_8044C028(f32 offset);
    // When shown, moves the two "Tp01"/"Tp02" text panes of mScreen[1] to
    // their rest y (field_0xC / field_0x10) minus field_0x14.
    void fn_8044C038();
    // When shown, draws both screens and clears the flag.
    void fn_8044C14C();

    /* 0x00 */ // vtable
    /* 0x04 */ J2DScreen* mScreen[2];
    /* 0x0C */ f32 field_0xc;  // "Tp01" y + 308
    /* 0x10 */ f32 field_0x10; // "Tp02" y + 308
    /* 0x14 */ f32 field_0x14; // y offset
    /* 0x18 */ bool field_0x18; // shown
    /* 0x19 */ u8 pad_0x19[3];
}; // Size: 0x1C

extern cls_804B1D20* lbl_8053AF38;

#endif
