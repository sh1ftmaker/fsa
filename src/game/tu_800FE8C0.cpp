#include "game/GameAudio.h"
#include "game/cls_800FE8C0.h"

static const s32 lbl_80463AA0[4] = {0x3C, 0x3D, 0x3E, 0x3F};
static const u8 lbl_8053C0E0[4] = {10, 0xFF};
static const s32 lbl_80463AB0[4] = {0, 8, 0x10, 0x18};
static const u8 lbl_80463AC0[9] = {3, 3, 3, 3, 3, 3, 3, 3, 0xFF};
static const s32 lbl_80463ACC[4] = {0x20, 0x27, 0x2E, 0x35};
static const u8 lbl_8053C0E4[8] = {1, 1, 1, 2, 1, 1, 1, 0xFE};
static const s32 lbl_80463ADC[4] = {0x252, 0x256, 0x25A, 0x25E};
static const u8 lbl_8053C0EC[8] = {1, 4, 1, 2, 0xFE};
static const s32 lbl_80463AEC[4] = {0xF1, 0xF9, 0x101, 0x109};
static const u8 lbl_80463AFC[10] = {3, 6, 6, 3, 3, 3, 6, 0x50, 3, 0xFE};
static const u8 lbl_80463B08[9] = {0, 1, 2, 3, 4, 5, 6, 7};
static const s32 lbl_80463B14[4] = {0xA7, 0xAB, 0xAF, 0xB3};
static const u8 lbl_8053C0F4[8] = {4, 4, 4, 0x1E, 0xFE};
static const u8 lbl_80463B24[4][12] = {
            {8, 9, 10, 0, 1, 2, 3, 4, 5, 6, 7, 11},
            {12, 13, 14, 4, 5, 6, 7, 0, 1, 2, 3, 15},
            {16, 17, 18, 2, 3, 4, 5, 6, 7, 0, 1, 19},
            {20, 21, 22, 6, 7, 0, 1, 2, 3, 4, 5, 23},
        };
static const u8 lbl_80463B54[13] = {4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0xFE};
static const u8 lbl_8053C0FC[8] = {10, 6, 10, 6, 0xFF};
static const u8 lbl_8053C104[4] = {0, 1, 2, 1};
static const u8 lbl_8053C108[4] = {12, 2, 0x3C, 0xFE};
static const s32 lbl_80463B64[4] = {0x6F, 0x77, 0x7F, 0x87};
static const u8 lbl_8053C10C[4] = {0x19, 0xFE};

// FIXME: 99.2%, regalloc-only. The state (mState) is loaded into r5 and the
// .rodata base into r4 here; the original has them the other way round
// (lwz r4; lis r5), which also changes r4/r5 in the state-reset block.
void cls_800FE8C0::fn_800FE8C0() {
    if (mState != mPrevState) {
        mPrevState = mState;
        mTimer = 0;
        mIndex = 0;
        mFlags &= ~0x20;
    }

    // Per state: the base frame by direction, and a table of frame durations
    // (one byte per entry, 0xFF: restart, 0xFE: stop on the last entry).
    const u8* durations = NULL;
    switch (mState) {
    case 0: {
        mFrame = lbl_80463AA0[mDir];
        durations = lbl_8053C0E0 + mIndex;
        mFrame += mIndex;
        break;
    }
    case 1: {
        mFrame = lbl_80463AB0[mDir];
        durations = lbl_80463AC0 + mIndex;
        mFrame += mIndex;
        break;
    }
    case 2: {
        mFrame = lbl_80463ACC[mDir];
        durations = lbl_8053C0E4 + mIndex;
        mFrame += mIndex;
        break;
    }
    case 3: {
        mFrame = lbl_80463ADC[mDir];
        durations = lbl_8053C0EC + mIndex;
        mFrame += mIndex;
        break;
    }
    case 4: {
        mFrame = lbl_80463AEC[mDir];
        durations = lbl_80463AFC + mIndex;
        mFrame += lbl_80463B08[mIndex];
        break;
    }
    case 5: {
        mFrame = lbl_80463B14[mDir];
        durations = lbl_8053C0F4 + mIndex;
        mFrame += mIndex;
        break;
    }
    case 6: {
        mFrame = lbl_80463B24[mDir][mIndex] + 0x8F;
        durations = lbl_80463B54 + mIndex;
        break;
    }
    case 7: {
        mFrame = 0x36A;
        durations = lbl_8053C0FC + mIndex;
        mFrame += lbl_8053C104[mIndex];
        break;
    }
    case 8: {
        mFrame = 0xE6;
        durations = lbl_8053C108 + mIndex;
        mFrame += mIndex;
        break;
    }
    case 9: {
        mFrame = lbl_80463B64[mDir];
        durations = lbl_8053C10C + mIndex;
        mFrame += mIndex;
        break;
    }
    }

    if (mTimer == durations[0]) {
        mTimer = 0;
        mIndex++;
        if (durations[1] == 0xFF) {
            mIndex = 0;
        } else if (durations[1] == 0xFE) {
            mIndex--;
            mFlags |= 0x20;
        }
    }
    mTimer++;
}

void cls_800FE8C0::fn_800FEB78(int state) {
    mState = state;
    mPrevState = 10;
    mDir = 0;
    fn_800FE8C0();
}
