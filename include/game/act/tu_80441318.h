#ifndef GAME_ACT_TU_80441318_H
#define GAME_ACT_TU_80441318_H

// Code: game/tu_80441318.cpp. Created by game/tu_80418B64.cpp (__nw__ 0x10
// at 0x80420654, stored at 0x50 of its object; fn_80441490 is called from
// there and from game/tu_8042165C.cpp each frame).

#include "types.h"

// Picks the window (gGameRoot2_8053AB10->field_0x20's field_0x22c) one player's
// screen shows. Named after its vtable, 0x804B19E0 (only the destructor,
// fn_80441730); the constructor is fn_80441778. fn_80441490 steps the state
// machine below and sets the window; the two empty members are called by the
// units that own it, once per frame.
class cls_804B19E0 {
public:
    cls_804B19E0();
    virtual ~cls_804B19E0();

    // Draw hook, empty (tu_80418B64 and tu_8042165C call it after the screen draw).
    void fn_8044148C();
    // Update: sets the window, then runs the state function.
    void fn_80441490();
    // Empty (tu_80418B64 calls it at 0x804201C0).
    void fn_8044172C();

    // State 2: takes the window of the player fn_80231A50 reports; state 1 when none.
    void fn_80441318();
    // State 1: counts up to 300 frames, then state 2 once a player exists.
    void fn_804413F0();
    // State 0: state 1 (timer 0) once the view has left its screen.
    void fn_80441444();

    /* 0x00 */ // vtable
    /* 0x04 */ s32 field_0x4; // window index, -1 for none
    /* 0x08 */ s32 field_0x8; // frames without a player (up to 3)
    /* 0x0C */ u16 mTimer;
    /* 0x0E */ u8 mState;
    /* 0x0F */ u8 field_0xf;
}; // Size: 0x10

#endif
