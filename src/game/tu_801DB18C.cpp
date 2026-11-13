#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "JSystem/JMath/JMath.h"
#include "game/Player.h"
#include "game/act/tu_801DB18C.h"
#include "game/act/tu_801DAC6C.h"

extern const s16* lbl_8053A148[]; // the animation table (.sdata, this unit)
extern const char lbl_80469AA4[]; // "enemy_both_sob.bin"

// FIXME(shotgun): 99.86 %, reloc-name; behaviour clean.
cls_804A4EE8::cls_804A4EE8() {
    mTargetPlayer = -1;
    field_0x2ac = 0;
    mSearchRange = 0.0f;
    field_0x2b4 = 0.0f;
    field_0x2b8 = 0.0f;
    mSearchPos.x = 0.0f;
    mSearchPos.y = 0.0f;
    mSearchPos.z = 0.0f;
    field_0x2c8 = 0.0f;
    field_0x2cc = 0.0f;
    field_0x2d0 = 0.0f;
    mRadius = 10.0f;
    mTargetRadius = 10.0f;
    mAngle = 0;
    mAngleSpeed = 0;
    field_0x2e0 = 0.0f;
    field_0x2e4 = NULL;
}

cls_804A4EE8::~cls_804A4EE8() {
    releaseEmitter_801EE434(field_0x2e4);
    field_0x2e4 = NULL;
    if (field_0x16c == -1) {
        fn_801DAC6C()->fn_801DAEAC(mSerial);
    }
}

// FIXME(shotgun): 97.81 %, missing/extra + reloc-name; behaviour clean.
void cls_804A4EE8::init() {
    EnemyBase_804A4808::init();
    mBox.set(-8.0f, -16.0f, 8.0f, 8.0f);
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0xb0 = 0x10101;
    field_0x98 = 2;
    mFlags |= 0x20;
    field_0x114 = 2;
    field_0x194 = 1;
    if (field_0x16c == -1) {
        s16 a = fn_801CD664(0x3800);
        if (fn_801CD664(2) == 0) {
            a = -a;
        }
        f32 n = (f32)(fn_801CD664(0x3C) + 1);
        field_0x2b4 = 0.0f;
        field_0x2b8 = 0.1f * n;
        f32 x = field_0x2b4;
        field_0x2b4 = x * JMath::sincosTable_.cosShort(-a) - field_0x2b8 * JMath::sincosTable_.sinShort(-a);
        field_0x2b8 = x * JMath::sincosTable_.sinShort(-a) + field_0x2b8 * JMath::sincosTable_.cosShort(-a);
    }
    mState = 0;
    mStateTimer = 0;
    if (field_0x16c == -1) {
        field_0x250 = fn_801CD664(1) + 1;
    } else {
        field_0x250 = fn_801CD664(0x3D) + 0x1E;
    }
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mAnim.fn_801D2608(lbl_8053A148, 1);
    mAnim.startAnim_801D22DC(0, 2, false);
    mAnim.mFrameCtrl.setRate(0.25f);
    mAnim.fn_801D1D60();
    field_0x2e4 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x258, 1, 0, NULL, NULL, true, 1.0f);
    if (field_0x16c == -1) {
        fn_801DAC6C()->fn_801DAF14(mSerial);
    }
}

