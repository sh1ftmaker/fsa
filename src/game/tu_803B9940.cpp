#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "dolphin/card.h"
#include "dolphin/os.h"
#include "string.h"
#include "JSystem/JKernel/JKRThread.h"

// The card thread's stack, thread object and the CARD work area (fn_803B9F40).
static u8 lbl_80510E60[0x1000];
static OSThread lbl_80511E60;
static u8 lbl_80512180[0xA000] ATTRIBUTE_ALIGN(32);

cls_803B9940 lbl_8051C1A0 ATTRIBUTE_ALIGN(32);

static void* fn_803BAA64(void*);

// 0x803B9940
u32 cls_803B9940::fn_803B9940(s32 unused) {
    if (OSTryLockMutex(&mMutex)) {
        u32 result;
        switch (mState) {
        case 1:
            result = 2;
            break;
        case 2:
            result = 1;
            break;
        case 3:
            result = 3;
            break;
        case 4:
            result = 4;
            break;
        case 5:
            result = 5;
            break;
        case 0:
            result = 0;
            break;
        case 6:
            result = 7;
            break;
        case 7:
            result = 6;
            break;
        case 8:
            result = 0xB;
            break;
        case 9:
            result = 0xC;
            break;
        case 10:
            result = 9;
            break;
        case 11:
            result = 0xA;
            break;
        case 12:
            result = 8;
            break;
        case 13:
            result = 0xE;
            break;
        }
        OSUnlockMutex(&mMutex);
        return result;
    }
    return 0xE;
}

// 0x803B9A24
s32 cls_803B9940::fn_803B9A24() {
    s32 result = 0;
    if (OSTryLockMutex(&mMutex)) {
        if (mState == 3) {
            mState = 1;
            result = 1;
        } else {
            result = 2;
        }
        OSUnlockMutex(&mMutex);
    }
    return result;
}

// 0x803B9A94
s32 cls_803B9940::fn_803B9A94(void* dst1, u32 size1, void* dst2, u32 size2, s32 offset) {
    s32 result = 0;
    if (OSTryLockMutex(&mMutex)) {
        if (mState == 3) {
            memcpy(dst1, &mHeader[offset], size1);
            memcpy(dst2, &mSave[offset], size2);
            result = 1;
            mState = 1;
        } else {
            result = 2;
            mState = 2;
        }
        OSUnlockMutex(&mMutex);
    }
    return result;
}

// 0x803B9B3C
void cls_803B9940::fn_803B9B3C() {
    if (OSTryLockMutex(&mMutex)) {
        field_0x1fcc = 2;
        OSUnlockMutex(&mMutex);
        OSSignalCond(&mCond);
    }
}

// 0x803B9B8C
void cls_803B9940::fn_803B9B8C() {
    if (OSTryLockMutex(&mMutex)) {
        field_0x1fcc = 1;
        OSUnlockMutex(&mMutex);
        OSSignalCond(&mCond);
    }
}

// 0x803B9BDC
s32 cls_803B9940::fn_803B9BDC() {
    s32 result = 0;
    if (OSTryLockMutex(&mMutex)) {
        if (mState == 4) {
            mState = 1;
            result = 1;
        } else if (mState == 1) {
            result = 0;
        } else {
            result = 2;
        }
        OSUnlockMutex(&mMutex);
    }
    return result;
}

// 0x803B9C5C
s32 cls_803B9940::fn_803B9C5C() {
    s32 result = 0;
    if (OSTryLockMutex(&mMutex)) {
        if (mState == 5) {
            mState = 2;
            result = 1;
        } else {
            result = 2;
        }
        OSUnlockMutex(&mMutex);
    }
    return result;
}

// 0x803B9CCC
void cls_803B9940::fn_803B9CCC(void* src, u32 n, s32 offset) {
    if (OSTryLockMutex(&mMutex)) {
        memcpy(&mHeader[offset], src, n);
        field_0x1fcc = 3;
        OSUnlockMutex(&mMutex);
        OSSignalCond(&mCond);
    }
}

// 0x803B9D54
void cls_803B9940::fn_803B9D54(void* src1, u32 n1, void* src2, u32 n2, s32 offset) {
    if (OSTryLockMutex(&mMutex)) {
        memcpy(&mHeader[offset], src1, n1);
        memcpy(&mSave[offset], src2, n2);
        field_0x1fcc = 3;
        OSUnlockMutex(&mMutex);
        OSSignalCond(&mCond);
    }
}

// 0x803B9DDC
void cls_803B9940::fn_803B9DDC() {
    if (OSTryLockMutex(&mMutex)) {
        field_0x1fcc = 4;
        OSUnlockMutex(&mMutex);
        OSSignalCond(&mCond);
    }
}

// 0x803B9E2C
void cls_803B9940::fn_803B9E2C() {
    if (lbl_8053ACA8->fn_802B1DCC()) {
        OSLockMutex(&mMutex);
        field_0x1fcc = 6;
        field_0x1ff5 = 3;
        OSUnlockMutex(&mMutex);
        OSSignalCond(&mCond);
    } else if (fn_803B9940(0) != 0xE) {
        if (CARDProbe(0) && fn_803B9940(0) == 0) {
            OSLockMutex(&mMutex);
            field_0x1ff5 = 0;
            field_0x1fcc = 5;
            OSUnlockMutex(&mMutex);
            OSSignalCond(&mCond);
        } else if (!CARDProbe(0) && fn_803B9940(0) != 0) {
            OSLockMutex(&mMutex);
            field_0x1ff5 = 1;
            field_0x1fcc = 6;
            OSUnlockMutex(&mMutex);
            OSSignalCond(&mCond);
        }
    }
}

// 0x803BA980
void cls_803B9940::fn_803BA980() {
    CARDInit();
    field_0x1ff4 = 0;
    field_0x1fcc = 0;
    mState = 0;
    field_0x1ff5 = 2;
    OSInitMutex(&mMutex);
    OSInitCond(&mCond);
    OSCreateThread(&lbl_80511E60, fn_803BAA64, NULL, lbl_80510E60 + sizeof(lbl_80510E60),
                   sizeof(lbl_80510E60), OSGetThreadPriority(OSGetCurrentThread()) + 1, 1);
    OSResumeThread(&lbl_80511E60);
}

// 0x803B9F40 (cls_803B9940::fn_803B9F40, the thread's request loop, 0xA40 bytes) is not
// written yet: it maps every CARD result through one 7-case state switch, inlined at each site.

// 0x803BAA1C
cls_803B9940::~cls_803B9940() {}

// 0x803BAA64
static void* fn_803BAA64(void*) {
    JKRThread(OSGetCurrentThread(), 0);
    lbl_8051C1A0.fn_803B9F40();
    return NULL;
}
