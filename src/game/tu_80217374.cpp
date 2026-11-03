// cls_80217374, a wandering-direction timer (see its header). The unit's
// __sinit (the 9 JAudio pools) comes from the GameAudio.h include.

#include "game/GameAudio.h"
#include "game/cls_80217374.h"
#include "game/GameGlobals.h"

// FIXME: 96.6 %. The target copies angle to r3 on entry and computes the
// state 1/2 result straight into r3; this keeps it in r30 and adds a final
// mr r3, r30. A result variable, a ternary and returning from that branch
// were each worse.
s16 cls_80217374::fn_80217374(s16 angle) {
    mTimer++;
    if (mTimer >= mDuration) {
        switch (mState) {
        case 0:
            if (fn_801CD664(2) == 0) {
                mDuration = fn_801CD664(61) + 60;
                mState = 1;
                mTimer = 0;
                return angle - 0x4000;
            } else {
                mDuration = fn_801CD664(61) + 60;
                mState = 2;
                mTimer = 0;
                return angle + 0x4000;
            }
        case 1:
        case 2:
            mDuration = fn_801CD664(1) + 180;
            if (mState == 1) {
                angle += 0x4000;
            } else {
                angle -= 0x4000;
            }
            mState = 0;
            mTimer = 0;
            break;
        }
    }
    return angle;
}

void cls_80217374::fn_80217484() {
    mState = 0;
    mTimer = 0;
    mDuration = fn_801CD664(1) + 180;
    mDuration = mDuration * lbl_8053AAF8->mRandom.get_ufloat_1();
}
