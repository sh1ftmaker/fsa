// The actor at vtable 0x804A1E50 (include/game/cls_804A1E50.h) and the
// helpers its unit shares.

#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/cls_804A1E50.h"
#include "JSystem/JParticle/JPAEmitter.h"

void cls_804A1E50::vfunc_0x24() {
    startSound_801F0E34(0x40015, 0);
    vfunc_0x28(2, 0.0f, 16.0f);
}

cls_804A1E50::~cls_804A1E50() {
    releaseEmitter_801EE434(field_0x328);
    releaseEmitter_801EE434(field_0x32c);
    releaseEmitter_801EE434(field_0x330);
    if (field_0x26c == 0) {
        getGameView_802D6A9C()->fn_802D7440(true);
        fn_8022ADCC();
    }
}

s32 fn_8018E89C() {
    return lbl_8053AAF8->fn_80138A30();
}

// Out-of-line copy of TVec3<f32>::set(const TVec3&).
void fn_8018E8C0(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    dst->set(*src);
}

void fn_8018E8DC(s32 bit, bool set, u8 value) {
    getGameView_802D6A9C()->fn_802D8050(bit, set, value);
}

bool fn_8018E930(s32 bit) {
    return getGameView_802D6A9C()->fn_802D800C(bit);
}

// Out-of-line copy of TVec2<f32>::set(x, y).
void fn_8018E964(JGeometry::TVec2f* v, f32 x, f32 y) {
    v->set(x, y);
}
