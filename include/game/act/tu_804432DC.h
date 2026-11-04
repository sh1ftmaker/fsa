#ifndef GAME_ACT_TU_804432DC_H
#define GAME_ACT_TU_804432DC_H

// Code: game/tu_804432DC.cpp. A map-editor overlay built from "map_ed.blo".
// Named after its vtable, 0x804B1AE8 (only the destructor, fn_8044368C); the
// constructor is fn_80443760, which stores the object in lbl_8053AEF0.

#include "types.h"

class J2DScreen;
class cls_804AF86C;

class cls_804B1AE8 {
public:
    cls_804B1AE8();
    virtual ~cls_804B1AE8();

    // Starts a window animation of kind 0-7 (a pane offset and scale from
    // the tables in the .cpp) with timer a and hold time b. Only while the
    // state is idle (3); returns false.
    bool fn_804432DC(u8 kind, s16 a, s16 b);
    // State 0 (table entry 0): counts mTimer down, then goes to state 1.
    void fn_804435B8();
    // State 1: counts mTimer up while the panes move, then goes to state 2.
    void fn_804434B8();
    // State 2: counts mHoldTimer down, then goes to idle (state 3).
    void fn_80443494();
    // Draws the screen unless idle.
    void fn_804435E4();
    // Calls the state's pointer-to-member from lbl_804B1AC4 on the object.
    void fn_80443638();
    // Empty; called on lbl_8053AEF0 by game/tu_8043A0E8.cpp.
    void fn_80443688();

    /* 0x00 */ // vtable
    /* 0x04 */ J2DScreen* mScreen;
    /* 0x08 */ cls_804AF86C* mPtestmap; // pane 'Ptestmap'
    /* 0x0C */ cls_804AF86C* mPmapten;  // pane 'Pmapten'
    /* 0x10 */ s16 mTimer;
    /* 0x12 */ s16 mHoldTimer;
    /* 0x14 */ u8 mKind;
    /* 0x15 */ u8 mState; // 0-2 running, 3 idle
};

extern cls_804B1AE8* lbl_8053AEF0;

#endif
