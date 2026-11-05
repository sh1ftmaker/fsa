#ifndef GAME_ACT_TU_8011E96C_H
#define GAME_ACT_TU_8011E96C_H

// Code: game/tu_8011E96C.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x8049DE90.
class cls_8049DE90 : public ActorBase_804A5770 {
public:
    cls_8049DE90() : mState(1), mCycle(0), mTimer(0), mDuration(0), mNext(0) {}
    /* 0x08 */ virtual ~cls_8049DE90();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x230 */ s32 mState;      // 0 waiting for the bit, 1 counting down, 2 finished
    /* 0x234 */ s32 mCycle;      // parameter bit 6: cycle the table in order
    /* 0x238 */ s32 mTimer;
    /* 0x23C */ s32 mDuration;   // (parameter & 0x3F) * 10 frames
    /* 0x240 */ s32 mNext;       // next table entry when cycling
    /* 0x244 */ s32 mTable[4];   // parameter bits 7-26, 5 bits each
}; // Size: 0x254

#endif
