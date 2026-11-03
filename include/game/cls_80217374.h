#ifndef GAME_CLS_80217374_H
#define GAME_CLS_80217374_H

#include "types.h"

// A 0xC-byte wandering-direction timer, defined in game/tu_80217374.cpp.
// No vtable or constructor; named after its first method. Embedded in the
// shadow Link (act_shadow_link_2, +0x550) and thief (act_thief, +0x368)
// actors, which pass their facing angle (s16 at 0x33C) through fn_80217374
// every frame.
class cls_80217374 {
public:
    // Counts mTimer up to mDuration; then from state 0 turns a quarter turn
    // left or right (state 1 or 2) for 60-120 frames, and from state 1 or 2
    // turns back (state 0) for 180 frames. Returns the new angle.
    s16 fn_80217374(s16 angle);
    // State 0, with a random part of the 180 frames already used up.
    void fn_80217484();

    /* 0x0 */ s32 mState;
    /* 0x4 */ s32 mTimer;
    /* 0x8 */ s32 mDuration;
};

#endif
