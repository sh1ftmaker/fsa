// The DVD loader thread: a queue of commands (cls_804B0FD0) that a thread
// of its own runs one after another. The command layout and the queue follow
// TWW's m_Do_dvd_thread.cpp (mDoDvdThd_param_c, mDoDvdThd_command_c and its
// toMainRam / mountArchive commands; structure only).
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "dolphin/dvd.h"
#include "dolphin/os.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRDvdRipper.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRThread.h"

// The thread's command queue (TWW's mDoDvdThd_param_c). Named after the
// thread function that runs it.
class cls_80402054 {
public:
    cls_80402054() {
        OSInitMessageQueue(&mQueue, mMessages, 1);
        OSInitMutex(&mMutex);
        mHead = NULL;
        mTail = NULL;
    }

    void kick() { OSSendMessage(&mQueue, NULL, OS_MESSAGE_NOBLOCK); }
    BOOL waitForKick() { return OSReceiveMessage(&mQueue, NULL, OS_MESSAGE_BLOCK); }
    cls_804B0FD0* getFirstCommand() { return mHead; }

    void addition(cls_804B0FD0* cmd) {
        OSLockMutex(&mMutex);
        cmd->mNext = NULL;
        if (mTail == NULL) {
            mTail = cmd;
            mHead = cmd;
        } else {
            mTail->mNext = cmd;
            mTail = cmd;
        }
        OSUnlockMutex(&mMutex);
        kick();
    }

    void cut(cls_804B0FD0* cmd) {
        OSLockMutex(&mMutex);
        cls_804B0FD0* prev = NULL;
        for (cls_804B0FD0* node = mHead; node != NULL; node = node->mNext) {
            if (node == cmd) {
                if (prev != NULL) {
                    prev->mNext = node->mNext;
                    if (mTail == node) {
                        mTail = prev;
                    }
                } else if (mHead == mTail) {
                    mHead = NULL;
                    mTail = NULL;
                } else {
                    mHead = node->mNext;
                }
            }
            prev = node;
        }
        OSUnlockMutex(&mMutex);
        kick();
    }

    /* 0x00 */ OSMessageQueue mQueue;
    /* 0x20 */ OSMessage mMessages[1];
    /* 0x24 */ OSMutex mMutex;
    /* 0x3C */ cls_804B0FD0* mHead;
    /* 0x40 */ cls_804B0FD0* mTail;
}; // Size: 0x44

// Runs one command; TWW's cb(void*), called with &cmd as there (the original
// keeps cmd in a stack slot for this call).
inline BOOL cb(void* userData) {
    cls_804B0FD0** cmd = (cls_804B0FD0**)userData;
    return (*cmd)->vfunc_0xC();
}

static OSThread lbl_80528980;
static u8 lbl_80528CA0[0x1000] ATTRIBUTE_ALIGN(32); // the thread's stack
static cls_80402054 lbl_80529CA0;
static JKRHeap* lbl_8053ADF0; // the commands are allocated here
static JKRHeap* lbl_8053ADF4; // commands without a heap of their own load here
static u8 lbl_8053A218 = 1;   // direction for commands created with 0

inline void* cls_804B0FD0::operator new(size_t size) {
    return JKRHeap::alloc(size, -4, lbl_8053ADF0);
}

// The deleting destructors inline this. The compiler also emits an unreferenced
// weak copy, which the link dead-strips (map: UNUSED).
inline void cls_804B0FD0::operator delete(void* p) {
    JKRHeap::free(p, NULL);
}

inline cls_804B0FD0::cls_804B0FD0() {
    field_0x4 = 0;
    mNext = NULL;
}

inline cls_804B0FA0::cls_804B0FA0(u8 direction) {
    mDirection = direction;
    if (direction == 0) {
        mDirection = lbl_8053A218;
    }
}

inline cls_804B0FB0::cls_804B0FB0(u8 direction, JKRAramArchive* archive) {
    mDirection = direction;
    mEntryNum = -1;
    mArchive = archive;
    mHeap = NULL;
    if (direction == 0) {
        mDirection = lbl_8053A218;
    }
}

inline cls_804B0FC0::cls_804B0FC0(u8 direction) {
    mDirection = direction;
    mEntryNum = -1;
    field_0x14 = NULL;
    mHeap = NULL;
    if (direction == 0) {
        mDirection = lbl_8053A218;
    }
}

// The thread: runs the queued commands in order (TWW's main and
// mDoDvdThd_param_c::mainLoop; written in place here, an inlined member
// loop gives different code).
void* fn_80402054(void* param) {
    { JKRThread thread(OSGetCurrentThread(), 0); }
    cls_80402054* queue = (cls_80402054*)param;
    while (queue->waitForKick()) {
        while (cls_804B0FD0* cmd = queue->getFirstCommand()) {
            queue->cut(cmd);
            cb(&cmd);
        }
    }
    return NULL;
}

