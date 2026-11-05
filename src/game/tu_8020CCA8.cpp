#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/Effect.h"
#include "game/EnemyBase.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/act/tu_801DE714.h"

// Nothing is drawn in state 0, or in state 0xE while the scale (field_0x54) is
// zero; from state 0xE the sprite is scaled by field_0x54.
// FIXME: 98.83%. Only the loads of field_0x54.y and .x for the scale matrix are
// swapped (the target loads y first; same as PushSwitchInPyramid_804AE8F4::draw).
void cls_804A51E4::draw() {
    if (mState == 0) {
        return;
    }
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    params.field_0x2d = 1;
    if (mState == 0xE) {
        if (field_0x54.squared() <= 32.0f * FLT_EPSILON) {
            return;
        }
        mtx[0][3] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][3] = 0.0f;
        mtx[0][0] = field_0x54.x;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = field_0x54.y;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = field_0x54.z;
        params.mMtx = mtx;
    }
    s32 frame = mAnim.mCel + 0xC0;
    s32 flags = 7;
    if (fn_801E1D7C()) {
        if (fn_8022461C(&mPos, field_0x4) == 0x39) {
            flags = 0;
        }
    }
    if (mSpriteParams.mMtx == NULL) {
        fn_801EF830(field_0x4, &mPos, &params, flags, "enemy_both2_sob.bin", frame, NULL);
    }
    if (field_0x4 < 8) {
        void* res = lbl_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
    }
    if (field_0x4 == 8) {
        JGeometry::TVec3f pos;
        fn_801CD0D0(&mPos, &pos);
        fn_801EF574("enemy_both2_sob.bin", frame, &pos, &params);
    }
    fn_803075AC(mSerial, field_0x4, &mPos, frame, false, 2, -1, 1, 0, 0);
}

void cls_804A51E4::fn_8020CF7C() const {
    spawnEmitter_8013CC50(field_0x4, &mPos, 0x48D, 0, 0, NULL, NULL, true, 1.0f);
}

void cls_804A51E4::fn_8020CFC4() const {
    switch (fn_8022461C(&mPos, field_0x4)) {
    case 0xD:
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x3D0, 0, 1, NULL, NULL, true, 1.2f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x15B, 0, 1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x15C, -1, 0, NULL, NULL, true, 1.0f);
        startSound_801F0E34(0x30022, 0);
        break;
    case 0xE:
        if (fn_801CD2F8(field_0x4)) {
            startSound_801F0E34(0x4003E, 0);
        } else {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x2A9, -1, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x2AA, 0, 1, NULL, NULL, true, 1.0f);
            if (fn_802DEB58()) {
                startSound_801F0E34(0x30022, 0);
            } else {
                startSound_801F0E34(0x5005F, 0);
            }
        }
        break;
    default:
        startSound_801F0E34(0x4003E, 0);
        break;
    }
}

void cls_804A51E4::fn_8020D16C() const {
    if (fn_8022461C(&mPos, field_0x4) == 0x39) {
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x15B, 0, 1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x15C, -1, 0, NULL, NULL, true, 1.0f);
        startSound_801F0E34(0x400E0, 0);
    } else if (fn_80224B88(&mPos, field_0x4)) {
        startSound_801F0E34(0x400E1, 0);
    } else if (fn_8022517C(&mPos, field_0x4)) {
        startSound_801F0E34(0x400DF, 0);
    } else if (fn_80224E88(&mPos, field_0x4)) {
        startSound_801F0E34(0x50071, 0);
    } else {
        startSound_801F0E34(0x4004F, 0);
    }
}

// FIXME: 94.47%. Only the copy of mPos differs: the target loads z (lfs 0x14)
// before the psq_l of x, y and stores z first; this code copies in the other
// order.
void cls_804A51E4::fn_8020D2AC() {
    JGeometry::TVec3f pos(mPos);
    pos.y -= 26.0f;
    if (!fn_801E1D9C()) {
        switch (field_0x98) {
        case 0:
            pos.x += 6.0f;
            break;
        case 1:
            pos.x -= 6.0f;
            break;
        }
    }
    spawnEmitter_8013CC50(field_0x4, &pos, 0x33A, 0, 1, NULL, NULL, true, 1.0f);
}
