#include "game/ActorBase.h"
#include "game/GameAudio.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/cls_804A1630.h"

// The actor's destructor: unless field_0x90 is set, it turns the view's flag
// off and calls fn_8022ADCC; then it releases its two emitters.
cls_804A1630::~cls_804A1630() {
    if (field_0x90 == 0) {
        fn_802D6A9C()->fn_802D7440(true);
        fn_8022ADCC();
    }
    for (s32 i = 0; i < 2; i++) {
        if (mEmitter318[i] != NULL) {
            fn_801EE434(mEmitter318[i]);
        }
    }
}

void ActorBase_804A5770::fn_8017EF40() {
    fn_801F0E34(0x40038, 0);
    vfunc_0x28(2, 0.0f, -24.0f);
}

// Copies a TVec2f from src into dst. Like fn_8017EADC, the original keeps it
// as a plain global function taking both vectors by pointer.
extern "C" void fn_80182690(JGeometry::TVec2f* dst, const JGeometry::TVec2f* src) {
    dst->x = src->x;
    dst->y = src->y;
}

// The view's box for an actor layer: mVisibleBox for layers 0-7, field_0xdc
// for the rest.
extern "C" JGeometry::TBox2f* fn_801826A4(s32 layer) {
    GameView_804A9950* view = fn_802D6A9C();
    if (layer < 8) {
        return &view->mVisibleBox;
    }
    return &view->field_0xdc;
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
