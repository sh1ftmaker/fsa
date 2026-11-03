// The game's THP movie player (0x80427D2C-0x8042E964).
//
// One translation unit holds Nintendo's THP decoder library (sms's
// THPDec.c / THPAudio.c, included below), the SDK's THPPlayer sample
// adapted to C++ (its read, video decode and audio decode threads became
// classes with a static instance, reached through the sample's C-style
// functions), and the game's movie layer on top. FSA builds it with
// `-inline off,deferred`: code is emitted in reverse source order, so the
// source runs from the wrappers at the end of .text back to the movie layer
// at its start.
//
// Names: the THPPlayer functions and the sample's thread functions keep the
// SDK sample's names (doldecomp/sms, doldecomp/mkdd and zeldaret/tww's
// d_a_movie_player, whose code they follow). The classes, the movie layer and
// the extra read-thread functions have no known names: provisional names carry
// their address.

#include "game/GameAudio.h"
#include <dolphin/os.h>
#include <dolphin/dvd.h>
#include <dolphin/gx.h>
#include <dolphin/vi.h>
#include <dolphin/ai.h>
#include <dolphin/mtx.h>
#include <dolphin/thp.h>
#include <string.h>
#include "JSystem/JAudio/System/JASDriver.h"
#include "JSystem/JFramework/JFWDisplay.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTVideo.h"

extern "C" void LCEnable();
extern "C" void LCDisable();

typedef struct THPHeader {
    /* 0x00 */ char magic[4];
    /* 0x04 */ u32 version;
    /* 0x08 */ u32 bufsize;
    /* 0x0C */ u32 audioMaxSamples;
    /* 0x10 */ f32 frameRate;
    /* 0x14 */ u32 numFrames;
    /* 0x18 */ u32 firstFrameSize;
    /* 0x1C */ u32 movieDataSize;
    /* 0x20 */ u32 compInfoDataOffsets;
    /* 0x24 */ u32 offsetDataOffsets;
    /* 0x28 */ u32 movieDataOffsets;
    /* 0x2C */ u32 finalFrameDataOffsets;
} THPHeader; // Size: 0x30

typedef struct THPFrameCompInfo {
    /* 0x00 */ u32 numComponents;
    /* 0x04 */ u8 frameComp[16];
} THPFrameCompInfo; // Size: 0x14

typedef struct THPVideoInfo {
    /* 0x00 */ u32 xSize;
    /* 0x04 */ u32 ySize;
    /* 0x08 */ u32 videoType;
} THPVideoInfo; // Size: 0xC

typedef struct THPAudioInfo {
    /* 0x00 */ u32 sndChannels;
    /* 0x04 */ u32 sndFrequency;
    /* 0x08 */ u32 sndNumSamples;
    /* 0x0C */ u32 sndNumTracks;
} THPAudioInfo; // Size: 0x10

typedef struct THPTextureSet {
    /* 0x00 */ u8* ytexture;
    /* 0x04 */ u8* utexture;
    /* 0x08 */ u8* vtexture;
    /* 0x0C */ s32 frameNumber;
} THPTextureSet; // Size: 0x10

typedef struct THPAudioBuffer {
    /* 0x00 */ s16* buffer;
    /* 0x04 */ s16* curPtr;
    /* 0x08 */ u32 validSample;
} THPAudioBuffer; // Size: 0xC

typedef struct THPReadBuffer {
    /* 0x00 */ u8* ptr;
    /* 0x04 */ s32 frameNumber;
    /* 0x08 */ BOOL isValid;
} THPReadBuffer; // Size: 0xC

#define THP_READ_BUFFER_COUNT 10
#define THP_TEXTURE_SET_COUNT 3
#define THP_AUDIO_BUFFER_COUNT 3

// The SDK sample's player state, with byte-sized flags.
typedef struct THPPlayer {
    /* 0x000 */ DVDFileInfo fileInfo;
    /* 0x03C */ THPHeader header;
    /* 0x06C */ THPFrameCompInfo compInfo;
    /* 0x080 */ THPVideoInfo videoInfo;
    /* 0x08C */ THPAudioInfo audioInfo;
    /* 0x09C */ void* thpWork;
    /* 0x0A0 */ u8 open;
    /* 0x0A1 */ u8 state;
    /* 0x0A2 */ u8 internalState;
    /* 0x0A3 */ u8 playFlag;
    /* 0x0A4 */ u8 audioExist;
    /* 0x0A8 */ s32 dvdError;
    /* 0x0AC */ s32 videoError;
    /* 0x0B0 */ u8 onMemory;
    /* 0x0B4 */ u8* movieData;
    /* 0x0B8 */ s32 initOffset;
    /* 0x0BC */ s32 initReadSize;
    /* 0x0C0 */ s32 initReadFrame;
    /* 0x0C4 */ u32 curField;
    /* 0x0C8 */ s64 retaceCount;
    /* 0x0D0 */ s32 prevCount;
    /* 0x0D4 */ s32 curCount;
    /* 0x0D8 */ s32 videoDecodeCount;
    /* 0x0DC */ f32 curVolume;
    /* 0x0E0 */ f32 targetVolume;
    /* 0x0E4 */ f32 deltaVolume;
    /* 0x0E8 */ s32 rampCount;
    /* 0x0EC */ s32 curAudioTrack;
    /* 0x0F0 */ s32 curVideoNumber;
    /* 0x0F4 */ s32 curAudioNumber;
    /* 0x0F8 */ THPTextureSet* dispTextureSet;
    /* 0x0FC */ THPAudioBuffer* playAudioBuffer;
    /* 0x100 */ THPReadBuffer readBuffer[THP_READ_BUFFER_COUNT];
    /* 0x178 */ THPTextureSet textureSet[THP_TEXTURE_SET_COUNT];
    /* 0x1A8 */ THPAudioBuffer audioBuffer[THP_AUDIO_BUFFER_COUNT];
} THPPlayer; // Size: 0x1D0

// The read thread (the SDK sample's THPRead.c). Named after its vtable.
class THPReadThread_804B15F4 {
public:
    THPReadThread_804B15F4();
    virtual ~THPReadThread_804B15F4();
    BOOL create(OSPriority prio);
    void cancel();
    void start();
    void* reader(void* arg);
    void pushReadedBuffer2(void* buffer);
    void* popReadedBuffer2();
    void pushFreeReadBuffer(void* buffer);
    void* popFreeReadBuffer();
    void pushReadedBuffer(void* buffer);
    void* popReadedBuffer();

    // The movie layer borrows the idle thread to open the file.
    void setCreated_80428524(BOOL created);
    bool isTerminated_8042852C();
    void startOpen_8042855C();
    bool createOpen_8042858C(OSPriority prio);
    void* opener_804285F4(void* arg);

    static THPReadThread_804B15F4* sInstance;

    /* 0x004 */ BOOL mCreated;
    /* 0x008 */ OSMessageQueue mFreeReadBufferQueue;
    /* 0x028 */ OSMessageQueue mReadedBufferQueue;
    /* 0x048 */ OSMessageQueue mReadedBufferQueue2;
    /* 0x068 */ OSMessage mFreeReadBufferMessage[THP_READ_BUFFER_COUNT];
    /* 0x090 */ OSMessage mReadedBufferMessage[THP_READ_BUFFER_COUNT];
    /* 0x0B8 */ OSMessage mReadedBufferMessage2[THP_READ_BUFFER_COUNT];
    /* 0x0E0 */ OSThread mThread;
    /* 0x3F8 */ u8 mStack[0x1000];
}; // Size: 0x13F8

// The video decode thread (THPVideoDecode.c). Named after its vtable.
class THPVideoDecodeThread_804B15E8 {
public:
    THPVideoDecodeThread_804B15E8();
    virtual ~THPVideoDecodeThread_804B15E8();
    void cancel();
    void start();
    BOOL create(OSPriority prio, void* arg);
    void* videoDecoderForOnMemory(void* arg);
    void* videoDecoder(void* arg);
    void videoDecode(THPReadBuffer* readBuffer);
    void pushDecodedTextureSet(void* tex);
    void* popDecodedTextureSet(s32 flags);
    void pushFreeTextureSet(void* tex);
    void* popFreeTextureSet();

    static THPVideoDecodeThread_804B15E8* sInstance;

    /* 0x00004 */ BOOL mCreated;
    /* 0x00008 */ OSThread mThread;
    /* 0x00320 */ u8 mStack[0x64000];
    /* 0x64320 */ OSMessageQueue mFreeTextureSetQueue;
    /* 0x64340 */ OSMessageQueue mDecodedTextureSetQueue;
    /* 0x64360 */ OSMessage mFreeTextureSetMessage[THP_TEXTURE_SET_COUNT];
    /* 0x6436C */ OSMessage mDecodedTextureSetMessage[THP_TEXTURE_SET_COUNT];
    /* 0x64378 */ BOOL mFirst;
}; // Size: 0x64380

