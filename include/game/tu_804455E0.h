#ifndef GAME_TU_804455E0_H
#define GAME_TU_804455E0_H

// Exports of game/tu_804455E0.cpp.

#include "types.h"

class J2DScreen;
class cls_804AF86C;

// The 0x48-byte object (tu_8043A0E8 allocates 0x48 at 0x8043F678) whose constructor (game/tu_804455E0.cpp, 0x804461D0)
// stores the vtable 0x804B1BB0 and the object itself in lbl_8053AEF8. Named
// after that vtable. Only the best-time records fn_801ED3DC (game/tu_801ED3DC.cpp)
// writes are known.
class cls_804B1BB0 {
public:
    // Constructor (game/tu_804455E0.cpp). Stores itself in lbl_8053AEF8 and
    // builds mScreen and the pane arrays.
    cls_804B1BB0();
    // Virtual destructor (game/tu_804455E0.cpp): deletes mScreen and the pane
    // arrays below.
    virtual ~cls_804B1BB0();

    // Calls lbl_8053AB10->fn_8013B490(), then draws mScreen at (0, 0) with
    // lbl_8053AB10's J2DOrthoGraph as the grafcontext.
    void fn_80445CDC();
    // For each of the four slots: clears bits slot and slot + 4 of field_0x44,
    // resets the digit and rank pictures to their first TIMG names, and clears
    // the 0xB0 byte of three panes.
    void fn_80445D24();
    // Per slot: the time of field_0x3c / field_0x38 as digit pictures, the
    // best-time and rank panes' 0xB0 bytes, and the rank picture; clears
    // field_0x44 at the end.
    void fn_804455E0();

    /* 0x000 */ // vtable
    /* 0x004 */ J2DScreen* mScreen; // new J2DScreen in the constructor, then setPriority (lbl_80490020)
    /* 0x008 */ cls_804AF86C* field_0x8[4];  // panes, created by the constructor
    /* 0x018 */ cls_804AF86C* field_0x18[4]; // panes, created by the constructor
    /* 0x028 */ cls_804AF86C* field_0x28[4]; // panes, created by the constructor
    /* 0x038 */ u8 field_0x38[4]; // per slot: hundredths of a second
    /* 0x03C */ u8 field_0x3c[4]; // per slot: seconds
    /* 0x040 */ u8 field_0x40[4]; // per slot: rank + 1
    /* 0x044 */ u8 field_0x44;    // bit n: slot n has a time; bit n + 4: slot n has a rank
    /* 0x045 */ u8 pad_0x45[0x48 - 0x45];
}; // Size: 0x48
extern cls_804B1BB0* lbl_8053AEF8;

#endif
