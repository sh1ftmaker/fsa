#include "game/GameAudio.h"
#include "JSystem/JGeometry.h"

// FIXME(shotgun): 100 %, matches
void fn_801583E4(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    dst->set(src->x, src->y, src->z);
}

// FIXME(shotgun): 100 %, matches
void fn_801583D4(JGeometry::TVec3f* dst, f32 x, f32 y, f32 z) {
    dst->set(x, y, z);
}