// The audio decode thread (THPAudioDecode.c). Named after its vtable.
class THPAudioDecodeThread_804B15DC {
public:
    THPAudioDecodeThread_804B15DC();
    virtual ~THPAudioDecodeThread_804B15DC();
    void cancel();
    void start();
    BOOL create(OSPriority prio, void* arg);
    void* audioDecoderForOnMemory(void* arg);
    void* audioDecoder(void* arg);
    void audioDecode(THPReadBuffer* readBuffer);
    void pushDecodedAudioBuffer(void* buf);
    void* popDecodedAudioBuffer(s32 flags);
    void pushFreeAudioBuffer(void* buf);
    void* popFreeAudioBuffer();

    static THPAudioDecodeThread_804B15DC* sInstance;

    /* 0x00004 */ BOOL mCreated;
    /* 0x00008 */ OSThread mThread;
    /* 0x00320 */ u8 mStack[0x64000];
    /* 0x64320 */ OSMessageQueue mFreeAudioBufferQueue;
    /* 0x64340 */ OSMessageQueue mDecodedAudioBufferQueue;
    /* 0x64360 */ OSMessage mFreeAudioBufferMessage[THP_AUDIO_BUFFER_COUNT];
    /* 0x6436C */ OSMessage mDecodedAudioBufferMessage[THP_AUDIO_BUFFER_COUNT];
}; // Size: 0x64378

// The SDK sample's C interface to the three threads.
inline BOOL CreateReadThread(OSPriority prio) { return THPReadThread_804B15F4::sInstance->create(prio); }
inline void ReadThreadStart() { THPReadThread_804B15F4::sInstance->start(); }
inline void ReadThreadCancel() { THPReadThread_804B15F4::sInstance->cancel(); }
inline void* Reader(void* arg) { return THPReadThread_804B15F4::sInstance->reader(arg); }
inline void* PopReadedBuffer() { return THPReadThread_804B15F4::sInstance->popReadedBuffer(); }
inline void PushReadedBuffer(void* buffer) { THPReadThread_804B15F4::sInstance->pushReadedBuffer(buffer); }
inline void* PopFreeReadBuffer() { return THPReadThread_804B15F4::sInstance->popFreeReadBuffer(); }
inline void PushFreeReadBuffer(void* buffer) { THPReadThread_804B15F4::sInstance->pushFreeReadBuffer(buffer); }
inline void* PopReadedBuffer2() { return THPReadThread_804B15F4::sInstance->popReadedBuffer2(); }
inline void PushReadedBuffer2(void* buffer) { THPReadThread_804B15F4::sInstance->pushReadedBuffer2(buffer); }

inline BOOL CreateVideoDecodeThread(OSPriority prio, void* arg) {
    return THPVideoDecodeThread_804B15E8::sInstance->create(prio, arg);
}
inline void VideoDecodeThreadStart() { THPVideoDecodeThread_804B15E8::sInstance->start(); }
inline void VideoDecodeThreadCancel() { THPVideoDecodeThread_804B15E8::sInstance->cancel(); }
inline void* VideoDecoder(void* arg) { return THPVideoDecodeThread_804B15E8::sInstance->videoDecoder(arg); }
inline void* VideoDecoderForOnMemory(void* arg) {
    return THPVideoDecodeThread_804B15E8::sInstance->videoDecoderForOnMemory(arg);
}
inline void VideoDecode(THPReadBuffer* readBuffer) { THPVideoDecodeThread_804B15E8::sInstance->videoDecode(readBuffer); }
inline void* PopFreeTextureSet() { return THPVideoDecodeThread_804B15E8::sInstance->popFreeTextureSet(); }
inline void PushFreeTextureSet(void* tex) { THPVideoDecodeThread_804B15E8::sInstance->pushFreeTextureSet(tex); }
inline void* PopDecodedTextureSet(s32 flags) {
    return THPVideoDecodeThread_804B15E8::sInstance->popDecodedTextureSet(flags);
}
inline void PushDecodedTextureSet(void* tex) { THPVideoDecodeThread_804B15E8::sInstance->pushDecodedTextureSet(tex); }

inline BOOL CreateAudioDecodeThread(OSPriority prio, void* arg) {
    return THPAudioDecodeThread_804B15DC::sInstance->create(prio, arg);
}
inline void AudioDecodeThreadStart() { THPAudioDecodeThread_804B15DC::sInstance->start(); }
inline void AudioDecodeThreadCancel() { THPAudioDecodeThread_804B15DC::sInstance->cancel(); }
inline void* AudioDecoder(void* arg) { return THPAudioDecodeThread_804B15DC::sInstance->audioDecoder(arg); }
inline void* AudioDecoderForOnMemory(void* arg) {
    return THPAudioDecodeThread_804B15DC::sInstance->audioDecoderForOnMemory(arg);
}
inline void AudioDecode(THPReadBuffer* readBuffer) { THPAudioDecodeThread_804B15DC::sInstance->audioDecode(readBuffer); }
inline void* PopFreeAudioBuffer() { return THPAudioDecodeThread_804B15DC::sInstance->popFreeAudioBuffer(); }
inline void PushFreeAudioBuffer(void* buf) { THPAudioDecodeThread_804B15DC::sInstance->pushFreeAudioBuffer(buf); }
inline void* PopDecodedAudioBuffer(s32 flags) {
    return THPAudioDecodeThread_804B15DC::sInstance->popDecodedAudioBuffer(flags);
}
inline void PushDecodedAudioBuffer(void* buf) { THPAudioDecodeThread_804B15DC::sInstance->pushDecodedAudioBuffer(buf); }

inline void SetReadThreadCreated_8042E204(BOOL created) { THPReadThread_804B15F4::sInstance->setCreated_80428524(created); }
inline bool IsReadThreadTerminated_8042E22C() { return THPReadThread_804B15F4::sInstance->isTerminated_8042852C(); }
inline void* Opener_8042E250(void* arg) { return THPReadThread_804B15F4::sInstance->opener_804285F4(arg); }
inline void OpenThreadStart_8042E278() { THPReadThread_804B15F4::sInstance->startOpen_8042855C(); }
inline bool CreateOpenThread_8042E29C(OSPriority prio) { return THPReadThread_804B15F4::sInstance->createOpen_8042858C(prio); }

static BOOL THPPlayerInit(BOOL audioSystem);
static void THPPlayerQuit();
static bool THPPlayerOpen(const char* fileName, BOOL onMemory);
static BOOL THPPlayerClose();
static BOOL THPPlayerSetBuffer(u8* buffer);
static bool THPPlayerPrepare(s32 frame, u8 flag, s32 audioTrack);
static BOOL THPPlayerPlay();
static void THPPlayerStop();
static void THPPlayerDrawDone();
static s32 THPPlayerDrawCurrentFrame(GXRenderModeObj* rmode, s32 x, s32 y, s32 polygonW, s32 polygonH);
static BOOL THPPlayerGetVideoInfo(THPVideoInfo* videoInfo);
static BOOL THPPlayerGetAudioInfo(THPAudioInfo* audioInfo);
static u32 THPPlayerGetTotalFrame();
static s32 THPPlayerGetState();
static BOOL THPPlayerSetVolume(s32 vol, s32 duration);
static void PrepareReady(u8 msg);

// Part of FSA's THPDec.c, which sms's copy lacks: it opens the unit's .bss in
// front of THPDec.c's statics. Name and size from zeldaret/tww's
// d_a_movie_player (the same player in one TU): `static u8 THPStatistics[1120];`
// (src/d/actor/d_a_movie_player.cpp:29; rels/d_a_movie_player/symbols.txt:190).
static u8 THPStatistics[1120];

// FIXME: __THPInverseDCTY8, __THPInverseDCTNoYPos and __THPReadQuantizationTable
// differ only in their .sdata2 literals: the target pools the ReadQuantization
// double before the IDCT floats, ours after them (part of the unsolved
// -inline deferred output order, see the progress notes).
// FIXME: __sinit (99.3 %) calls the JASMemPool constructors kept at
// 0x8042E964.. (tu_8042E964's range); ours calls this object's own copies.
#include "../dolphin/thp/THPAudio.c"
#include "../dolphin/thp/THPDec.c"

static THPPlayer ActivePlayer;

// --- THPRead ---------------------------------------------------------------

THPReadThread_804B15F4* THPReadThread_804B15F4::sInstance;

THPReadThread_804B15F4::THPReadThread_804B15F4() {
    sInstance = this;
    mCreated = FALSE;
}

THPReadThread_804B15F4::~THPReadThread_804B15F4() {
    sInstance = NULL;
}

void* THPReadThread_804B15F4::popReadedBuffer() {
    void* buffer;
    OSReceiveMessage(&mReadedBufferQueue, &buffer, OS_MESSAGE_BLOCK);
    return buffer;
}

void THPReadThread_804B15F4::pushReadedBuffer(void* buffer) {
    OSSendMessage(&mReadedBufferQueue, buffer, OS_MESSAGE_BLOCK);
}

void* THPReadThread_804B15F4::popFreeReadBuffer() {
    void* buffer;
    OSReceiveMessage(&mFreeReadBufferQueue, &buffer, OS_MESSAGE_BLOCK);
    return buffer;
}

