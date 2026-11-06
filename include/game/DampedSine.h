#ifndef GAME_DAMPED_SINE_H
#define GAME_DAMPED_SINE_H

#include "types.h"

// Owner: game/tu_801E4A18.cpp
struct DampedSine_801E4A18 {
    /* 0x00 */ f32 value;
    /* 0x04 */ u16 remaining;
    /* 0x06 */ u16 duration;
    /* 0x08 */ s16 angleStep;
    /* 0x0C */ f32 amplitude;
    /* 0x10 */ f32 decay;
}; // Size: 0x14

void fn_801E4B0C(DampedSine_801E4A18* s);
void fn_801E4AB0(DampedSine_801E4A18* s, u16 duration, s16 angleStep, f32 amplitude);
f32 fn_801E4A18(DampedSine_801E4A18* s);

#endif
