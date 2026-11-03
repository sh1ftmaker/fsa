#include "game/GameAudio.h"
#include "game/cls_8021CBA0.h"
#include "game/act/act_shadow_link_2.h"

// Frame counts of the sequence steps per type (types 0 and 2 in sdata2).
extern const s32 lbl_8053FD20[1] = {3};
extern const s32 lbl_8053FD24[2] = {3, 4};

// One object holding the step lengths of type 1 and the three per-variant
// frame tables (indexed [variant][step], the unit's single rodata symbol).
struct cls_8021CBA0_Tables {
    /* 0x00 */ s32 lengths[7];
    /* 0x1C */ s32 table0[4][1];
    /* 0x2C */ s32 table1[4][7];
    /* 0x9C */ s32 table2[4][2];
    /* 0xBC */ s32 pad;
}; // Size: 0xC0

extern const cls_8021CBA0_Tables lbl_8046A688 = {
    {5, 4, 4, 4, 4, 4, 5},
    {{0xF1}, {0xF9}, {0x101}, {0x109}},
    {{0xF2, 0xF7, 0xF6, 0xF5, 0xF4, 0xF3, 0xF2},
     {0xFA, 0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA},
     {0x102, 0x107, 0x106, 0x105, 0x104, 0x103, 0x102},
     {0x10A, 0x10F, 0x10E, 0x10D, 0x10C, 0x10B, 0x10A}},
    {{0xF1, 0x3C}, {0xF9, 0x3D}, {0x101, 0x3E}, {0x109, 0x3F}},
    0,
};

// Only [0] of each state is read (by ShadowLink2_804A43E8::fn_8021CDE4).
extern const u16 lbl_8046A748[][5] = {
    {0x0C, 0x0A, 0x1E, 0x3C, 0x78},
    {0x0C, 0x0A, 0x1E, 0x3C, 0x78},
    {0x08, 0x0A, 0x1E, 0x3C, 0x78},
    {0x01, 0x0A, 0x1E, 0x3C, 0x78},
};

// The frame count of the current step of the current type's table.
static inline s32 stepFrames(const cls_8021CBA0& seq) {
    const cls_8021CBA0_Tables* tables = &lbl_8046A688;
    s32 frames = 0;
    switch (seq.mType) {
    case 0: frames = tables->table0[seq.mVariant][seq.mStep]; break;
    case 1: frames = tables->table1[seq.mVariant][seq.mStep]; break;
    case 2: frames = tables->table2[seq.mVariant][seq.mStep]; break;
    }
    return frames;
}

// FIXME: 82 %. The original keeps the address of lbl_8046A688 in one register
// from the start (r6) and reaches each table as base + 0x1c / 0x2c / 0x9c plus
// an indexed lwzx, and reuses it for the type 1 step lengths; ours folds the
// table offsets into the lwz displacements and reloads the base (lis/addi).
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
        case 1: lengths = lbl_8046A688.lengths; break;
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

// FIXME: 76 %. Same table-addressing difference as fn_8021C848 (base + table
// offset in a register, lwzx), and the shared zero is r5 in the original (ours r6).
void cls_8021CBA0::fn_8021C9C4(int variant) {
    mVariant = variant;
    mType = 0;
    mStep = 0;
    mTimer = 0;
    mDone = false;
    mFrames = stepFrames(*this);
}

// FIXME: 75 %. Same as fn_8021C9C4.
void cls_8021CBA0::fn_8021CA70(int type) {
    mType = type;
    mStep = 0;
    mTimer = 0;
    mDone = false;
    mFrames = stepFrames(*this);
}

s32 cls_8021CBA0::fn_8021CB18() {
    s32 total = 0;
    switch (mType) {
    case 0:
        total = lbl_8053FD20[0];
        break;
    case 1:
        for (int i = 0; i < 7; i++) {
            total += lbl_8046A688.lengths[i];
        }
        break;
    case 2:
        total = lbl_8053FD24[0] + lbl_8053FD24[1];
        break;
    }
    return total;
}

cls_8021CBA0::cls_8021CBA0() {
    mType = 0;
    mFrames = 0;
    mStep = 0;
    mTimer = 0;
    mVariant = 0;
    mDone = false;
}