void THPReadThread_804B15F4::pushFreeReadBuffer(void* buffer) {
    OSSendMessage(&mFreeReadBufferQueue, buffer, OS_MESSAGE_BLOCK);
}

void* THPReadThread_804B15F4::popReadedBuffer2() {
    void* buffer;
    OSReceiveMessage(&mReadedBufferQueue2, &buffer, OS_MESSAGE_BLOCK);
    return buffer;
}

void THPReadThread_804B15F4::pushReadedBuffer2(void* buffer) {
    OSSendMessage(&mReadedBufferQueue2, buffer, OS_MESSAGE_BLOCK);
}

void THPReadThread_804B15F4::start() {
    if (mCreated) {
        OSResumeThread(&mThread);
    }
}

void THPReadThread_804B15F4::cancel() {
    if (mCreated) {
        OSCancelThread(&mThread);
        mCreated = FALSE;
    }
}

// The size of the next frame is stored at the start of each frame (tww
// d_a_movie_player.cpp has the same helper as daMP_NEXT_READ_SIZE).
inline s32 NEXT_READ_SIZE(THPReadBuffer* buf) {
    return *(s32*)buf->ptr;
}

void* THPReadThread_804B15F4::reader(void* arg) {
    THPReadBuffer* buf;
    s32 curFrame;
    s32 status;
    s32 offset = ActivePlayer.initOffset;
    s32 readSize = ActivePlayer.initReadSize;
    s32 frame = 0;

    while (TRUE) {
        buf = (THPReadBuffer*)PopFreeReadBuffer();
        status = DVDReadPrio(&ActivePlayer.fileInfo, buf->ptr, readSize, offset, 2);
        if (status != readSize) {
            if (status == -1) {
                ActivePlayer.dvdError = -1;
            }
            if (frame == 0) {
                PrepareReady(FALSE);
            }
            OSSuspendThread(&mThread);
        }

        buf->frameNumber = frame;
        PushReadedBuffer(buf);
        offset += readSize;
        readSize = NEXT_READ_SIZE(buf);

        curFrame = (frame + ActivePlayer.initReadFrame) % ActivePlayer.header.numFrames;
        if (curFrame == ActivePlayer.header.numFrames - 1) {
            if (ActivePlayer.playFlag & 1) {
                offset = ActivePlayer.header.movieDataOffsets;
            } else {
                OSSuspendThread(&mThread);
            }
        }
        frame++;
    }
}

BOOL THPReadThread_804B15F4::create(OSPriority prio) {
    if (OSCreateThread(&mThread, Reader, NULL, mStack + sizeof(mStack), sizeof(mStack), prio, 1) == FALSE) {
        return FALSE;
    }

    OSInitMessageQueue(&mFreeReadBufferQueue, mFreeReadBufferMessage, THP_READ_BUFFER_COUNT);
    OSInitMessageQueue(&mReadedBufferQueue, mReadedBufferMessage, THP_READ_BUFFER_COUNT);
    OSInitMessageQueue(&mReadedBufferQueue2, mReadedBufferMessage2, THP_READ_BUFFER_COUNT);
    mCreated = TRUE;
    return TRUE;
}

// --- THPVideoDecode --------------------------------------------------------

THPVideoDecodeThread_804B15E8* THPVideoDecodeThread_804B15E8::sInstance;

THPVideoDecodeThread_804B15E8::THPVideoDecodeThread_804B15E8() {
    sInstance = this;
    mCreated = FALSE;
}

THPVideoDecodeThread_804B15E8::~THPVideoDecodeThread_804B15E8() {
    sInstance = NULL;
}

void* THPVideoDecodeThread_804B15E8::popFreeTextureSet() {
    void* tex;
    OSReceiveMessage(&mFreeTextureSetQueue, &tex, OS_MESSAGE_BLOCK);
    return tex;
}

void THPVideoDecodeThread_804B15E8::pushFreeTextureSet(void* tex) {
    OSSendMessage(&mFreeTextureSetQueue, tex, OS_MESSAGE_NOBLOCK);
}

void* THPVideoDecodeThread_804B15E8::popDecodedTextureSet(s32 flags) {
    void* tex;
    if (OSReceiveMessage(&mDecodedTextureSetQueue, &tex, flags) == TRUE) {
        return tex;
    }
    return NULL;
}

void THPVideoDecodeThread_804B15E8::pushDecodedTextureSet(void* tex) {
    OSSendMessage(&mDecodedTextureSetQueue, tex, OS_MESSAGE_BLOCK);
}

void THPVideoDecodeThread_804B15E8::videoDecode(THPReadBuffer* readBuffer) {
    THPTextureSet* textureSet;
    u32 i;
    u32* tileOffsets;
    u8* tile;

    tileOffsets = (u32*)(readBuffer->ptr + 8);
    tile = &readBuffer->ptr[ActivePlayer.compInfo.numComponents * 4] + 8;
    textureSet = (THPTextureSet*)PopFreeTextureSet();

    for (i = 0; i < ActivePlayer.compInfo.numComponents; i++) {
        switch (ActivePlayer.compInfo.frameComp[i]) {
        case 0:
            ActivePlayer.videoError = THPVideoDecode(tile, textureSet->ytexture, textureSet->utexture,
                                                     textureSet->vtexture, ActivePlayer.thpWork);
            if (ActivePlayer.videoError) {
                if (mFirst) {
                    PrepareReady(FALSE);
                    mFirst = FALSE;
                }
                OSSuspendThread(&mThread);
            }
            textureSet->frameNumber = readBuffer->frameNumber;
            PushDecodedTextureSet(textureSet);
            BOOL enabled = OSDisableInterrupts();
            ActivePlayer.videoDecodeCount++;
            OSRestoreInterrupts(enabled);
        }

        tile += *tileOffsets;
        tileOffsets++;
    }

    if (mFirst) {
        PrepareReady(TRUE);
        mFirst = FALSE;
    }
}

void* THPVideoDecodeThread_804B15E8::videoDecoder(void* arg) {
    THPReadBuffer* thpBuffer;

    while (TRUE) {
        if (ActivePlayer.audioExist) {
            while (ActivePlayer.videoDecodeCount < 0) {
                thpBuffer = (THPReadBuffer*)PopReadedBuffer2();
                s32 remaining = (thpBuffer->frameNumber + ActivePlayer.initReadFrame) % ActivePlayer.header.numFrames;
                if (remaining == ActivePlayer.header.numFrames - 1 && (ActivePlayer.playFlag & 1) == 0) {
                    VideoDecode(thpBuffer);
                }
                PushFreeReadBuffer(thpBuffer);
                BOOL enabled = OSDisableInterrupts();
                ActivePlayer.videoDecodeCount++;
                OSRestoreInterrupts(enabled);
            }
        }

        if (ActivePlayer.audioExist) {
            thpBuffer = (THPReadBuffer*)PopReadedBuffer2();
        } else {
            thpBuffer = (THPReadBuffer*)PopReadedBuffer();
        }
        VideoDecode(thpBuffer);
        PushFreeReadBuffer(thpBuffer);
    }
}

void* THPVideoDecodeThread_804B15E8::videoDecoderForOnMemory(void* arg) {
    THPReadBuffer readBuffer;
    s32 readSize;
    s32 frame;

    frame = 0;
    readSize = ActivePlayer.initReadSize;
    readBuffer.ptr = (u8*)arg;

    while (TRUE) {
        if (ActivePlayer.audioExist) {
            while (ActivePlayer.videoDecodeCount < 0) {
                BOOL enabled = OSDisableInterrupts();
                ActivePlayer.videoDecodeCount++;
                OSRestoreInterrupts(enabled);

                s32 remaining = (frame + ActivePlayer.initReadFrame) % ActivePlayer.header.numFrames;
                if (remaining == ActivePlayer.header.numFrames - 1) {
                    if ((ActivePlayer.playFlag & 1) == 0) {
                        break;
                    }
                    readSize = *(s32*)readBuffer.ptr;
                    readBuffer.ptr = ActivePlayer.movieData;
                } else {
                    s32 size = *(s32*)readBuffer.ptr;
                    readBuffer.ptr += readSize;
                    readSize = size;
                }
                frame++;
            }
        }

        readBuffer.frameNumber = frame;
        VideoDecode(&readBuffer);

        s32 remaining = (frame + ActivePlayer.initReadFrame) % ActivePlayer.header.numFrames;
        if (remaining == ActivePlayer.header.numFrames - 1) {
            if (ActivePlayer.playFlag & 1) {
                readSize = *(s32*)readBuffer.ptr;
                readBuffer.ptr = ActivePlayer.movieData;
            } else {
                OSSuspendThread(&mThread);
            }
        } else {
            s32 size = *(s32*)readBuffer.ptr;
            readBuffer.ptr += readSize;
            readSize = size;
        }
        frame++;
    }
}

