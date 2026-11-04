#include "game/ActorBase.h"
#include "game/GameAudio.h"

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
