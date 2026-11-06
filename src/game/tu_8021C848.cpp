#include "game/GameAudio.h"
#include "game/cls_8021CBA0.h"
#include "game/act/act_shadow_link_2.h"

// Frame counts of the sequence steps per type (types 0 and 2 in sdata2).
static const s32 lbl_8053FD20[1] = {3};
static const s32 lbl_8053FD24[2] = {3, 4};

// Step lengths of type 1 and the per-variant frame tables, indexed [variant][step].
static const s32 lbl_8046A688[7] = {5, 4, 4, 4, 4, 4, 5};
static const s32 lbl_8046A6A4[4][1] = {{0xF1}, {0xF9}, {0x101}, {0x109}};
static const s32 lbl_8046A6B4[4][7] = {
    {0xF2, 0xF7, 0xF6, 0xF5, 0xF4, 0xF3, 0xF2},
    {0xFA, 0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA},
    {0x102, 0x107, 0x106, 0x105, 0x104, 0x103, 0x102},
    {0x10A, 0x10F, 0x10E, 0x10D, 0x10C, 0x10B, 0x10A},
};
static const s32 lbl_8046A724[4][2] = {{0xF1, 0x3C}, {0xF9, 0x3D}, {0x101, 0x3E}, {0x109, 0x3F}};

// The frame count of the current step of the current type's table.
static inline s32 stepFrames(const cls_8021CBA0& seq) {
    s32 frames = 0;
    switch (seq.mType) {
    case 0: frames = lbl_8046A6A4[seq.mVariant][seq.mStep]; break;
    case 1: frames = lbl_8046A6B4[seq.mVariant][seq.mStep]; break;
    case 2: frames = lbl_8046A724[seq.mVariant][seq.mStep]; break;
    }
    return frames;
}

cls_8021CBA0::cls_8021CBA0() {
    mType = 0;
    mFrames = 0;
    mStep = 0;
    mTimer = 0;
    mVariant = 0;
    mDone = false;
}

s32 cls_8021CBA0::fn_8021CB18() {
    s32 total = 0;
    switch (mType) {
    case 0:
        total = lbl_8053FD20[0];
        break;
    case 1:
        for (int i = 0; i < 7; i++) {
            total += lbl_8046A688[i];
        }
        break;
    case 2:
        total = lbl_8053FD24[0] + lbl_8053FD24[1];
        break;
    }
    return total;
}

void cls_8021CBA0::fn_8021CA70(int type) {
    mType = type;
    mStep = 0;
    mTimer = 0;
    mDone = false;
    mFrames = stepFrames(*this);
}

void cls_8021CBA0::fn_8021C9C4(int variant) {
    mVariant = variant;
    mType = 0;
    mStep = 0;
    mTimer = 0;
    mDone = false;
    mFrames = stepFrames(*this);
}

u8 cls_8021CBA0::fn_8021C848() {
    if (!mDone) {
        s32 count = 0;
        switch (mType) {
        case 0: count = 1; break;
        case 1: count = 7; break;
        case 2: count = 2; break;
        }
        mTimer++;
        const s32* lengths = NULL;
        switch (mType) {
        case 0: lengths = lbl_8053FD20; break;
        case 1: lengths = lbl_8046A688; break;
        case 2: lengths = lbl_8053FD24; break;
        }
        if (mTimer >= lengths[mStep]) {
            mTimer = 0;
            mStep++;
            if (mStep >= count) {
                mStep--;
                mDone = true;
            }
        }
    }
    mFrames = stepFrames(*this);
    return mDone;
}