BOOL THPVideoDecodeThread_804B15E8::create(OSPriority prio, void* arg) {
    if (arg) {
        if (OSCreateThread(&mThread, VideoDecoderForOnMemory, arg, mStack + sizeof(mStack), sizeof(mStack), prio, 1)
            == FALSE) {
            return FALSE;
        }
    } else {
        if (OSCreateThread(&mThread, VideoDecoder, NULL, mStack + sizeof(mStack), sizeof(mStack), prio, 1) == FALSE) {
            return FALSE;
        }
    }

    OSInitMessageQueue(&mFreeTextureSetQueue, mFreeTextureSetMessage, THP_TEXTURE_SET_COUNT);
    OSInitMessageQueue(&mDecodedTextureSetQueue, mDecodedTextureSetMessage, THP_TEXTURE_SET_COUNT);
    mCreated = TRUE;
    mFirst = TRUE;
    return TRUE;
}

void THPVideoDecodeThread_804B15E8::start() {
    if (mCreated) {
        OSResumeThread(&mThread);
    }
}

void THPVideoDecodeThread_804B15E8::cancel() {
    if (mCreated) {
        OSCancelThread(&mThread);
        mCreated = FALSE;
    }
}

// --- THPAudioDecode --------------------------------------------------------

THPAudioDecodeThread_804B15DC* THPAudioDecodeThread_804B15DC::sInstance;

THPAudioDecodeThread_804B15DC::THPAudioDecodeThread_804B15DC() {
    sInstance = this;
    mCreated = FALSE;
}

THPAudioDecodeThread_804B15DC::~THPAudioDecodeThread_804B15DC() {
    sInstance = NULL;
}

void* THPAudioDecodeThread_804B15DC::popFreeAudioBuffer() {
    void* buf;
    OSReceiveMessage(&mFreeAudioBufferQueue, &buf, OS_MESSAGE_BLOCK);
    return buf;
}

void THPAudioDecodeThread_804B15DC::pushFreeAudioBuffer(void* buf) {
    OSSendMessage(&mFreeAudioBufferQueue, buf, OS_MESSAGE_NOBLOCK);
}

void* THPAudioDecodeThread_804B15DC::popDecodedAudioBuffer(s32 flags) {
    void* buf;
    if (OSReceiveMessage(&mDecodedAudioBufferQueue, &buf, flags) == TRUE) {
        return buf;
    }
    return NULL;
}

void THPAudioDecodeThread_804B15DC::pushDecodedAudioBuffer(void* buf) {
    OSSendMessage(&mDecodedAudioBufferQueue, buf, OS_MESSAGE_BLOCK);
}

void THPAudioDecodeThread_804B15DC::audioDecode(THPReadBuffer* readBuffer) {
    THPAudioBuffer* audioBuf;
    u32 i;
    u32* offsets;
    u8* audioData;

    offsets = (u32*)(readBuffer->ptr + 8);
    audioData = &readBuffer->ptr[ActivePlayer.compInfo.numComponents * 4] + 8;
    audioBuf = (THPAudioBuffer*)PopFreeAudioBuffer();

    for (i = 0; i < ActivePlayer.compInfo.numComponents; i++) {
        switch (ActivePlayer.compInfo.frameComp[i]) {
        case 1:
            audioBuf->validSample = THPAudioDecode(audioBuf->buffer,
                                                   audioData + *offsets * ActivePlayer.curAudioTrack, 0);
            audioBuf->curPtr = audioBuf->buffer;
            PushDecodedAudioBuffer(audioBuf);
            return;
        }

        audioData += *offsets;
        offsets++;
    }
}

void* THPAudioDecodeThread_804B15DC::audioDecoder(void* arg) {
    THPReadBuffer* buf;

    while (TRUE) {
        buf = (THPReadBuffer*)PopReadedBuffer();
        AudioDecode(buf);
        PushReadedBuffer2(buf);
    }
}

void* THPAudioDecodeThread_804B15DC::audioDecoderForOnMemory(void* arg) {
    s32 size;
    s32 readSize;
    THPReadBuffer readBuffer;
    s32 frame;
    s32 remaining;

    readSize = ActivePlayer.initReadSize;
    readBuffer.ptr = (u8*)arg;
    frame = 0;

    while (TRUE) {
        readBuffer.frameNumber = frame;
        AudioDecode(&readBuffer);

        remaining = (frame + ActivePlayer.initReadFrame) % ActivePlayer.header.numFrames;
        if (remaining == ActivePlayer.header.numFrames - 1) {
            if (ActivePlayer.playFlag & 1) {
                readSize = *(s32*)readBuffer.ptr;
                readBuffer.ptr = ActivePlayer.movieData;
            } else {
                OSSuspendThread(&mThread);
            }
        } else {
            size = *(s32*)readBuffer.ptr;
            readBuffer.ptr += readSize;
            readSize = size;
        }
        frame++;
    }
}

BOOL THPAudioDecodeThread_804B15DC::create(OSPriority prio, void* arg) {
    if (arg) {
        if (OSCreateThread(&mThread, AudioDecoderForOnMemory, arg, mStack + sizeof(mStack), sizeof(mStack), prio, 1)
            == FALSE) {
            return FALSE;
        }
    } else {
        if (OSCreateThread(&mThread, AudioDecoder, NULL, mStack + sizeof(mStack), sizeof(mStack), prio, 1) == FALSE) {
            return FALSE;
        }
    }

    OSInitMessageQueue(&mFreeAudioBufferQueue, mFreeAudioBufferMessage, THP_AUDIO_BUFFER_COUNT);
    OSInitMessageQueue(&mDecodedAudioBufferQueue, mDecodedAudioBufferMessage, THP_AUDIO_BUFFER_COUNT);
    mCreated = TRUE;
    return TRUE;
}

void THPAudioDecodeThread_804B15DC::start() {
    if (mCreated) {
        OSResumeThread(&mThread);
    }
}

void THPAudioDecodeThread_804B15DC::cancel() {
    if (mCreated) {
        OSCancelThread(&mThread);
        mCreated = FALSE;
    }
}

// --- THPDraw ---------------------------------------------------------------

void THPGXRestore() {
    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_SET);
    GXSetNumTexGens(1);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE3, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
}

// FIXME: 98.4 %: the target loads fbWidth before the -1.0f literal and
// converts w before h; ours schedules them the other way round. The GXColorS10
// literal's relocation also differs while .sdata2 order differs (see below).
void THPGXYuv2RgbSetup(GXRenderModeObj* rmode) {
    s32 w, h;
    Mtx44 m;
    Mtx e_m;

    w = rmode->fbWidth;
    h = rmode->efbHeight;

    C_MTXOrtho(m, 0.0f, h, 0.0f, w, 0.0f, -1.0f);
    GXSetProjection(m, GX_ORTHOGRAPHIC);
    GXSetViewport(0.0f, 0.0f, w, h, 0.0f, 1.0f);
    GXSetScissor(0, 0, w, h);

    PSMTXIdentity(e_m);
    GXLoadPosMtxImm(e_m, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetNumChans(0);

    GXSetNumTexGens(2);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXInvalidateTexAll();

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT7, GX_VA_POS, GX_CLR_RGBA, GX_RGBA4, 0);
    GXSetVtxAttrFmt(GX_VTXFMT7, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBX8, 0);

    GXSetNumTevStages(4);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_DISABLE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_KONST, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_DISABLE, GX_TEVPREV);
    GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K0_A);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);

    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP2, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, GX_DISABLE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_TEXA, GX_CA_KONST, GX_CA_APREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_DISABLE, GX_TEVPREV);
    GXSetTevKColorSel(GX_TEVSTAGE1, GX_TEV_KCSEL_K1);
    GXSetTevKAlphaSel(GX_TEVSTAGE1, GX_TEV_KASEL_K1_A);
    GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);

    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_TEXC, GX_CC_ONE, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
    GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);

    GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_APREV, GX_CC_CPREV, GX_CC_KONST, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
    GXSetTevSwapMode(GX_TEVSTAGE3, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevKColorSel(GX_TEVSTAGE3, GX_TEV_KCSEL_K2);

    GXSetTevColorS10(GX_TEVREG0, (GXColorS10){-90, 0, -114, 135});
    GXSetTevKColor(GX_KCOLOR0, (GXColor){0, 0, 226, 88});
    GXSetTevKColor(GX_KCOLOR1, (GXColor){179, 0, 0, 182});
    GXSetTevKColor(GX_KCOLOR2, (GXColor){255, 0, 255, 128});
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
}

void THPGXYuv2RgbDraw(u8* y_data, u8* u_data, u8* v_data, s16 x, s16 y, s16 textureWidth, s16 textureHeight,
                      s16 polygonWidth, s16 polygonHeight) {
    GXTexObj tobj0, tobj1, tobj2;

    GXInitTexObj(&tobj0, y_data, textureWidth, textureHeight, GX_TF_I8, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&tobj0, GX_NEAR, GX_NEAR, 0, 0, 0, 0, 0, GX_ANISO_1);
    GXLoadTexObj(&tobj0, GX_TEXMAP0);
    GXInitTexObj(&tobj1, u_data, textureWidth >> 1, textureHeight >> 1, GX_TF_I8, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&tobj1, GX_NEAR, GX_NEAR, 0, 0, 0, 0, 0, GX_ANISO_1);
    GXLoadTexObj(&tobj1, GX_TEXMAP1);
    GXInitTexObj(&tobj2, v_data, textureWidth >> 1, textureHeight >> 1, GX_TF_I8, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&tobj2, GX_NEAR, GX_NEAR, 0, 0, 0, 0, 0, GX_ANISO_1);
    GXLoadTexObj(&tobj2, GX_TEXMAP2);

    GXBegin(GX_QUADS, GX_VTXFMT7, 4);
    GXPosition3s16(x, y, 0);
    GXTexCoord2u16(0, 0);
    GXPosition3s16(x + polygonWidth, y, 0);
    GXTexCoord2u16(1, 0);
    GXPosition3s16(x + polygonWidth, y + polygonHeight, 0);
    GXTexCoord2u16(1, 1);
    GXPosition3s16(x, y + polygonHeight, 0);
    GXTexCoord2u16(0, 1);
    GXEnd();
}

