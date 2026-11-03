#ifndef GAME_TU_8012FF9C_H
#define GAME_TU_8012FF9C_H

// Exports of game/tu_8012FF9C.cpp. Called by cls_8049EA00::init
// (game/tu_8012FBCC.cpp).

#include "dolphin/types.h"

// The guarded static at 0x8053AAC4 (function-local static of fn_8012FF9C; .sbss, so
// 8 bytes: the symbol's 0xC is padding to the unit's section end).
// No vtable; named after its destructor fn_8012FFF4, which is passed to
// __register_global_object. The constructor is inlined into fn_8012FF9C.
struct cls_8012FFF4 {
    cls_8012FFF4() : mCount(0) {}
    ~cls_8012FFF4() {}
    // True when the tracked actor is gone or is not an 'OGST' actor, or
    // else whatever cls_8049EA00::fn_8012FC2C returns for v.
    bool fn_80130030(const s32* v);
    // When value is positive: stores it at index mCount counted from
    // &mSerial (so the first call overwrites mSerial, later ones mCount
    // itself) and bumps mCount.
    void fn_801300B4(s32 value);
    // Forgets the tracked actor.
    void fn_801300D8();

    /* 0x0 */ s32 mSerial; // serial of the actor to look up
    /* 0x4 */ s32 mCount;
}; // Size: 0x8

cls_8012FFF4* fn_8012FF9C();

#endif
