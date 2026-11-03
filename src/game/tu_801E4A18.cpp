#include "game/GameAudio.h"
#include "JSystem/JMath/JMath.h"

// Damped sine oscillation (provisional name, no evidence for a real one):
// `amplitude` falls linearly by `decay` each tick while the sine of the
// elapsed time advances `angleStep` per tick. Callers: act_vaati_boss
// (0x801D68E8..), tu_801D6CEC (0x801D6D24).
struct DampedSine_801E4A18 {
    /* 0x00 */ f32 value;
    /* 0x04 */ u16 remaining;
    /* 0x06 */ u16 duration;
    /* 0x08 */ s16 angleStep;
    /* 0x0C */ f32 amplitude;
    /* 0x10 */ f32 decay;
    // size 0x14
};

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

void fn_801E4B0C(DampedSine_801E4A18* s) {
    s->value = 0.0f;
    s->remaining = 0;
    s->duration = 0;
    s->angleStep = 0;
    s->amplitude = 0.0f;
    s->decay = 0.0f;
}