// --- THPPlayer -------------------------------------------------------------

static u32 WorkBuffer[16] ATTRIBUTE_ALIGN(32);
static OSMessageQueue PrepareReadyQueue;
static OSMessageQueue UsedTextureSetQueue;
static OSMessage UsedTextureSetMessage[THP_TEXTURE_SET_COUNT];
static s16 SoundBuffer[2][0x460] ATTRIBUTE_ALIGN(32);

static u8 Initialized;
static void* PrepareReadyMessage;
static VIRetraceCallback OldVIPostCallback;
static s32 SoundBufferIndex;
static s16* LastAudioBuffer;
static s16* CurAudioBuffer;
static BOOL AudioSystem;

static u16 VolumeTable[128] = {
    0,     2,     8,     18,    32,    50,    73,    99,    130,   164,   203,   245,   292,   343,   398,   457,
    520,   587,   658,   733,   812,   895,   983,   1074,  1170,  1269,  1373,  1481,  1592,  1708,  1828,  1952,
    2080,  2212,  2348,  2488,  2632,  2781,  2933,  3090,  3250,  3415,  3583,  3756,  3933,  4114,  4298,  4487,
    4680,  4877,  5079,  5284,  5493,  5706,  5924,  6145,  6371,  6600,  6834,  7072,  7313,  7559,  7809,  8063,
    8321,  8583,  8849,  9119,  9394,  9672,  9954,  10241, 10531, 10826, 11125, 11427, 11734, 12045, 12360, 12679,
    13002, 13329, 13660, 13995, 14335, 14678, 15025, 15377, 15732, 16092, 16456, 16823, 17195, 17571, 17951, 18335,
    18723, 19115, 19511, 19911, 20316, 20724, 21136, 21553, 21974, 22398, 22827, 23260, 23696, 24137, 24582, 25031,
    25484, 25941, 26402, 26868, 27337, 27810, 28288, 28769, 29255, 29744, 30238, 30736, 31238, 31744, 32254, 32768,
};

// FIXME: 98.6 %: the loop's temporaries get r3-r7 permuted (target: buffer in
// r3, sampleNum in r7, VolumeTable in r5); code otherwise identical.
static void MixAudio(s16* destination, s16* source, u32 sample) {
    if (ActivePlayer.open && ActivePlayer.internalState == 2 && ActivePlayer.audioExist) {
        u32 sampleNum;
        u32 requestSample;
        s32 i;
        s16* dst;
        s16* curPtr;
        s32 l_mix, r_mix;
        u16 attenuation;

        requestSample = sample;
        dst = destination;

        do {
            do {
                if (ActivePlayer.playAudioBuffer == NULL) {
                    if (!(ActivePlayer.playAudioBuffer = (THPAudioBuffer*)PopDecodedAudioBuffer(OS_MESSAGE_NOBLOCK))) {
                        memset(dst, 0, requestSample * 4);
                        return;
                    }
                    ActivePlayer.curAudioNumber++;
                }
            } while ((sampleNum = ActivePlayer.playAudioBuffer->validSample) == 0);

            if (sampleNum >= requestSample) {
                sampleNum = requestSample;
            }

            curPtr = ActivePlayer.playAudioBuffer->curPtr;
            for (i = 0; i < sampleNum; i++) {
                if (ActivePlayer.rampCount != 0) {
                    ActivePlayer.rampCount--;
                    ActivePlayer.curVolume += ActivePlayer.deltaVolume;
                } else {
                    ActivePlayer.curVolume = ActivePlayer.targetVolume;
                }

                attenuation = VolumeTable[(s32)ActivePlayer.curVolume];

                l_mix = 0.7f * (attenuation * curPtr[0] >> 15);
                if (l_mix < -0x8000) {
                    l_mix = -0x8000;
                }
                if (l_mix > 0x7FFF) {
                    l_mix = 0x7FFF;
                }
                dst[0] = l_mix;

                r_mix = 0.7f * (attenuation * curPtr[1] >> 15);
                if (r_mix < -0x8000) {
                    r_mix = -0x8000;
                }
                if (r_mix > 0x7FFF) {
                    r_mix = 0x7FFF;
                }
                dst[1] = r_mix;

                dst += 2;
                curPtr += 2;
            }

            requestSample -= sampleNum;
            ActivePlayer.playAudioBuffer->validSample -= sampleNum;
            ActivePlayer.playAudioBuffer->curPtr = curPtr;

            if (ActivePlayer.playAudioBuffer->validSample == 0) {
                PushFreeAudioBuffer(ActivePlayer.playAudioBuffer);
                ActivePlayer.playAudioBuffer = NULL;
            }

            if (requestSample == 0) {
                break;
            }
        } while (TRUE);
    } else {
        memset(destination, 0, sample * 4);
    }
}

static s16* audioCallback(s32 sample) {
    BOOL enabled;

    if (!ActivePlayer.open || ActivePlayer.internalState != 2 || !ActivePlayer.audioExist) {
        return NULL;
    }

    enabled = OSEnableInterrupts();
    SoundBufferIndex ^= 1;
    MixAudio(SoundBuffer[SoundBufferIndex], NULL, sample);
    OSRestoreInterrupts(enabled);
    return SoundBuffer[SoundBufferIndex];
}

static void initAudio() {
    JASDriver::registerMixCallback(audioCallback, MixMode_InterLeave);
}

static void quitAudio() {
    JASDriver::registerMixCallback(NULL, MixMode_Mono);
}

static void PushUsedTextureSet(OSMessage msg) {
    OSSendMessage(&UsedTextureSetQueue, msg, OS_MESSAGE_NOBLOCK);
}

static OSMessage PopUsedTextureSet() {
    OSMessage msg;
    if (OSReceiveMessage(&UsedTextureSetQueue, &msg, OS_MESSAGE_NOBLOCK) == TRUE) {
        return msg;
    }
    return NULL;
}

static BOOL THPPlayerInit(BOOL audioSystem) {
    BOOL enabled;

    memset(&ActivePlayer, 0, sizeof(THPPlayer));
    LCEnable();
    OSInitMessageQueue(&UsedTextureSetQueue, UsedTextureSetMessage, THP_TEXTURE_SET_COUNT);

    if (!THPInit()) {
        return FALSE;
    }

    enabled = OSDisableInterrupts();
    AudioSystem = audioSystem;
    SoundBufferIndex = 0;
    LastAudioBuffer = NULL;
    CurAudioBuffer = NULL;
    initAudio();
    OSRestoreInterrupts(enabled);

    if (!AudioSystem) {
        memset(SoundBuffer, 0, sizeof(SoundBuffer));
        DCStoreRange(SoundBuffer, sizeof(SoundBuffer));
    }

    Initialized = TRUE;
    return TRUE;
}

static void THPPlayerQuit() {
    LCDisable();
    quitAudio();
    Initialized = FALSE;
    ActivePlayer.dvdError = 0;
    ActivePlayer.videoError = 0;
}

