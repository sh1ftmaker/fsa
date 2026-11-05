#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "game/act/act_kaepora_gaebora.h"


// The second pass of each sprite draw: the same frame again, translucent,
// unless the actor's own sprite matrix is set.
static inline void drawSecond(cls_804A6550* self, SpriteParams_801F5AE0& params, s32 frame) {
    params.field_0x10.mColor.a = 200;
    params.field_0x1d = 0;
    if (self->mSpriteParams.mMtx == NULL) {
        fn_801EF830(self->field_0x4, &self->mPos, &params, 0x407, "enemy_gc_sob.bin", frame, NULL);
    }
}

void cls_804A6550::draw() {
    mPos.y += 6.0f;
    if (fn_80219E04()) {
        fn_8021B1AC();
    } else {
        switch (mState) {
        case 4:
        case 5:
        case 11:
        case 12:
            fn_8021AFB8();
            break;
        }
    }
    mPos.y -= 6.0f;
}

// FIXME: 93.4%. Same instructions bar one; the original keeps the zero of the
// SpriteParams reset in r30 (the saved register that also holds `shift`
// across the fn_80219E04 call) and loads it once (li r30, 0 before the first
// zero store); here the reset stores use r6 and `shift` gets its own li r30, 0.
void cls_804A6550::fn_8021AFB8() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    params.field_0x1f = 0;
    s8 shift = 0;
    if (fn_80219E04()) {
        shift = -6;
    }
    params.field_0x1d = shift;
    if (mFlags & 0x2000) {
        mtx[0][3] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][3] = 0.0f;
        mtx[0][0] = -1.0f;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 1.0f;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = 1.0f;
        params.mMtx = mtx;
    }
    drawSprite_801F06F0("enemy_gc_sob.bin", mAnim.mCel + 0x25C, &params, -1, 1.0f, 1.0f);
    drawSecond(this, params, mAnim.mCel + 0x25C);
}

// FIXME: 98.9%. Same as fn_8021AFB8: the SpriteParams reset's zero stores use
// r6 here, the original shares r30 (the `shift` register) and has one li less.
void cls_804A6550::fn_8021B1AC() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    params.field_0x1f = 0;
    s8 shift = 0;
    if (fn_80219E04()) {
        shift = -6;
    }
    params.field_0x1d = shift;
    drawSprite_801F06F0("enemy_gc_sob.bin", 0x279, &params, -1, 1.0f, 1.0f);
    drawSecond(this, params, 0x279);

    params.reset();
    params.field_0x1f = 0;
    shift = 0;
    if (fn_80219E04()) {
        shift = -6;
    }
    params.field_0x1d = shift + 1;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    f32 sinA = JMASSin(mHead.mAngle);
    f32 cosA = JMASCos(mHead.mAngle);
    mtx[0][0] = cosA;
    mtx[0][1] = 0.0f;
    mtx[0][2] = sinA;
    mtx[1][0] = 0.0f;
    mtx[1][1] = 1.0f;
    mtx[1][2] = 0.0f;
    mtx[2][0] = -sinA;
    mtx[2][1] = 0.0f;
    mtx[2][2] = cosA;
    params.mMtx = mtx;
    mPos.y += -21.0f;
    drawSprite_801F06F0("enemy_gc_sob.bin", 0x27A, &params, -1, 1.0f, 1.0f);
    mPos.y -= -21.0f;
    params.field_0x10.mColor.a = 200;
    params.field_0x1d = 0;
    mPos.y += -10.5f;
    if (mSpriteParams.mMtx == NULL) {
        fn_801EF830(field_0x4, &mPos, &params, 0x407, "enemy_gc_sob.bin", 0x27A, NULL);
    }
    mPos.y -= -10.5f;
}

void cls_804A6550::fn_8021B548() {
    s8 shift = 0;
    if (fn_80219E04()) {
        shift = -6;
    }
    fn_801CB71C(&mEmitter, &mPos, 0, shift);
}

void cls_804A6550::fn_8021B5A0() {
    fn_801CB7A0(mEmitter);
}

void cls_804A6550::fn_8021B5C4() {
    deleteEmitter(&mEmitter);
    s32 layer = field_0x4;
    s8 shift = 0;
    if (fn_80219E04()) {
        shift = -6;
    }
    mEmitter = spawnEmitter_8013CC50(layer, &mPos, 0x344, 0, shift, NULL, NULL, true, 1.0f);
}

void cls_804A6550::fn_8021B668() const {
    s32 layer = field_0x4;
    s8 shift = 0;
    if (fn_80219E04()) {
        shift = -6;
    }
    spawnEmitter_8013CC50(layer, &mPos, 0x343, 0, shift, NULL, NULL, true, 1.0f);
}