void fn_80401FEC(s32 priority, JKRHeap* commandHeap, JKRHeap* loadHeap) {
    lbl_8053ADF4 = loadHeap;
    lbl_8053ADF0 = commandHeap;
    OSCreateThread(&lbl_80528980, fn_80402054, &lbl_80529CA0, lbl_80528CA0 + sizeof(lbl_80528CA0),
                   sizeof(lbl_80528CA0), priority, OS_THREAD_ATTR_DETACH);
    OSResumeThread(&lbl_80528980);
}

cls_804B0FD0::~cls_804B0FD0() {}

cls_804B0FC0::~cls_804B0FC0() {}

cls_804B0FC0* fn_80401E10(const char* path, u8 direction, JKRHeap* heap) {
    s32 entryNum = DVDConvertPathToEntrynum(path);
    cls_804B0FC0* cmd;
    if (entryNum == -1) {
        cmd = NULL;
    } else {
        cmd = new cls_804B0FC0(direction);
        if (cmd != NULL) {
            cmd->mEntryNum = entryNum;
            cmd->mHeap = heap;
            lbl_80529CA0.addition(cmd);
        }
    }
    return cmd;
}

BOOL cls_804B0FC0::vfunc_0xC() {
    JKRHeap* heap = mHeap != NULL ? mHeap : lbl_8053ADF4;
    JKRMemArchive* archive;
    if (mDirection == JKRArchive::MOUNT_DIRECTION_HEAD) {
        archive = new (heap, 0) JKRMemArchive(mEntryNum, JKRArchive::MOUNT_DIRECTION_HEAD);
    } else {
        archive = new (heap, -4) JKRMemArchive(mEntryNum, JKRArchive::MOUNT_DIRECTION_TAIL);
    }
    if (archive != NULL && archive->isMounted()) {
        field_0x14 = archive;
    } else if (archive != NULL) {
        delete archive;
    }
    field_0x4 = 1;
    return field_0x14 != NULL;
}

cls_804B0FB0::~cls_804B0FB0() {}

cls_804B0FB0* fn_80401BA4(const char* path, JKRAramArchive* archive, u8 direction, JKRHeap* heap) {
    s32 entryNum = DVDConvertPathToEntrynum(path);
    cls_804B0FB0* cmd;
    if (entryNum == -1) {
        cmd = NULL;
    } else {
        cmd = new cls_804B0FB0(direction, archive);
        if (cmd != NULL) {
            cmd->mEntryNum = entryNum;
            cmd->mHeap = heap;
            lbl_80529CA0.addition(cmd);
        }
    }
    return cmd;
}

BOOL cls_804B0FB0::vfunc_0xC() {
    JKRHeap* heap = mHeap != NULL ? mHeap : lbl_8053ADF4;
    BOOL result = FALSE;
    if (mArchive == NULL) {
        if (mDirection == JKRArchive::MOUNT_DIRECTION_HEAD) {
            mArchive = new (heap, 0x20) JKRAramArchive();
        } else {
            mArchive = new (heap, -0x20) JKRAramArchive();
        }
    }
    if (mArchive != NULL) {
        result = mArchive->mountFixed(mEntryNum, mDirection == JKRArchive::MOUNT_DIRECTION_HEAD
                                                     ? JKRArchive::MOUNT_DIRECTION_HEAD
                                                     : JKRArchive::MOUNT_DIRECTION_TAIL);
    }
    field_0x4 = 1;
    return result;
}

cls_804B0FA0* fn_804019A8(const char* path, u8 direction, JKRHeap* heap) {
    s32 entryNum = DVDConvertPathToEntrynum(path);
    cls_804B0FA0* cmd;
    if (entryNum == -1) {
        cmd = NULL;
    } else {
        cmd = new cls_804B0FA0(direction);
        if (cmd != NULL) {
            cmd->mEntryNum = entryNum;
            cmd->mHeap = heap;
            lbl_80529CA0.addition(cmd);
        }
    }
    return cmd;
}

BOOL cls_804B0FA0::vfunc_0xC() {
    JKRHeap* heap;
    if (mHeap != NULL) {
        heap = mHeap;
    } else {
        heap = lbl_8053ADF4;
    }
    field_0x14 = JKRDvdRipper::loadToMainRAM(mEntryNum, NULL, EXPAND_SWITCH_DECOMPRESS, 0, heap,
                                             mDirection == JKRArchive::MOUNT_DIRECTION_HEAD
                                                 ? JKRDvdRipper::ALLOC_DIR_TOP
                                                 : JKRDvdRipper::ALLOC_DIR_BOTTOM,
                                             0, NULL, NULL);
    if (field_0x14 != NULL) {
        mSize = heap->getSize(field_0x14);
    }
    field_0x4 = 1;
    return field_0x14 != NULL;
}

cls_804B0FA0::~cls_804B0FA0() {}
