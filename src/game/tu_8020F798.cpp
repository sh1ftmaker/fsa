#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/Effect.h"
#include "game/EnemyBase.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_thief.h"
#include "JSystem/JParticle/JPAEmitter.h"

// The sprite is drawn at its own position and then put back.
// FIXME: 99.31 %. Only the registers differ: the target keeps the table base in
// r5 and the entry offset (mulli) in r4, this code the other way round.
void Thief_804A5A30::draw() {
    const Thief_804A5A30::TypeParams& type = lbl_8046A238[mType];
    JGeometry::TVec3f pos = mPos;
    fn_8020F80C(type.mSpriteFrame + mAnim.mCel);
    mPos.set(pos.x, pos.y, pos.z);
}

void Thief_804A5A30::fn_8020F80C(s32 frame) {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    if (field_0xc4 != 0) {
        u8 color = fn_8023BCD4(field_0xc8);
        params.field_0x10.mColor.a = color;
        if (color == 0) {
            return;
        }
    }
    params.field_0x1c = 0;
    params.field_0x1d = 0;
    if (mSpriteParams.mMtx == NULL) {
        fn_801EF830(field_0x4, &mPos, &params, 0, "kage_sob.bin", 0, NULL);
    }
    s32 mode = 0;
    params.field_0x1f = 0;
    if (field_0xc4 != 0) {
        params.field_0x20 = 0;
        params.field_0x1c = fn_8023CA88(field_0xc8);
        params.field_0x1d = fn_8023CB24(field_0xc8) + 1;
        mtx[0][3] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][3] = 0.0f;
        mtx[0][0] = 1.0f;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 1.0f;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = -1.0f;
        params.mMtx = mtx;
        mode = 2;
        mPos.z -= lbl_8046A238[mType].mHeight;
    }
    if (field_0x4 < 8) {
        void* res = lbl_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both_sob.bin");
        lbl_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
    }
    if (field_0x4 == 8) {
        JGeometry::TVec3f pos;
        fn_801CD0D0(&mPos, &pos);
        fn_801EF574("enemy_both_sob.bin", frame, &pos, &params);
    }
    fn_80307AC8(mSerial, field_0x4, &mPos, frame, false, mode, -1);
}

void Thief_804A5A30::fn_8020FADC() {
    bool release = false;
    if (fn_8022461C(&mPos, field_0x4) == 0x39) {
        bool moving = false;
        if (field_0xc4 == 0 && mPos.z <= 0.0f) {
            if (__fabsf(mPos.x - mPrevPos.x) > 0.001f || __fabsf(mPos.y - mPrevPos.y) > 0.001f) {
                moving = true;
            }
        }
        if (moving) {
            startSound_801F0E34(0x4007F, 0);
            if (mEmitter358 == NULL) {
                mEmitter358 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x4D2, 0, 0, NULL, NULL, true, 1.0f);
            }
        } else {
            release = true;
        }
    } else {
        release = true;
    }
    if (release && mEmitter358 != NULL && !mEmitter358->checkStatus(JPAEMIT_Immortal)) {
        fn_801CB7A0(mEmitter358);
    }
    if (mEmitter358 != NULL) {
        fn_801CB71C(&mEmitter358, &mPos, 0, 0);
    }
}
