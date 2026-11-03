#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"

void ActorBase_804A5770::fn_8017EAF8(const JGeometry::TVec3f& v) {
    field_0x3c = v;
}

// Stores x, y, z into v and returns it (the caller uses the result as the
// source of a copy). Not a JGeometry member: its object is passed by pointer
// and the original keeps it as a plain global function.
extern "C" JGeometry::TVec3f* fn_8017EB0C(JGeometry::TVec3f* v, f32 x, f32 y, f32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
    return v;
}

// Sets the map's field_0x216 flag (lbl_8053AB10->0x20, cls_804AA858; 9 units
// set it to 1). Takes no arguments; the caller is the fn_8017C4C8 branch of
// the actor update.
extern "C" void fn_8017ED08() {
    lbl_8053AB10->field_0x20->field_0x216 = 1;
}

// Stores x, y, z, w into v and returns it, like fn_8017EB0C does for a TVec3f.
// The caller passes a stack TVec4f and uses the returned pointer as the copy source.
extern "C" JGeometry::TVec4f* fn_8017EC5C(JGeometry::TVec4f* v, f32 x, f32 y, f32 z, f32 w) {
    v->x = x;
    v->y = y;
    v->z = z;
    v->w = w;
    return v;
}
