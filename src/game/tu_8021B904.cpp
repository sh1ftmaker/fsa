#include "game/GameAudio.h"
#include "game/cls_8021B904.h"
#include "game/GameGlobals.h"

// A random offset in [-range / 2, range / 2].
static inline s32 spread(s32 range) {
    return fn_801CD664(range + 1) - range / 2;
}

void cls_8021B904::fn_8021BA30(s16 heading) {
    mTimer = 0;
    mTarget = fn_801CD664(91) + 30;
    mCount = 0;
    mCountMax = fn_801CD664(21) + 10;
    mHeading = heading;
}

s16 cls_8021B904::fn_8021B904(s16 angle, bool turn) {
    if (turn) {
        mCount = 0;
        mCountMax = fn_801CD664(21) + 10;
        mTimer++;
        if (mTimer >= mTarget) {
            angle -= 0x8000;
            angle += spread(0x4000);
            mTimer = 0;
            mTarget = fn_801CD664(91) + 30;
        }
        mHeading = angle;
    } else {
        mTimer = 0;
        mTarget = fn_801CD664(91) + 30;
        mCount++;
        if (mCount >= mCountMax) {
            mHeading += spread(0x8000);
            mCount = 0;
            mCountMax = fn_801CD664(21) + 10;
        }
        fn_801CD930(&angle, mHeading, 0x200);
    }
    return angle;
}
