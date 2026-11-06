#include "game/GameAudio.h"
#include "JSystem/JMath/JMath.h"
#include "game/DampedSine.h"

void fn_801E4B0C(DampedSine_801E4A18* s) {
    s->value = 0.0f;
    s->remaining = 0;
    s->duration = 0;
    s->angleStep = 0;
    s->amplitude = 0.0f;
    s->decay = 0.0f;
}

void fn_801E4AB0(DampedSine_801E4A18* s, u16 duration, s16 angleStep, f32 amplitude) {
    if (duration != 0) {
        s->value = 0.0f;
        s->duration = duration;
        s->remaining = duration;
        s->angleStep = angleStep;
        s->amplitude = amplitude;
        s->decay = s->amplitude * (1.0f / s->duration);
    }
}

f32 fn_801E4A18(DampedSine_801E4A18* s) {
    if (s->remaining != 0) {
        s->remaining--;
        if (s->remaining == 0) {
            s->value = 0.0f;
            s->remaining = 0;
            s->duration = 0;
            s->angleStep = 0;
            s->amplitude = 0.0f;
            s->decay = 0.0f;
        } else {
            s->amplitude -= s->decay;
            s->value = s->amplitude * JMASinShort(s->angleStep * (s->duration - s->remaining));
        }
    } else {
        s->value = 0.0f;
    }
    return s->value;
}
