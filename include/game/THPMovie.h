#ifndef GAME_THPMOVIE_H
#define GAME_THPMOVIE_H

#include "types.h"

// game/tu_80427D2C.cpp: the game's THP movie layer.
class THPReadThread_804B15F4;
class THPVideoDecodeThread_804B15E8;
class THPAudioDecodeThread_804B15DC;

// The movie object the game creates; named after its vtable. Users:
// tu_80427D2C (definition), tu_803DAC3C (the game mode screen's movie).
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
    /* 0x10 */ u32 (*mGetRestFrame)(); // tu_803DAC3C tests it with cmplwi
    /* 0x14 */ void (*mSetVolume)(f32);
    /* 0x18 */ u32 (*mGetTotalFrame)();
}; // Size: 0x1C

// game/tu_80427D2C.cpp (0x804286A0): the movie state is 2 (playing).
bool IsMoviePlaying_804286A0();

#endif
