#ifndef GAME_ACT_TU_8044B80C_H
#define GAME_ACT_TU_8044B80C_H

// Code: game/tu_8044B80C.cpp. Created in game/tu_80139BE8.cpp (0x8013A830),
// requested by tu_802E9428 through lbl_8053AF30.

#include "types.h"

class J2DScreen;
class cls_804B0FC0;

// A staff-roll page loader. Named after its vtable, 0x804B1D10 (only the
// destructor, fn_8044BCA4); the constructor is fn_8044BDCC, which stores the
// object in lbl_8053AF30. It loads "/GC4Sword_usa/staff/staff%d.arc" and
// builds a J2DScreen from "staffroll_p%d.blo" in it.
class cls_804B1D10 {
public:
    cls_804B1D10();
    virtual ~cls_804B1D10();

    // Requests page idx (0-7). Returns true when that page is already
    // shown; otherwise drops a shown page and returns false.
    bool fn_8044B80C(u8 idx);
    // Draws the screen once, when the draw flag is set and the page is shown.
    void fn_8044B8EC();
    // Loads the archive (state 1), then builds the screen (state 2 -> 3).
    void fn_8044B960();

    /* 0x00 */ // vtable
    /* 0x04 */ J2DScreen* field_0x4;      // only freed by the destructor
    /* 0x08 */ J2DScreen* mScreen;
    /* 0x0C */ cls_804B0FC0* mArchive;    // fn_80401E10 result
    /* 0x10 */ u32 field_0x10;
    /* 0x14 */ bool mDraw;
    /* 0x15 */ u8 mPage;                  // 0xFF when none
    /* 0x16 */ u8 mState;                 // 0 idle, 1 load, 2 loading, 3 shown
    /* 0x17 */ u8 pad_0x17;
}; // Size: 0x18 (tu_80139BE8 allocates 0x18)

extern cls_804B1D10* lbl_8053AF30;

#endif