static bool THPPlayerOpen(const char* fileName, BOOL onMemory) {
    s32 offset;
    u32 i;

    if (!Initialized) {
        return FALSE;
    }

    if (ActivePlayer.open) {
        return FALSE;
    }

    memset(&ActivePlayer.videoInfo, 0, sizeof(THPVideoInfo));
    memset(&ActivePlayer.audioInfo, 0, sizeof(THPAudioInfo));

    if (!DVDOpen(fileName, &ActivePlayer.fileInfo)) {
        return FALSE;
    }

    if (DVDReadPrio(&ActivePlayer.fileInfo, WorkBuffer, 64, 0, 2) < 0) {
        DVDClose(&ActivePlayer.fileInfo);
        return FALSE;
    }

    memcpy(&ActivePlayer.header, WorkBuffer, sizeof(THPHeader));

    if (strcmp(ActivePlayer.header.magic, "THP") != 0) {
        DVDClose(&ActivePlayer.fileInfo);
        return FALSE;
    }

    if (ActivePlayer.header.version != 0x11000) {
        DVDClose(&ActivePlayer.fileInfo);
        return FALSE;
    }

    offset = ActivePlayer.header.compInfoDataOffsets;
    if (DVDReadPrio(&ActivePlayer.fileInfo, WorkBuffer, 32, offset, 2) < 0) {
        DVDClose(&ActivePlayer.fileInfo);
        return FALSE;
    }

    memcpy(&ActivePlayer.compInfo, WorkBuffer, sizeof(THPFrameCompInfo));
    offset += sizeof(THPFrameCompInfo);
    ActivePlayer.audioExist = FALSE;

    for (i = 0; i < ActivePlayer.compInfo.numComponents; i++) {
        switch (ActivePlayer.compInfo.frameComp[i]) {
        case 0:
            if (DVDReadPrio(&ActivePlayer.fileInfo, WorkBuffer, 32, offset, 2) < 0) {
                DVDClose(&ActivePlayer.fileInfo);
                return FALSE;
            }
            memcpy(&ActivePlayer.videoInfo, WorkBuffer, sizeof(THPVideoInfo));
            offset += sizeof(THPVideoInfo);
            break;
        case 1:
            if (DVDReadPrio(&ActivePlayer.fileInfo, WorkBuffer, 32, offset, 2) < 0) {
                DVDClose(&ActivePlayer.fileInfo);
                return FALSE;
            }
            memcpy(&ActivePlayer.audioInfo, WorkBuffer, sizeof(THPAudioInfo));
            offset += sizeof(THPAudioInfo);
            ActivePlayer.audioExist = TRUE;
            break;
        default:
            return FALSE;
        }
    }

    ActivePlayer.internalState = 0;
    ActivePlayer.state = 0;
    ActivePlayer.playFlag = 0;
    ActivePlayer.onMemory = onMemory;
    ActivePlayer.open = TRUE;
    ActivePlayer.curVolume = 127.0f;
    ActivePlayer.targetVolume = ActivePlayer.curVolume;
    ActivePlayer.rampCount = 0;
    return TRUE;
}

static BOOL THPPlayerClose() {
    if (ActivePlayer.open && ActivePlayer.state == 0) {
        ActivePlayer.open = FALSE;
        DVDClose(&ActivePlayer.fileInfo);
        return TRUE;
    }
    return FALSE;
}

static BOOL THPPlayerSetBuffer(u8* buffer) {
    u32 i;
    u8* ptr;
    u32 ysize;
    u32 uvsize;

    if (ActivePlayer.open && ActivePlayer.state == 0) {
        ptr = buffer;
        if (ActivePlayer.onMemory) {
            ActivePlayer.movieData = buffer;
            ptr += ActivePlayer.header.movieDataSize;
        } else {
            for (i = 0; i < THP_READ_BUFFER_COUNT; i++) {
                ActivePlayer.readBuffer[i].ptr = ptr;
                ptr += OSRoundUp32B(ActivePlayer.header.bufsize);
            }
        }

        ysize = OSRoundUp32B(ActivePlayer.videoInfo.xSize * ActivePlayer.videoInfo.ySize);
        uvsize = OSRoundUp32B(ActivePlayer.videoInfo.xSize * ActivePlayer.videoInfo.ySize / 4);

        for (i = 0; i < THP_TEXTURE_SET_COUNT; i++) {
            ActivePlayer.textureSet[i].ytexture = ptr;
            DCInvalidateRange(ptr, ysize);
            ptr += ysize;
            ActivePlayer.textureSet[i].utexture = ptr;
            DCInvalidateRange(ptr, uvsize);
            ptr += uvsize;
            ActivePlayer.textureSet[i].vtexture = ptr;
            DCInvalidateRange(ptr, uvsize);
            ptr += uvsize;
        }

        if (ActivePlayer.audioExist) {
            for (i = 0; i < THP_AUDIO_BUFFER_COUNT; i++) {
                ActivePlayer.audioBuffer[i].buffer = (s16*)ptr;
                ActivePlayer.audioBuffer[i].curPtr = (s16*)ptr;
                ActivePlayer.audioBuffer[i].validSample = 0;
                ptr += OSRoundUp32B(ActivePlayer.header.audioMaxSamples * 4);
            }
        }

        ActivePlayer.thpWork = ptr;
        return TRUE;
    }

    return FALSE;
}

static void InitAllMessageQueue() {
    s32 i;

    if (!ActivePlayer.onMemory) {
        for (i = 0; i < THP_READ_BUFFER_COUNT; i++) {
            PushFreeReadBuffer(&ActivePlayer.readBuffer[i]);
        }
    }

    for (i = 0; i < THP_TEXTURE_SET_COUNT; i++) {
        PushFreeTextureSet(&ActivePlayer.textureSet[i]);
    }

    if (ActivePlayer.audioExist) {
        for (i = 0; i < THP_AUDIO_BUFFER_COUNT; i++) {
            PushFreeAudioBuffer(&ActivePlayer.audioBuffer[i]);
        }
    }

    OSInitMessageQueue(&PrepareReadyQueue, &PrepareReadyMessage, 1);
}

static bool ProperTimingForStart() {
    if (ActivePlayer.videoInfo.videoType & 1) {
        if (VIGetNextField() == VI_FIELD_BELOW) {
            return true;
        }
    } else if (ActivePlayer.videoInfo.videoType & 2) {
        if (VIGetNextField() == VI_FIELD_ABOVE) {
            return true;
        }
    } else {
        return true;
    }
    return false;
}

static bool ProperTimingForGettingNextFrame() {
    s32 frameRate;

    if (ActivePlayer.videoInfo.videoType & 1) {
        if (VIGetNextField() == VI_FIELD_BELOW) {
            return true;
        }
    } else if (ActivePlayer.videoInfo.videoType & 2) {
        if (VIGetNextField() == VI_FIELD_ABOVE) {
            return true;
        }
    } else {
        frameRate = ActivePlayer.header.frameRate * 100.0f;
        if (VIGetTvFormat() == VI_PAL) {
            ActivePlayer.curCount = ActivePlayer.retaceCount * frameRate / 5000;
        } else {
            ActivePlayer.curCount = ActivePlayer.retaceCount * frameRate / 5994;
        }

        if (ActivePlayer.prevCount != ActivePlayer.curCount) {
            ActivePlayer.prevCount = ActivePlayer.curCount;
            return true;
        }
    }
    return false;
}

static void PlayControl(u32 retraceCount) {
    THPTextureSet* decodedTexture;
    s32 curFrame;

    if (OldVIPostCallback != NULL) {
        OldVIPostCallback(retraceCount);
    }

    decodedTexture = (THPTextureSet*)-1;
    if (ActivePlayer.open && ActivePlayer.state == 2) {
        if (ActivePlayer.dvdError || ActivePlayer.videoError) {
            ActivePlayer.state = 5;
            ActivePlayer.internalState = 5;
            return;
        }

        if (++ActivePlayer.retaceCount == 0) {
            if (ProperTimingForStart()) {
                if (ActivePlayer.audioExist) {
                    if (ActivePlayer.curVideoNumber - ActivePlayer.curAudioNumber <= 1) {
                        decodedTexture = (THPTextureSet*)PopDecodedTextureSet(OS_MESSAGE_NOBLOCK);
                        ActivePlayer.videoDecodeCount--;
                        ActivePlayer.curVideoNumber++;
                    } else {
                        ActivePlayer.internalState = 2;
                    }
                } else {
                    decodedTexture = (THPTextureSet*)PopDecodedTextureSet(OS_MESSAGE_NOBLOCK);
                }
            } else {
                ActivePlayer.retaceCount = -1;
            }
        } else {
            if (ActivePlayer.retaceCount == 1) {
                ActivePlayer.internalState = 2;
            }

            if (ProperTimingForGettingNextFrame()) {
                if (ActivePlayer.audioExist) {
                    if (ActivePlayer.curVideoNumber - ActivePlayer.curAudioNumber <= 1) {
                        decodedTexture = (THPTextureSet*)PopDecodedTextureSet(OS_MESSAGE_NOBLOCK);
                        ActivePlayer.videoDecodeCount--;
                        ActivePlayer.curVideoNumber++;
                    }
                } else {
                    decodedTexture = (THPTextureSet*)PopDecodedTextureSet(OS_MESSAGE_NOBLOCK);
                }
            }
        }

        if (decodedTexture != NULL && decodedTexture != (THPTextureSet*)-1) {
            if (ActivePlayer.dispTextureSet != NULL) {
                PushUsedTextureSet(ActivePlayer.dispTextureSet);
            }
            ActivePlayer.dispTextureSet = decodedTexture;
        }

        if ((ActivePlayer.playFlag & 1) == 0) {
            if (ActivePlayer.audioExist) {
                curFrame = ActivePlayer.curAudioNumber + ActivePlayer.initReadFrame;
                if (curFrame == ActivePlayer.header.numFrames && ActivePlayer.playAudioBuffer == NULL) {
                    ActivePlayer.internalState = 3;
                    ActivePlayer.state = 3;
                }
            } else {
                if (ActivePlayer.dispTextureSet != NULL) {
                    curFrame = ActivePlayer.dispTextureSet->frameNumber + ActivePlayer.initReadFrame;
                } else {
                    curFrame = ActivePlayer.initReadFrame - 1;
                }

                if (curFrame == ActivePlayer.header.numFrames - 1 && decodedTexture == NULL) {
                    ActivePlayer.internalState = 3;
                    ActivePlayer.state = 3;
                }
            }
        }
    }
}

