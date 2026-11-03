#include "game/GameAudio.h"
#include "game/ActorBase.h"

void ActorBase_804A5770::fn_8019CEAC(s32 v) {
    field_0xb0 = v;
}

s32 ActorBase_804A5770::fn_8019CEB4() const {
    return field_0xb0;
}

extern "C" void fn_8019C4F4(JGeometry::TVec2f* v, f32 x, f32 y) {
    v->x = x;
    v->y = y;
}
