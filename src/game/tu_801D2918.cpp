#include "game/GameAudio.h"
#include "JSystem/JGeometry.h"

// Timed 2D velocity: frames counter plus a velocity that is damped as it ticks.
// Provisional name; no owning class known (callers: act_thief 0x80205C28, tu_80206B30 0x80206E7C; no vtable).
struct Damped2D_801D2918 {
    /* 0x0 */ s32 frames;
    /* 0x4 */ JGeometry::TVec2f vel;
    /* size 0xC */
};

// Decays vel by 0.78 per frame while frames > 0; returns 1 when it expires.
bool fn_801D2918(Damped2D_801D2918* self) {
    bool expired = false;
    if (self->frames > 0) {
        self->frames--;
        self->vel.x *= 0.78f;
        self->vel.y *= 0.78f;
    }
    if (self->frames <= 0) {
        self->frames = 0;
        self->vel.x = 0.0f;
        self->vel.y = 0.0f;
        expired = true;
    }
    return expired;
}

// Sets vel to the direction from b to a, normalized and scaled by 16 * scale.
void fn_801D2978(Damped2D_801D2918* self, const JGeometry::TVec2f* a, const JGeometry::TVec2f* b, s32 frames, f32 scale) {
    self->vel.set(a->x - b->x, a->y - b->y);
    self->vel.normalize();
    self->vel.scale(16.0f * scale);
    self->frames = frames;
}