// 1: prepared, 0: preparing failed, -1: not ready yet.
static s32 WaitUntilPrepare() {
    OSMessage msg;

    if (OSReceiveMessage(&PrepareReadyQueue, &msg, OS_MESSAGE_NOBLOCK) == TRUE) {
        return msg != NULL;
    }
    return -1;
}

static void PrepareReady(u8 msg) {
    OSSendMessage(&PrepareReadyQueue, (OSMessage)msg, OS_MESSAGE_BLOCK);
}

static bool THPPlayerPrepare(s32 frame, u8 flag, s32 audioTrack) {
    if (ActivePlayer.open && ActivePlayer.state == 0) {
        if (frame > 0) {
            if (ActivePlayer.header.offsetDataOffsets == 0) {
                return FALSE;
            }

            if (ActivePlayer.header.numFrames > frame) {
                ActivePlayer.initReadFrame = frame;
                ActivePlayer.initOffset = ActivePlayer.header.movieDataOffsets + WorkBuffer[0];
                ActivePlayer.initReadSize = WorkBuffer[1] - WorkBuffer[0];
            } else {
                return FALSE;
            }
        } else {
            ActivePlayer.initOffset = ActivePlayer.header.movieDataOffsets;
            ActivePlayer.initReadSize = ActivePlayer.header.firstFrameSize;
            ActivePlayer.initReadFrame = frame;
        }

        if (ActivePlayer.audioExist) {
            if (audioTrack < 0 || audioTrack >= ActivePlayer.audioInfo.sndNumTracks) {
                return FALSE;
            }
            ActivePlayer.curAudioTrack = audioTrack;
        }

        ActivePlayer.playFlag = flag & 1;
        ActivePlayer.videoDecodeCount = 0;

        if (!ActivePlayer.onMemory) {
            CreateVideoDecodeThread(20, NULL);
            if (ActivePlayer.audioExist) {
                CreateAudioDecodeThread(12, NULL);
            }
            CreateReadThread(8);
        }

        InitAllMessageQueue();
        VideoDecodeThreadStart();
        if (ActivePlayer.audioExist) {
            AudioDecodeThreadStart();
        }
        if (!ActivePlayer.onMemory) {
            ReadThreadStart();
        }
        return TRUE;
    }

    return FALSE;
}

static void THPPlayerDrawDone() {
    void* textureSet;

    GXDrawDone();
    if (Initialized) {
        while (TRUE) {
            textureSet = PopUsedTextureSet();
            if (textureSet == NULL) {
                break;
            }
            PushFreeTextureSet(textureSet);
        }
    }
}

static BOOL THPPlayerPlay() {
    if (ActivePlayer.open && (ActivePlayer.state == 1 || ActivePlayer.state == 4)) {
        ActivePlayer.state = 2;
        ActivePlayer.prevCount = 0;
        ActivePlayer.curCount = 0;
        ActivePlayer.retaceCount = -1;
        return TRUE;
    }
    return FALSE;
}

static void THPPlayerStop() {
    if (ActivePlayer.open && ActivePlayer.state != 0) {
        ActivePlayer.internalState = 0;
        ActivePlayer.state = 0;
        VISetPostRetraceCallback(OldVIPostCallback);

        if (!ActivePlayer.onMemory) {
            DVDCancel(&ActivePlayer.fileInfo.cb);
            ReadThreadCancel();
        }

        VideoDecodeThreadCancel();

        if (ActivePlayer.audioExist) {
            AudioDecodeThreadCancel();
            quitAudio();
        }

        while (PopUsedTextureSet() != NULL) {
        }

        ActivePlayer.rampCount = 0;
        ActivePlayer.curVolume = ActivePlayer.targetVolume;
    }
}

static s32 THPPlayerDrawCurrentFrame(GXRenderModeObj* rmode, s32 x, s32 y, s32 polygonW, s32 polygonH) {
    if (ActivePlayer.open && ActivePlayer.state != 0 && ActivePlayer.dispTextureSet != NULL) {
        THPGXYuv2RgbSetup(rmode);
        THPGXYuv2RgbDraw(ActivePlayer.dispTextureSet->ytexture, ActivePlayer.dispTextureSet->utexture,
                         ActivePlayer.dispTextureSet->vtexture, x, y, ActivePlayer.videoInfo.xSize,
                         ActivePlayer.videoInfo.ySize, polygonW, polygonH);
        THPGXRestore();
        return (ActivePlayer.dispTextureSet->frameNumber + ActivePlayer.initReadFrame) % ActivePlayer.header.numFrames;
    }
    return -1;
}

static BOOL THPPlayerGetVideoInfo(THPVideoInfo* videoInfo) {
    if (ActivePlayer.open) {
        memcpy(videoInfo, &ActivePlayer.videoInfo, sizeof(THPVideoInfo));
        return TRUE;
    }
    return FALSE;
}

static BOOL THPPlayerGetAudioInfo(THPAudioInfo* audioInfo) {
    if (ActivePlayer.open) {
        memcpy(audioInfo, &ActivePlayer.audioInfo, sizeof(THPAudioInfo));
        return TRUE;
    }
    return FALSE;
}

static u32 THPPlayerGetTotalFrame() {
    if (ActivePlayer.open) {
        return ActivePlayer.header.numFrames;
    }
    return 0;
}

static s32 THPPlayerGetState() {
    return ActivePlayer.state;
}

static BOOL THPPlayerSetVolume(s32 vol, s32 duration) {
    s32 numSamples;
    BOOL enabled;

    if (ActivePlayer.open && ActivePlayer.audioExist) {
        numSamples = AIGetDSPSampleRate() == 0 ? 32 : 48;

        if (vol > 127) {
            vol = 127;
        }
        if (vol < 0) {
            vol = 0;
        }

        if (duration > 60000) {
            duration = 60000;
        }
        if (duration < 0) {
            duration = 0;
        }

        enabled = OSDisableInterrupts();
        ActivePlayer.targetVolume = vol;
        if (duration != 0) {
            ActivePlayer.rampCount = numSamples * duration;
            ActivePlayer.deltaVolume = (ActivePlayer.targetVolume - ActivePlayer.curVolume) / ActivePlayer.rampCount;
        } else {
            ActivePlayer.rampCount = 0;
            ActivePlayer.curVolume = ActivePlayer.targetVolume;
        }
        OSRestoreInterrupts(enabled);
        return TRUE;
    }
    return FALSE;
}

// --- The game's movie layer -------------------------------------------------

enum {
    THP_STATE_STOP = 0,
    THP_STATE_PREPARED = 1,
    THP_STATE_PLAY = 2,
    THP_STATE_PLAYED = 3,
    THP_STATE_PAUSE = 4,
    THP_STATE_ERROR = 5,
};

// Game singleton at 0x80529DEC (0x354 bytes, used by many units); only the
// heap pointer at 0x8 is known.
class cls_80529DEC {
public:
    JKRHeap* getHeap_8042E780() { return mHeap; }

    /* 0x0 */ u8 field_0x0[0x8];
    /* 0x8 */ JKRHeap* mHeap;
};

extern cls_80529DEC lbl_80529DEC;

inline JKRHeap* GetMovieHeap_8042E758() { return lbl_80529DEC.getHeap_8042E780(); }

static s32 sOffsetX_8053AEA4;
static s32 sOffsetY_8053AEA8;
static u8* sMovieBuffer_8053AEAC;
static s32 sAudioTrack_8053AEB0;
static const char* sMoviePath_8053AEB4;
static s32 sMovieState_8053AEB8;
static u8 sOpenFailed_8053AEBC;
static u8 sMovieFailed_8053AEBD;
static u16 sSavedFrameRate_8053AEBE;

static THPVideoInfo sVideoInfo_8052BEC0;
static THPAudioInfo sAudioInfo_8052BECC;

static bool IsMoviePlaying_804286A0() {
    return sMovieState_8053AEB8 == 2;
}

static u8 StartMovie_80428630() {
    THPPlayerInit(FALSE);

    sMovieBuffer_8053AEAC = (u8*)GetMovieHeap_8042E758()->alloc(0x1C0000, 0x20);
    if (sMovieBuffer_8053AEAC == NULL) {
        return false;
    }

    if (!CreateOpenThread_8042E29C(OSGetThreadPriority(OSGetCurrentThread()) + 1)) {
        return false;
    }

    OpenThreadStart_8042E278();
    return true;
}

void* THPReadThread_804B15F4::opener_804285F4(void* arg) {
    if (!THPPlayerOpen(sMoviePath_8053AEB4, FALSE)) {
        sOpenFailed_8053AEBC = TRUE;
    }
    return NULL;
}

bool THPReadThread_804B15F4::createOpen_8042858C(OSPriority prio) {
    if (OSCreateThread(&mThread, Opener_8042E250, NULL, mStack + sizeof(mStack), sizeof(mStack), prio, 1) == FALSE) {
        return FALSE;
    }
    mCreated = TRUE;
    return TRUE;
}

