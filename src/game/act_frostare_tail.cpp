#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/ViewLayer.h"
#include "game/act/act_frostare_tail.h"
#include "game/act/tu_801DAC6C.h"

// Two animations: (cel, duration) pairs, ended by a cel of -1.
static const s16 lbl_80469AB8[] = {0, 6, 1, 6, 2, 6, 3, 6, -1, -1};
static const s16 lbl_80469ACC[] = {0, 4, 4, 4, 5, 4, 6, 3, 4, 4, -1, -1};
static const s16* lbl_8053A150[] = {lbl_80469AB8, lbl_80469ACC};

cls_804A4F90::cls_804A4F90() {
    mHitCount = 0;
}

cls_804A4F90::~cls_804A4F90() {
    // The singleton in a local: the original sets r0 to -1 after the call.
    Singleton_801DAC6C* frostare = fn_801DAC6C();
    frostare->mSerial4 = -1;
}

void cls_804A4F90::init() {
    EnemyBase_804A4808::init();
    mBox.set(-12.0f, -16.0f, 8.0f, 8.0f);
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0xb0 = 0x40004;
    field_0x98 = 2;
    mFlags |= 0x2A0;
    mLife = 1000;
    mLifeBackup = mLife;
    field_0x114 = 2;
    field_0x196 = 1;
    mState = 0;
    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mAnim.fn_801D2608(lbl_8053A150, 2);
    mAnim.startAnim_801D22DC(0, 2, false);
    mAnim.fn_801D1D60();
    fn_801DAC6C()->fn_801DAF3C(mSerial);
}

void cls_804A4F90::vfunc_0x90(u8 mapHit) {
    switch (mState) {
    case 0:
        break;
    case 1:
        getGameView_802D6A9C()->fn_802D79E8(field_0x4, 4.0f, 0.5f, false);
        gGameRoot_8053AAF8->fn_80134DE4(1, 0);
        if (mStateTimer >= 60) {
            startSound_801F0E34(0x40055, 0);
            fn_801DAC6C()->field_0x84 &= ~1;
            fn_801DAC6C()->fn_801DAD88();
            mStateTimer = 0;
            mState = 2;
        }
        break;
    case 2:
        if (mStateTimer >= 120) {
            fn_801F3668();
        }
        break;
    }
}

void cls_804A4F90::vfunc_0x9C(u8 mapHit) {
    if ((mFlags & (ENEMY_FLAG_HIT | ENEMY_FLAG_HIT_PLAYER | ENEMY_FLAG_HIT_ACTOR)) == 0) {
        fn_801CE2BC(mapHit);
    } else {
        vfunc_0x94();
    }
}

// FIXME: 97.86. Register swap only: the original loads lbl_8053EFB8 into f1 and
// field_0x3c.y into f0 (fadds f0, f0, f1); ours uses f0 / f1.
void cls_804A4F90::vfunc_0x88() {
    field_0x3c.x = 0.0f;
    field_0x3c.z = 0.0f;
    if (mState == 2) {
        field_0x3c.y += __fabsf(lbl_8053EFB8);
    } else {
        field_0x3c.y = 0.0f;
    }
}

void cls_804A4F90::vfunc_0x74() {
    mAnim.fn_801D22A0(0, 2);
    if (mHitCount >= 10) {
        mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
        mBox2.set(mBox);
        field_0xb0 = 0;
        fn_801DAC6C()->field_0x84 |= 1;
        mStateTimer = 0;
        mState = 1;
    }
}

// FIXME: 97.14. Same instructions; the original keeps the layer flag in r27 and
// the rupee serial in r28 (loading 'RUPY' into r28 before getActorManager_801F856C)
// and puts the upper-layer velocity temporary at sp+0x20, the other at sp+0x2C;
// ours swaps both pairs, and the random terms' scratch registers follow.
bool cls_804A4F90::vfunc_0x5C() {
    bool hit = false;
    if (mBox.getWidth() > 0.0f && mBox.getHeight() > 0.0f) {
        for (s32 i = 0; i < 4; i++) {
            s32 flag;
            if (i == fn_801F285C(NULL, &flag, true, i)) {
                fn_8023B6CC(i);
                if (fn_801F37AC(0, getPlayerPos_8023E724(i), 0, flag, i)) {
                    hit = true;
                    mHitCount++;
                    u8 upper = viewLayerFlag(field_0x4);
                    s32 serial =
                        getActorManager_801F856C()->fn_801F8544('RUPY', field_0x4, &mPos, 2);
                    if (serial >= 0) {
                        ActorBase_804A5770* rupee =
                            getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                        if (rupee != NULL) {
                            if (upper) {
                                rupee->field_0x3c = JGeometry::TVec3f(
                                    1.5f * (2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f),
                                    -4.0f - 2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1(),
                                    0.0f);
                            } else {
                                rupee->field_0x3c = JGeometry::TVec3f(
                                    1.2f * (2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f),
                                    1.2f * (2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f),
                                    3.0f + 3.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
                            }
                        }
                    }
                }
            }
        }
        if (hit) {
            field_0xf8 = 0;
            field_0x3c.set(0.0f, 0.0f, 0.0f);
            mPos.z = 0.0f;
            mAnim.startAnim_801D22DC(1, 0, false);
            startSound_801F0E34(0x40054, 0);
            fn_801DAC6C()->fn_801DADD0();
        }
    }
    return hit;
}

bool cls_804A4F90::vfunc_0x60() {
    if (mBox.getWidth() > 0.0f && mBox.getHeight() > 0.0f) {
        fn_801F2DB4(mActorId, 'NULL', NULL);
    }
    return false;
}

void cls_804A4F90::draw() {
    if (fn_801CD2F8(field_0x4)) {
        fn_80307AC8(mSerial, field_0x4, &mPos, mAnim.mCel + 0xE3, 0, 0, -1);
    } else {
        fn_801F02BC("enemy_both_sob.bin", mAnim.mCel + 0xE3, NULL, -1, 0, false, 1.0f, 1.0f);
    }
}

// FIXME: 98.39. Register swap only: the original keeps the result in r5 and
// field_0xdc in r4; ours the other way round.
u8 cls_804A4F90::fn_801DC264() {
    u8 alpha = 0;
    if (mAnim.mAnim == 1) {
        s32 length = field_0xdc;
        s32 time = field_0xd8;
        s32 fadeStart = length - 25;
        if (time > fadeStart || length < 25) {
            if (length < 25) {
                alpha = time * 255 / length;
            } else {
                alpha = (time - fadeStart) * 255 / 25;
            }
        }
    }
    return alpha;
}
