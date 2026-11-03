#include "game/GameAudio.h"
#include "game/cls_804B1D58.h"

u8 cls_804B1D58::fn_8044CB00() {
    return field_0x140;
}

u8 cls_804B1D58::fn_8044D290() {
    return field_0x140;
}

// Stores x, y, z into v and returns it (the caller passes a stack TVec3f and
// uses the returned pointer as the copy source). Like fn_8017EB0C, the original
// keeps it as a plain global function, not a JGeometry member.
extern "C" JGeometry::TVec3f* fn_80453874(JGeometry::TVec3f* v, f32 x, f32 y, f32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
    return v;
}
