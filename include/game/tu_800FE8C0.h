#ifndef GAME_TU_800FE8C0_H
#define GAME_TU_800FE8C0_H

// Exports of game/tu_800FE8C0.cpp. An animation-frame sequencer embedded in
// an actor (act_shadow_link_demo and act_shadow_link_2 keep it at +0x278 and
// call fn_800FEB78 and fn_800FE8C0 on it). Named after its first function,
// fn_800FEB78 (no vtable, no constructor proper, no destructor).
//
// Owner: game/tu_800FE8C0.cpp.

#include "types.h"

class cls_800FEB78 {
public:
    // Sets the state (r4), previous state 10 and direction 0, then updates.
    void fn_800FEB78(int state);
    // Advances one tick: picks the frame id of the current state, counts the
    // frame's duration and steps to the next entry.
    void fn_800FE8C0();

    /* 0x00 */ int mState;
    /* 0x04 */ int mPrevState; // 10 after fn_800FEB78; a change resets the timer
    /* 0x08 */ int mFrame;
    /* 0x0C */ s8 mDir;
    /* 0x0D */ u8 mIndex;
    /* 0x10 */ int mTimer;
    /* 0x14 */ u32 mFlags; // bit 0x20: set when a sequence ends with 0xFE
}; // Size: at least 0x18 (unknown)

#endif
