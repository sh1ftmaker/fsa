#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameState.h"
#include "game/GameGlobals.h"

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

extern "C" void fn_8019CECC(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
}

extern "C" void fn_8019CDC4(s32 v) {
    GameRoot_8049EF20* root = lbl_8053AAF8;
    root->field_0x85 = 1;
    root->field_0x74 = 0;
    root->field_0x78 = 0;
    root->field_0x54 = v;
}

extern "C" void fn_8019CEE8(u8 v) {
    lbl_80529DEC.field_0x24d = v;
}