// FIXME(shotgun): 76.72 %, missing/extra + reloc-name; behaviour drift.
void cls_804A4EE8::fn_801DB9CC() {
    if (fn_801CD664(2) == 0) {
        mFlags &= ~0x2000;
    } else {
        mFlags |= 0x2000;
    }
    mTargetRadius = (f32)(fn_801CD664(0x1F) + 0x1E);
    mAngleSpeed = 0;
    mAngle = fn_801CD664(0x10000);
    if (field_0x16c == -1) {
        f32 vx = field_0x2b4;
        f32 vy = field_0x2b8;
        if (!(vx * vx + vy * vy <= 32.0f * JGeometry::TUtil<f32>::epsilon())) {
            f32 rx;
            f32 ry;
            if (vx < 0.0f) {
                mFlags |= 0x2000;
                f32 s = JMath::sincosTable_.sinShort((s16)0xC000);
                f32 c = JMath::sincosTable_.cosShort((s16)0xC000);
                ry = vx * s + vy * c;
                rx = vx * c - vy * s;
            } else {
                mFlags &= ~0x2000;
                f32 s = JMath::sincosTable_.sinShort(0x4000);
                f32 c = JMath::sincosTable_.cosShort(0x4000);
                ry = vx * s + vy * c;
                rx = vx * c - vy * s;
            }
            if (fn_801CD664(2) == 0) {
                rx = -rx;
                ry = -ry;
                mFlags ^= 0x2000;
            }
            mAngle = (s32)(JMath::atanTable_.atan2_(rx, ry) * 10430.378f);
            JGeometry::TVec2f v(field_0x2b4, field_0x2b8);
            mAngleSpeed = (s32)(0.015915494f * (65536.0f * v.length()));
        }
    }
    mSearchPos.x = mPos.x - JMath::sincosTable_.sinShort(mAngle) * mRadius;
    mSearchPos.y = mPos.y - JMath::sincosTable_.cosShort(mAngle) * mRadius;
    mSearchPos.z = mSearchPos.x;
    field_0x2c8 = mSearchPos.y;
    if (field_0x16c == -1) {
        JGeometry::TVec3f p;
        if (fn_801DAC6C()->fn_801DACAC(&p)) {
            field_0x2cc = mSearchPos.z;
            field_0x2d0 = field_0x2c8;
            field_0x2cc -= p.x;
            field_0x2d0 -= p.y;
        }
        switch (fn_801CD664(3)) {
        case 0:
            mSearchRange = 96.0f;
            break;
        case 1:
            mSearchRange = 192.0f;
            break;
        default:
            mSearchRange = 288.0f;
            break;
        }
    } else {
        mSearchRange = 0.0f;
    }
}

// Not auto-inlined: vfunc_0x90 calls fn_801DB950 out of line in the original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
void cls_804A4EE8::fn_801DB950(s32 state) {
    // A one-case switch: MWCC's cmpwi/beq/b shape for state 1 is what matches.
    switch (state) {
    case 1:
        field_0x250 = -1;
        mAnim.mFrameCtrl.setRate(1.0f);
        field_0x2ac = 0;
        fn_801DB9CC();
        mTargetPlayer = fn_801D05F8(&mSearchPos, mSearchRange);
        break;
    }
    mStateTimer = 0;
    mState = state;
}
#pragma auto_inline reset

// FIXME(shotgun): 84.97 %, missing/extra + reloc-name; behaviour drift.
void cls_804A4EE8::fn_801DB6C8() {
    if (field_0x16c == -1) {
        JGeometry::TVec3f p;
        if (fn_801DAC6C()->fn_801DACAC(&p)) {
            mSearchPos.z = p.x;
            field_0x2c8 = p.y;
            mSearchPos.z += field_0x2cc;
            field_0x2c8 += field_0x2d0;
        }
    }
    f32 tx;
    f32 ty;
    if (mTargetPlayer == -1) {
        tx = mSearchPos.z + field_0x2b4;
        ty = field_0x2c8 + field_0x2b8;
    } else {
        JGeometry::TVec3f* player = &field_0x1ac[mTargetPlayer];
        f32 px = player->x;
        f32 py = player->y;
        JGeometry::TVec2f away(mSearchPos.x - px, mSearchPos.y - py);
        away.normalize();
        tx = px + away.x * mRadius + field_0x2b4;
        ty = py + away.y * mRadius + field_0x2b8;
    }
    JGeometry::TVec2f d(tx - mSearchPos.x, ty - mSearchPos.y);
    f32 len = d.length();
    f32 step = field_0x2e0;
    if (len < step) {
        step = len;
    }
    d.normalize();
    mSearchPos.x += d.x * step;
    mSearchPos.y += d.y * step;
    if (mFlags & 0x2000) {
        mAngle -= mAngleSpeed;
    } else {
        mAngle += mAngleSpeed;
    }
}

