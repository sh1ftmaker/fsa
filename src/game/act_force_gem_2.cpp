#include "game/GameAudio.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/RotMatrix.h"
#include "game/act/act_force_gem_2.h"

// Frame offsets indexed by field_0x264 (a writable table, .data in the original).
s32 lbl_804AB810[8] = {1, 5, 9, 13, 17, 21, 25, 29};
// Effect / sound ids by field_0x264 (passed to fn_802362C4).
s32 lbl_804AB830[8] = {1, 5, 20, 50, 100, 150, 200, 300};

// FIXME: 99.25 %. Register allocation only: the original keeps the scale (field_0x238)
// in f29 and the cos of the zero angle in f31; ours swap them, so the products and the
// sin / cos results shift registers. Loading the scale before or after the rotation
// vector does not change it.
void ForceGem2_804AB850::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    params.field_0x2d = 1;
    params.field_0x1d = 1;
    if (mState == 5 || mState == 6) {
        mtx[0][3] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][3] = 0.0f;
        JGeometry::TVec3f rot(0.0f, field_0x23c, 0.0f);
        f32 scale = field_0x238;
        setRotScaleMtx(mtx, rot, scale);
        params.mMtx = mtx;
        s32 frame = field_0x24c + lbl_804AB810[field_0x264];
        if (field_0x4 < 8) {
            void* res = lbl_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
            lbl_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
        }
        return;
    }
    if (mState != 2) {
        s32 flags = 2;
        if (field_0x264 >= 4) {
            flags = 0;
        }
        params.field_0x1d = 1;
        if (mPos.z != 0.0f) {
            params.field_0x1c = 1;
        }
        s32 frame = field_0x24c + lbl_804AB810[field_0x264];
        fn_801F06F0("enemy_both2_sob.bin", frame, &params, flags, 1.0f, 1.0f);
    } else {
        params.field_0x2d = 1;
        s32 frame = field_0x24c + lbl_804AB810[field_0x264];
        if (field_0x4 < 8) {
            void* res = lbl_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
            lbl_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
        }
    }
    fn_803075AC(mSerial, field_0x4, &mPos, field_0x24c + lbl_804AB810[field_0x264], false, 2, -1, 0, 0, 0);
}

// FIXME: 97.72 %. Literal and jump-table labels (@NNN) and a few register picks.
void ForceGem2_804AB850::update() {
    s32 player = 0;
    u16 angle = 0;
    s32 dir = 0;
    f32 dist = 0.0f;
    bool moved = false;
    JGeometry::TVec2f p(mPos.x, mPos.y);
    if (field_0x94 != 6) {
        fn_801F71A4(&field_0x3c.x, field_0x26c.x, 0.3f, 1.0f);
        fn_801F71A4(&field_0x3c.y, field_0x26c.y, 0.3f, 1.0f);
    }
    mPos += field_0x3c;
    JGeometry::TBox2f box(mBox.i, mBox.f);
    box.addPos(p);
    if (mState == 0 || mState == 3 || mState == 4) {
        s32 idx = fn_8023AF14(&box, field_0x4, false, mPos.z);
        if (mState != 0) {
            fn_801EEC98(mSerial, &angle, &dir, &dist, &player, 0, 0);
            if (field_0x94 != 6 || mPos.z == 0.0f) {
                moved = fn_801F2618(NULL, false);
            }
        }
        if (idx >= 0) {
            if (field_0x94 == 5 || field_0x94 == 6 || fn_8023E724(idx)->z >= 20.0f ||
                fn_80234ECC(idx)) {
                fn_802362C4(idx, lbl_804AB830[field_0x264]);
                field_0x260 = idx;
                mState = 1;
            }
        }
    }
    switch (mState) {
    case 0: {
        fn_801F71A4(&mPos.z, 4.0f * JMath::sincosTable_.sinShort(field_0x250) + 24.0f, 0.3f, 1.0f);
        field_0x250 += (s32)(256.0f * lbl_8053AAF8->mRandom.get_ufloat_1()) + 0x200;
        s32 mode = field_0x94;
        if (mode < 3) {
            if (mode >= 1 && field_0x244 == 0) {
                switch (field_0x98) {
                case 0:
                    field_0x26c.set(0.3f, 0.0f, 0.0f);
                    break;
                case 1:
                    field_0x26c.set(-0.3f, 0.0f, 0.0f);
                    break;
                case 2:
                    field_0x26c.set(0.0f, 0.3f, 0.0f);
                    break;
                case 3:
                    field_0x26c.set(0.0f, -0.3f, 0.0f);
                    break;
                }
                field_0x244 = 500;
                field_0x98 ^= 1;
            }
        } else if (mode < 5) {
            mAngle += mTimer;
            mPos.x = field_0x278.x + mRadius * JMath::sincosTable_.sinShort(mAngle);
            mPos.y = field_0x278.y + mRadius * JMath::sincosTable_.cosShort(mAngle);
        }
        break;
    }
    case 1:
        field_0x3c.zero();
        mPos.z = 0.0f;
        field_0x240 = 0;
        field_0x244 = 30;
        field_0x3c.z = 3.0f;
        mState = 2;
        // fall through
    case 2:
        field_0x3c.z -= 0.5f;
        if (mPos.z <= 0.0f) {
            if (field_0x3c.z < 0.0f) {
                field_0x3c.zero();
                mPos.z = 0.0f;
            }
        }
        if (fn_8023F264(field_0x260)) {
            mPos.x = fn_8023E724(field_0x260)->x;
            mPos.y = fn_8023E724(field_0x260)->y - 24.0f;
        }
        if (field_0x244 == 0) {
            vfunc_0x30(0xFF, true);
            fn_801F3668();
        }
        break;
    case 3:
        if (dist < 100.0f) {
            field_0x244 = 0;
            mState = 4;
        }
        break;
    case 4:
        if (mPos.z > 0.0f) {
            field_0x3c.z -= 0.3f;
            break;
        }
        if (field_0x94 == 6) {
            field_0x94 = 5;
        }
        mPos.z = 0.0f;
        field_0x3c.z = 0.0f;
        if (dist < 120.0f) {
            if (field_0x244 == 0) {
                if (moved) {
                    field_0x268 += 0x2000;
                    field_0x244 = 2;
                } else {
                    field_0x268 = angle + 0x8000;
                }
            }
            field_0x26c.x = 2.0f * JMath::sincosTable_.sinShort(field_0x268);
            field_0x26c.y = 2.0f * JMath::sincosTable_.cosShort(field_0x268);
        } else {
            field_0x26c.set(0.0f, 0.0f, 0.0f);
            mState = 3;
        }
        break;
    case 6:
        if (field_0x248 <= 0 && mEmitter284 == NULL) {
            mEmitter284 = fn_8013CC50(field_0x4, &mPos, 0x3D0, 0, 1, NULL, NULL, true, 1.0f);
            fn_801F0E34(0x30022, 0);
        }
        // fall through
    case 5:
        field_0x23c += 0.1f;
        field_0x238 -= 0.01f;
        if (field_0x238 < 0.1f) {
            if (mState == 6 && mEmitter284 != NULL) {
                mEmitter284->becomeInvalidEmitter();
            }
            fn_801F3668();
        }
        break;
    }
    field_0x240 -= 1;
    if (field_0x240 <= 0) {
        field_0x24c += 1;
        field_0x24c &= 3;
        if (field_0x24c == 0) {
            field_0x240 = (s32)(60.0f * lbl_8053AAF8->mRandom.get_ufloat_1()) + 60;
        } else {
            field_0x240 = 2;
        }
    }
    if (field_0x240 > 0) {
        field_0x240 -= 1;
    }
    if (field_0x244 > 0) {
        field_0x244 -= 1;
    }
    if (field_0x248 > 0) {
        field_0x248 -= 1;
    }
}

