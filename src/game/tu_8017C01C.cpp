#include "game/GameAudio.h"

// Stores x, y, z into v and returns it (the caller uses the result as the
// source of a copy). Not a JGeometry member: its object is passed by pointer
// and the original keeps it as a plain global function.
extern "C" JGeometry::TVec3f* fn_8017EB0C(JGeometry::TVec3f* v, f32 x, f32 y, f32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
    return v;
}
