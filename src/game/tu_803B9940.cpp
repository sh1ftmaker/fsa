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

// 0x803BAA64
static void* fn_803BAA64(void*) {
    JKRThread(OSGetCurrentThread(), 0);
    lbl_8051C1A0.fn_803B9F40();
    return NULL;
}

// 0x803BAA1C
cls_803B9940::~cls_803B9940() {}

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

extern "C" {
s32 fn_803BAEF4(CARDFileInfo* file, void* dst1, void* dst2, s32 size1, s32 size2);
s32 fn_803BBF08(CARDFileInfo* file, void* src1, void* src2, s32 size1, s32 size2);
}
s32 fn_803BAE68(CARDFileInfo* file);

static inline bool mapCardResult(cls_803B9940* self, s32 result) {
    bool ok = false;
    switch (result) {
    case -5:
    case -128:
        self->mState = 0xC;
        break;
    case -3:
        self->mState = 0;
        break;
    case 0:
    case -6:
        if (CARDCheck(self->field_0x1ff4) == 0) {
            ok = true;
        } else {
            self->mState = 7;
        }
        break;
    case -13:
        self->mState = 6;
        break;
    case -4:
        self->mState = 2;
        break;
    }
    return ok;
}

// 0x803B9F40
// FIXME(shotgun): 98 %, different (written from asm)
void cls_803B9940::fn_803B9F40() {
    while (true) {
        OSLockMutex(&mMutex);
        while (field_0x1fcc == 0) {
            OSWaitCond(&mCond, &mMutex);
        }
        switch (field_0x1fcc) {
        case 1: {
            CARDFileInfo file;
            s32 res = CARDOpen(field_0x1ff4, "gc4sword", &file);
            if (res == 0) {
                if (fn_803BAEF4(&file, mHeader, mSave, 0x258, 0x1D4C) == 0) {
                    mState = 3;
                } else {
                    mapCardResult(this, res);
                }
                CARDClose(&file);
            } else {
                mapCardResult(this, res);
            }
            break;
        }
        case 2:
            if (mState == 1) {
                CARDFileInfo file;
                s32 res = CARDOpen(field_0x1ff4, "gc4sword", &file);
                if (res == 0) {
                    if (fn_803BAE68(&file) == 0) {
                        mState = 3;
                    } else {
                        mapCardResult(this, res);
                    }
                    CARDClose(&file);
                } else {
                    mapCardResult(this, res);
                }
            }
            break;
        case 3: {
            CARDFileInfo file;
            s32 res;
            if (mState == 2) {
                res = CARDCreate(field_0x1ff4, "gc4sword", 0x6000, &file);
                if (res == 0) {
                    mState = 1;
                } else if (res == -7) {
                    mState = 1;
                } else {
                    mapCardResult(this, res);
                }
            }
            if (mState == 1) {
                res = CARDOpen(field_0x1ff4, "gc4sword", &file);
                if (res == 0) {
                    res = fn_803BBF08(&file, mHeader, mSave, 0x258, 0x1D4C);
                    if (res != 0) {
                        mapCardResult(this, res);
                    } else {
                        mState = 4;
                    }
                    CARDClose(&file);
                } else {
                    mapCardResult(this, res);
                }
            } else {
                mapCardResult(this, res);
            }
            break;
        }
        case 4: {
            s32 res = CARDFormat(field_0x1ff4);
            if (res == 0) {
                mState = 5;
            } else {
                mapCardResult(this, res);
            }
            break;
        }
        case 5: {
            s32 memSize;
            s32 sectorSize;
            if (CARDProbeEx(field_0x1ff4, &memSize, &sectorSize) == -2) {
                mState = 0xA;
            } else if (sectorSize != 0x2000) {
                mState = 0xB;
            } else if (mapCardResult(this, CARDMount(field_0x1ff4, lbl_80512180, NULL))) {
                CARDFileInfo file;
                bool exists;
                s32 res = CARDOpen(field_0x1ff4, "gc4sword", &file);
                if (res == 0) {
                    CARDClose(&file);
                    exists = true;
                } else {
                    mapCardResult(this, res);
                    exists = false;
                }
                if (exists) {
                    mState = 1;
                } else {
                    s32 byteNotUsed;
                    s32 filesNotUsed;
                    s32 freeStatus;
                    res = CARDFreeBlocks(field_0x1ff4, &byteNotUsed, &filesNotUsed);
                    if (res != 0) {
                        mapCardResult(this, res);
                        freeStatus = 3;
                    } else if (byteNotUsed < 0x6000) {
                        freeStatus = 1;
                    } else if (filesNotUsed < 1) {
                        freeStatus = 2;
                    } else {
                        freeStatus = 0;
                    }
                    switch (freeStatus) {
                    case 0:
                        mState = 2;
                        break;
                    case 1:
                        mState = 8;
                        break;
                    case 2:
                        mState = 9;
                        break;
                    }
                }
            }
            break;
        }
        case 6:
            CARDUnmount(field_0x1ff4);
            mState = 0;
            break;
        }
        field_0x1fcc = 0;
        OSUnlockMutex(&mMutex);
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

// 0x803B9DDC
void cls_803B9940::fn_803B9DDC() {
    if (OSTryLockMutex(&mMutex)) {
        field_0x1fcc = 4;
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

// 0x803B9CCC
void cls_803B9940::fn_803B9CCC(void* src, u32 n, s32 offset) {
    if (OSTryLockMutex(&mMutex)) {
        memcpy(&mHeader[offset], src, n);
        field_0x1fcc = 3;
        OSUnlockMutex(&mMutex);
        OSSignalCond(&mCond);
    }
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

// 0x803B9B8C
void cls_803B9940::fn_803B9B8C() {
    if (OSTryLockMutex(&mMutex)) {
        field_0x1fcc = 1;
        OSUnlockMutex(&mMutex);
        OSSignalCond(&mCond);
    }
}

// 0x803B9B3C
void cls_803B9940::fn_803B9B3C() {
    if (OSTryLockMutex(&mMutex)) {
        field_0x1fcc = 2;
        OSUnlockMutex(&mMutex);
        OSSignalCond(&mCond);
    }
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