bool ForceGem2_804AB850::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    s32 code = fn_8022461C(pos, layer);
    if (code == 0xC || code == 0x3 || code == 0xE) {
        field_0x3c.zero();
        mState = 5;
        return false;
    }
    if (code == 0xD) {
        field_0x248 = 0;
        field_0x3c.zero();
        mState = 6;
        return false;
    }
    return code != 0;
}

void ForceGem2_804AB850::init() {
    field_0x94 = (field_0x90 >> 16) & 0xFF;
    field_0x264 = field_0x90 & 7;
    mEmitter284 = NULL;
    field_0x24c = 0;
    field_0x250 = 0;
    mAngle = 0;
    mTimer = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x268 = 0;
    mLife = 10000;
    field_0x25c = 0;
    field_0x260 = -1;
    field_0x98 = 0;
    field_0x278.x = mPos.x;
    field_0x278.y = mPos.y;
    field_0x278.z = mPos.z;
    mTimer = 100;
    field_0x238 = 1.0f;
    field_0x23c = 0.0f;
    if (vfunc_0x34()) {
        fn_801F3668();
        return;
    }
    mRadius = 30.0f;
    mState = 0;
    // FIXME: 99.35 %. The seed constant 0x19660D and the lbl_8053AAF8 pointer
    // swap r4/r5 (ours: constant in r5, original r4); the code is otherwise identical.
    mAngle = (s32)(65535.0f * lbl_8053AAF8->mRandom.get_ufloat_1());
    switch (field_0x94) {
    case 1:
        mPos.z = 24.0f;
        field_0x98 = 2;
        field_0x98 |= (s32)(1.99f * lbl_8053AAF8->mRandom.get_ufloat_1());
        break;
    case 2:
        mPos.z = 24.0f;
        field_0x98 |= (s32)(1.99f * lbl_8053AAF8->mRandom.get_ufloat_1());
        break;
    case 3:
        mTimer = -100;
        // fall through
    case 4:
        mPos.z = 24.0f;
        mPos.x = field_0x278.x + mRadius * JMath::sincosTable_.sinShort(mAngle);
        mPos.y = field_0x278.y + mRadius * JMath::sincosTable_.cosShort(mAngle);
        break;
    case 6:
        mState = 4;
        break;
    case 5:
        mState = 3;
        break;
    }
    field_0x26c.set(0.0f, 0.0f, 0.0f);
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    mBox.set(-4.0f, -4.0f, 4.0f, 4.0f);
}

ForceGem2_804AB850::~ForceGem2_804AB850() {
    fn_801EE434(mEmitter284);
}
