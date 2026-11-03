#include "game/GameAudio.h"
#include "game/act/tu_8011E96C.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"

// Whether `layer` is 0 or any player's layer (fn_8023DE58).
static inline bool isOnPlayerLayer(s32 layer) {
    if (layer == 0) {
        return true;
    }
    for (s32 i = 0; i < 4; i++) {
        if (layer == fn_8023DE58(i)) {
            return true;
        }
    }
    return false;
}

cls_8049DE90::~cls_8049DE90() {}

// FIXME: 97%. Only the layer check in state 1 differs: the original reloads
// field_0x4 from memory after each fn_8023DE58 call (no register hoist) and
// has one extra unreachable `b` after the switch dispatch.
void cls_8049DE90::update() {
    switch (mState) {
    case 0:
        if (vfunc_0x3C() != 0 && !vfunc_0x34()) {
            mState = 1;
            break;
        }
        mTimer++;
        if (mTimer >= mDuration) {
            s32 pick = fn_801CD664(4);
            if (mCycle == 1) {
                pick = mNext;
                mNext++;
                if (mNext >= 4) {
                    mNext = 0;
                }
            }
            if (mTable[pick] != 0) {
                for (s32 i = 0; i < 4; i++) {
                    if (i != pick && mTable[i] != mTable[pick]) {
                        fn_802D6A9C()->fn_802D8050(mTable[i], false, 0xFF);
                    }
                }
                fn_802D6A9C()->fn_802D8050(mTable[pick], true, 0xFF);
            }
            mTimer = 0;
        }
        if (mDuration == 0) {
            mState = 2;
            fn_801F3668();
        }
        break;
    case 1: {
        if (mDuration == 0 && isOnPlayerLayer(field_0x4)) {
            mState = 0;
        } else if (vfunc_0x3C() == 0) {
            mState = 0;
        } else if (vfunc_0x34()) {
            mState = 0;
        }
        break;
    }
    }
}

void cls_8049DE90::init() {
    mDuration = field_0x90 & 0x3F;
    mDuration *= 10;
    mCycle = (field_0x90 >> 6) & 1;
    mTable[0] = (field_0x90 >> 7) & 0x1F;
    mTable[1] = (field_0x90 >> 12) & 0x1F;
    mTable[2] = (field_0x90 >> 17) & 0x1F;
    mTable[3] = (field_0x90 >> 22) & 0x1F;
    bool empty = true;
    for (s32 i = 0; i < 4; i++) {
        if (mTable[i] != 0) {
            empty = false;
            break;
        }
    }
    if (empty) {
        mState = 2;
        fn_801F3668();
    }
}