void cls_804A4EE8::vfunc_0x90(u8 mapHit) {
    switch (mState) {
    case 0:
        if (mStateTimer >= field_0x250) {
            fn_801DB950(1);
        }
        break;
    case 1: {
        fn_801CD950(&mRadius, mTargetRadius, 1.0f);
        fn_801CD950(&field_0x2e0, 0.4f, 0.01f);
        if (mAngleSpeed != 0x200) {
            s32 step = 0x20;
            if (mAngleSpeed > 0x200) {
                step = 0x800;
            }
            fn_801CD930(&mAngleSpeed, 0x200, step);
        }
        s32 target = fn_801D05F8(&mSearchPos, mSearchRange);
        if (target == -1) {
            mTargetPlayer = -1;
            field_0x2ac = 0;
        } else {
            field_0x2ac++;
            if (mTargetPlayer != target && field_0x2ac >= 0x5A) {
                mTargetPlayer = target;
                field_0x2ac = 0;
            }
        }
        fn_801DB6C8();
        break;
    }
    }
}

void cls_804A4EE8::vfunc_0x94() {
    s32 cel = mAnim.mCel;
    EnemyBase_804A4808::vfunc_0x94();
    if (cel == 8 && mAnim.mCel == 7) {
        startSound_801F0E34(0x4005B, 0);
    }
}

void cls_804A4EE8::vfunc_0x9C(u8 mapHit) {
    EnemyBase_804A4808::vfunc_0x9C(mapHit);
    setEmitterPos_8013CB44(field_0x2e4, &mPos, 1, 0, true);
}

// FIXME: 87.43 %, scheduling+regalloc; behaviour clean.
// FIXME(shotgun): remaining diff is instruction scheduling and registers only.
void cls_804A4EE8::vfunc_0x88() {
    if (field_0x2b4 != 0.0f) {
        if (mState == 0) {
            field_0x2b4 *= 0.95f;
        } else {
            field_0x2b4 *= 0.5f;
        }
        if (__fabsf(field_0x2b4) < 0.001f) {
            field_0x2b4 = 0.0f;
        }
    }
    if (field_0x2b8 != 0.0f) {
        if (mState == 0) {
            field_0x2b8 *= 0.95f;
        } else {
            field_0x2b8 *= 0.5f;
        }
        if (__fabsf(field_0x2b8) < 0.001f) {
            field_0x2b8 = 0.0f;
        }
    }
    if (mState == 0) {
        field_0x3c.x = field_0x2b4;
        field_0x3c.y = field_0x2b8;
        field_0x3c.z = 0.0f;
        return;
    }
    f32 sinv = JMath::sincosTable_.sinShort(mAngle) * mRadius;
    f32 cosv = JMath::sincosTable_.cosShort(mAngle) * mRadius;
    f32 tx = mSearchPos.x + sinv;
    f32 ty = mSearchPos.y + cosv;
    field_0x3c.x = tx - mPos.x;
    field_0x3c.y = ty - mPos.y;
    field_0x3c.z = 0.0f;
}

u8 cls_804A4EE8::vfunc_0x68() {
    u8 hitPlayers = 0;
    if (mBox.getWidth() > 0.0f && mBox.getHeight() > 0.0f) {
        hitPlayers = fn_801F2740(-1);
        if (hitPlayers != 0) {
            JGeometry::TVec3f conv;
            JGeometry::TVec3f center3;
            JGeometry::TVec2f center;
            JGeometry::TVec2f pos;
            fn_801F3128(&center);
            for (s32 player = 0; player < 4; player++) {
                if (hitPlayers & (1 << player)) {
                    if (fn_801F329C(player)) {
                        center3.x = center.x;
                        center3.y = center.y;
                        center3.z = 0.0f;
                        fn_801CD0D0(&center3, &conv);
                        pos.x = conv.x;
                        pos.y = conv.y;
                    } else {
                        pos = center;
                    }
                    fn_80238548(player, field_0x114, &pos, 1);
                }
            }
        }
    }
    return hitPlayers;
}

void cls_804A4EE8::draw() {
    SpriteParams_801F5AE0 params;
    params.field_0x1c = 1;
    params.field_0x1d = 0;
    fn_801F02BC(lbl_80469AA4, mAnim.mCel + 0xE3, &params, -1, 0, false, 1.0f, 1.0f);
}

