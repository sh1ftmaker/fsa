#ifndef GAME_CLS_8021B904_H
#define GAME_CLS_8021B904_H

#include "types.h"

// A wandering-direction steering timer, defined in game/tu_8021B904.cpp.
// No vtable; named after its first method (fn_8021BA30 initialises it).
// Size: at least 0x12 (the size of the embedding actors' slot is unknown).
class cls_8021B904 {
public:
    // With turn set, counts mTimer up to mTarget and then flips the heading
    // to roughly the opposite direction (plus up to +-0x2000 of noise);
    // without it, counts mCount up to mCountMax and then adds up to +-0x4000
    // to mHeading, steering the angle towards mHeading. Returns the angle.
    s16 fn_8021B904(s16 angle, bool turn);
    // Resets both timers to random durations and sets the heading.
    void fn_8021BA30(s16 heading);

    /* 0x00 */ s32 mTimer;
    /* 0x04 */ s32 mTarget;
    /* 0x08 */ s32 mCount;
    /* 0x0C */ s32 mCountMax;
    /* 0x10 */ s16 mHeading;
};

#endif
