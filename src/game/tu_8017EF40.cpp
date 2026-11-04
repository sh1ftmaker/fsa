#include "game/ActorBase.h"
#include "game/GameAudio.h"

// Copies a TVec2f from src into dst. Like fn_8017EADC, the original keeps it
// as a plain global function taking both vectors by pointer.
extern "C" void fn_80182690(JGeometry::TVec2f* dst, const JGeometry::TVec2f* src) {
    dst->x = src->x;
    dst->y = src->y;
}

// Stores an (x, y) pair into a TVec2f; the callers pass a stack vector.
void fn_801826E4(JGeometry::TVec2f* v, f32 x, f32 y) {
    v->x = x;
    v->y = y;
}

void fn_80182640(JGeometry::TVec3f* v, f32 x, f32 y, f32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

void fn_80182650(JGeometry::TVec3f* v, f32 x, f32 y, f32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

void ActorBase_804A5770::fn_80182660(const JGeometry::TVec3f& pos) {
    mPos = pos;
}

// Copies a TVec3f from src into dst, like fn_80182690 does for TVec2f.
extern "C" void fn_80182674(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
}