void THPReadThread_804B15F4::startOpen_8042855C() {
    if (mCreated) {
        OSResumeThread(&mThread);
    }
}

bool THPReadThread_804B15F4::isTerminated_8042852C() {
    return OSIsThreadTerminated(&mThread);
}

void THPReadThread_804B15F4::setCreated_80428524(BOOL created) {
    mCreated = created;
}

static bool PrepareMovie_80428448() {
    THPPlayerGetVideoInfo(&sVideoInfo_8052BEC0);
    THPPlayerGetAudioInfo(&sAudioInfo_8052BECC);
    sOffsetX_8053AEA4 = (JUTVideo::getManager()->getRenderMode()->fbWidth - sVideoInfo_8052BEC0.xSize) / 2;
    sOffsetY_8053AEA8 = (JUTVideo::getManager()->getRenderMode()->efbHeight - sVideoInfo_8052BEC0.ySize) / 2;
    THPPlayerSetBuffer(sMovieBuffer_8053AEAC);

    sAudioTrack_8053AEB0 = sAudioInfo_8052BECC.sndNumTracks != 1 ? OSGetTick() % sAudioInfo_8052BECC.sndNumTracks : 0;

    if (!THPPlayerPrepare(0, 0, sAudioTrack_8053AEB0)) {
        return false;
    }
    return true;
}

static bool OpenMovie_8042840C() {
    sMovieState_8053AEB8 = 0;
    sOpenFailed_8053AEBC = FALSE;
    return StartMovie_80428630();
}

static void QuitMovie_804283D4() {
    THPPlayerStop();
    THPPlayerClose();
    THPPlayerQuit();
    if (sMovieBuffer_8053AEAC != NULL) {
        JKRFree(sMovieBuffer_8053AEAC);
    }
}

static BOOL UpdateMovie_8042829C() {
    if (sMovieState_8053AEB8 == 0) {
        if (IsReadThreadTerminated_8042E22C()) {
            SetReadThreadCreated_8042E204(FALSE);
            if (sOpenFailed_8053AEBC) {
                return FALSE;
            }
            if (!PrepareMovie_80428448()) {
                return FALSE;
            }
            sMovieState_8053AEB8 = 1;
        }
    } else if (sMovieState_8053AEB8 == 1) {
        switch (WaitUntilPrepare()) {
        case 0:
            return FALSE;
        case 1:
            ActivePlayer.state = 1;
            ActivePlayer.internalState = 0;
            ActivePlayer.dispTextureSet = NULL;
            ActivePlayer.playAudioBuffer = NULL;
            ActivePlayer.curVideoNumber = 0;
            ActivePlayer.curAudioNumber = 0;
            OldVIPostCallback = VISetPostRetraceCallback(PlayControl);
            THPPlayerPlay();
            sMovieState_8053AEB8 = 2;
            break;
        }
    } else if (THPPlayerGetState() == THP_STATE_ERROR) {
        THPPlayerStop();
        THPPlayerClose();
        if (sMovieBuffer_8053AEAC != NULL) {
            JKRFree(sMovieBuffer_8053AEAC);
        }
        return FALSE;
    }
    return TRUE;
}

static void DrawMovie_804281FC(s32 x, s32 y, s32 w, s32 h) {
    if (IsMoviePlaying_804286A0()) {
        THPPlayerDrawCurrentFrame(JUTVideo::getManager()->getRenderMode(), sOffsetX_8053AEA4 + x, sOffsetY_8053AEA8 + y,
                                  sVideoInfo_8052BEC0.xSize + w, sVideoInfo_8052BEC0.ySize + h);
        THPPlayerDrawDone();
    }
}

// Frames left to play, or -1 when nothing is shown.
static s32 GetMovieRestFrame_80428134() {
    u32 frame;
    u32 total;

    if (sMovieFailed_8053AEBD || THPPlayerGetState() == THP_STATE_ERROR) {
        return 0;
    }

    if (ActivePlayer.open && ActivePlayer.dispTextureSet != NULL) {
        frame = (ActivePlayer.dispTextureSet->frameNumber + ActivePlayer.initReadFrame) % ActivePlayer.header.numFrames;
    } else {
        return -1;
    }

    total = THPPlayerGetTotalFrame();
    if (total == 0) {
        return 0;
    }
    if (total <= 1) {
        return 0;
    }
    return frame >= total - 1 ? 0 : total - 1 - frame;
}

static void SetMovieVolume_804280C0(f32 volume) {
    s32 vol;

    if (!sMovieFailed_8053AEBD) {
        if (volume >= 1.0f) {
            vol = 127;
        } else if (volume <= 0.0f) {
            vol = 0;
        } else {
            vol = volume / 127.0f;
        }
        THPPlayerSetVolume(vol, 0);
    }
}

// The movie object the game creates; named after its vtable.
class THPMovie_804B15D0 {
public:
    THPMovie_804B15D0();
    virtual ~THPMovie_804B15D0();
    BOOL draw_80427D2C(s32 x, s32 y, s32 w, s32 h);
    BOOL update_80427D60();
    BOOL quit_80427D94();
    BOOL start_80427DB4(s32 movie);
    BOOL drawIfOk_80427DD4(s32 x, s32 y, s32 w, s32 h);
    BOOL updateIfOk_80427E08();
    BOOL quitMovie_80427E28();
    BOOL startMovie_80427E58(s32 movie);

    /* 0x04 */ THPReadThread_804B15F4* mReadThread;
    /* 0x08 */ THPVideoDecodeThread_804B15E8* mVideoDecodeThread;
    /* 0x0C */ THPAudioDecodeThread_804B15DC* mAudioDecodeThread;
    /* 0x10 */ s32 (*mGetRestFrame)();
    /* 0x14 */ void (*mSetVolume)(f32);
    /* 0x18 */ u32 (*mGetTotalFrame)();
};

THPMovie_804B15D0::THPMovie_804B15D0() {
    mReadThread = new THPReadThread_804B15F4;
    mVideoDecodeThread = new THPVideoDecodeThread_804B15E8;
    mAudioDecodeThread = new THPAudioDecodeThread_804B15DC;
}

THPMovie_804B15D0::~THPMovie_804B15D0() {
    delete mReadThread;
    mReadThread = NULL;
    delete mVideoDecodeThread;
    mVideoDecodeThread = NULL;
    delete mAudioDecodeThread;
    mAudioDecodeThread = NULL;
}

static const char* sMovieFiles_804B15C0[] = {
    "/GC4Sword_usa/thpdemo/hyl.thp",
    "/GC4Sword_usa/thpdemo/sha.thp",
    "/GC4Sword_usa/thpdemo/nav.thp",
    "/GC4Sword_usa/thpdemo/4swords_title_us.thp",
};

static const char* GetMoviePath_80427F34(s32 movie) {
    if (movie >= 0 && movie < 4) {
        return sMovieFiles_804B15C0[movie];
    }
    return NULL;
}

BOOL THPMovie_804B15D0::startMovie_80427E58(s32 movie) {
    const char* path;

    sSavedFrameRate_8053AEBE = JFWDisplay::getManager()->getFrameRate();
    JFWDisplay::getManager()->setFrameRate(1);
    sMovieFailed_8053AEBD = FALSE;
    mGetRestFrame = GetMovieRestFrame_80428134;
    mSetVolume = SetMovieVolume_804280C0;
    mGetTotalFrame = THPPlayerGetTotalFrame;

    path = GetMoviePath_80427F34(movie);
    if (path != NULL) {
        sMoviePath_8053AEB4 = path;
        if (!OpenMovie_8042840C()) {
            sMovieFailed_8053AEBD = TRUE;
            return FALSE;
        }
        return TRUE;
    }

    sMoviePath_8053AEB4 = GetMoviePath_80427F34(0);
    if (!OpenMovie_8042840C()) {
        sMovieFailed_8053AEBD = TRUE;
        return FALSE;
    }
    return TRUE;
}

BOOL THPMovie_804B15D0::quitMovie_80427E28() {
    JFWDisplay::getManager()->setFrameRate(sSavedFrameRate_8053AEBE);
    QuitMovie_804283D4();
    return TRUE;
}

BOOL THPMovie_804B15D0::updateIfOk_80427E08() {
    return UpdateMovie_8042829C();
}

BOOL THPMovie_804B15D0::drawIfOk_80427DD4(s32 x, s32 y, s32 w, s32 h) {
    DrawMovie_804281FC(x, y, w, h);
    return TRUE;
}

BOOL THPMovie_804B15D0::start_80427DB4(s32 movie) {
    return startMovie_80427E58(movie);
}

BOOL THPMovie_804B15D0::quit_80427D94() {
    return quitMovie_80427E28();
}

BOOL THPMovie_804B15D0::update_80427D60() {
    if (sMovieFailed_8053AEBD) {
        return FALSE;
    }
    return updateIfOk_80427E08();
}

BOOL THPMovie_804B15D0::draw_80427D2C(s32 x, s32 y, s32 w, s32 h) {
    if (sMovieFailed_8053AEBD) {
        return FALSE;
    }
    return drawIfOk_80427DD4(x, y, w, h);
}
