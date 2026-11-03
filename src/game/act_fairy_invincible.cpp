#include "game/GameAudio.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/act/act_fairy_invincible.h"

// A uniform scale as a 3x4 matrix, filled row by row.
static inline void setUniformScaleMtx(Mtx m, f32 scale) {
    m[0][0] = scale;
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[0][3] = 0.0f;
    m[1][0] = 0.0f;
    m[1][1] = scale;
    m[1][2] = 0.0f;
    m[1][3] = 0.0f;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = scale;
    m[2][3] = 0.0f;
}

// FIXME: 99.46 %. The matrix stores are in the same order, but the original
// loads 0.0f before 1.5f, so the .sdata2 literals are numbered the other way
// (lbl_80541C28 vs @705, lbl_80541C2C vs @704).
void InvincibilityFairy_804AB4C0::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    setUniformScaleMtx(mtx, 1.5f);
    params.mMtx = mtx;
    if (mCounters[0][0] >= 0x100 || (mCounters[0][0] & 2) == 0) {
        params.field_0x2d = 1;
        fn_801F06F0("enemy_both2_sob.bin", field_0x250 + 0x5E, &params, 0, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x250 + 0x5E, false, 2, -1, 0, 0, 0);
    }
}

// Moves both emitters to the fairy.
inline void InvincibilityFairy_804AB4C0::followEmitters() {
    if (mEmitter264 != NULL) {
        mEmitter264->setGlobalTranslation(mPos);
    }
    if (mEmitter268 != NULL) {
        mEmitter268->setGlobalTranslation(mPos);
    }
}

// Stops both emitters from making particles and forgets them.
inline void InvincibilityFairy_804AB4C0::releaseEmitters() {
    if (mEmitter264 != NULL) {
        mEmitter264->becomeInvalidEmitter();
        mEmitter264 = NULL;
    }
    if (mEmitter268 != NULL) {
        mEmitter268->becomeInvalidEmitter();
        mEmitter268 = NULL;
    }
}

// The target player and the counters of the state 0 search, zeroed.
inline void InvincibilityFairy_804AB4C0::clearTracking() {
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
}

// FIXME: 96.47 %. Remaining: the .sdata2 literals are numbered differently
// (lbl_80541C28 vs @697); the box corners are loaded and scheduled a little
// differently around the copy (the first fadds and the stfs of 0x20); the
// original reads field_0x4 for the spawn a few instructions later; and the
// original's loop keeps the index i in r7 as well as the pair pointer.
void InvincibilityFairy_804AB4C0::update() {
    fn_801F71A4(&field_0x3c.x, field_0x258, 0.1f, 0.5f);
    fn_801F71A4(&field_0x3c.y, field_0x25c, 0.1f, 0.5f);
    fn_801F71A4(&mPos.z, field_0x260, 0.3f, 0.5f);
    mPos += field_0x3c;
    followEmitters();

    switch (field_0x230) {
    case 0:
        if (mCounters[1][0] == 0) {
            field_0x244 += 0x300;
            field_0x260 = 2.0f * JMASinShort(field_0x244) + 7.0f;
            if (field_0x24c == 0) {
                if (mCounters[1][1] != 0) {
                    field_0x248 += 0x100;
                } else {
                    mCounters[1][1] = 30 + (s32)(lbl_8053AAF8->mRandom.get_ufloat_1() * 30.0f);
                    field_0x24c = 1;
                    field_0x258 = 0.0f;
                    field_0x25c = 0.0f;
                }
            } else if (mCounters[1][1] == 0) {
                field_0x248 += (s32)(lbl_8053AAF8->mRandom.get_ufloat_1() * 32768.0f);
                mCounters[1][1] = 40 + (s32)(lbl_8053AAF8->mRandom.get_ufloat_1() * 40.0f);
                field_0x24c = 0;
            }

            JGeometry::TBox2f box(mBox);
            box.i.y += mPos.y;
            box.i.x += mPos.x;
            box.f.x += mPos.x;
            box.f.y += mPos.y;
            s32 player = fn_8023AF14(&box, field_0x4, true, 0.0f);
            if (player >= 0) {
                s32 layer = field_0x4;
                clearTracking();
                mCounters[0][0] = 0x1f4;
                field_0x244 = player;
                if (layer >= 8) {
                    fn_801F856C()->fn_801F9484(ACTOR_PEFF, layer, &mPos, (player & 3) | 4, -1, -1);
                }
                mCounters[1][0] = 0;
                field_0x254 = 0;
                field_0x248 = 0;
                field_0xb0 = 0;
                field_0x230 = 1;
                field_0x11e = 1;
            }
        }
        break;
    case 1:
        fn_8023C830(field_0x244);
        mCounters[0][0] = 1000;
        field_0x248 += 1;
        if (mCounters[1][0] == 0) {
            f32 dx = mPos.x - fn_8023E724(field_0x244)->x;
            f32 dy = mPos.y - fn_8023E724(field_0x244)->y;
            field_0x254 = JMath::atanTable_.atan2_(-dx, -dy) *
                          (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
            mCounters[1][0] = 5;
        }
        field_0x258 = 5.0f * JMASinShort(field_0x254);
        field_0x25c = 5.0f * JMACosShort(field_0x254);
        if (field_0x248 > 0x2D0) {
            releaseEmitters();
            mAlive = false;
        }
        break;
    }

    if (mCounters[0][1] == 0) {
        mCounters[0][1] = 4;
        field_0x250 += 1;
        if (field_0x250 > 2) {
            field_0x250 = 0;
        }
    }
    for (s32 i = 0; i < 2; i++) {
        if (mCounters[i][0] > 0) {
            mCounters[i][0]--;
        }
        if (mCounters[0][0] <= 0) {
            releaseEmitters();
            mAlive = false;
        }
        if (mCounters[i][1] > 0) {
            mCounters[i][1]--;
        }
        if (mCounters[0][0] <= 0) {
            releaseEmitters();
            mAlive = false;
        }
    }
}

// Zeroes the timers, counters and targets, and sets the life; init() then
// sets the values it starts from over them.
inline void InvincibilityFairy_804AB4C0::clearWork() {
    clearTracking();
    mCounters[0][0] = 0;
    mCounters[0][1] = 0;
    mCounters[1][0] = 0;
    mCounters[1][1] = 0;
    mLife = 8;
    field_0x250 = 0;
    field_0x258 = 0.0f;
    field_0x25c = 0.0f;
    field_0x260 = 0.0f;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
}

// FIXME: 93.97 %. Same .sdata2 literal numbering as update(). Register
// assignment differs: the original keeps the 0x660d multiplier in r12 and
// the 0x400 in r30, and loads the random object into r29 for each call.
void InvincibilityFairy_804AB4C0::init() {
    mBox.set(-4.0f, -4.0f, 4.0f, 4.0f);
    clearWork();
    mCounters[0][0] = 0x400;
    field_0x260 = 10.0f;
    mCounters[1][0] = 10;
    mCounters[1][1] = 30 + (s32)(lbl_8053AAF8->mRandom.get_ufloat_1() * 30.0f);
    field_0x248 = (s32)(lbl_8053AAF8->mRandom.get_ufloat_1() * 32768.0f);
    field_0x230 = 0;
    field_0xb0 = 0x20000;
    mEmitter264 = fn_8013CC50(field_0x4, &mPos, 0x183, 0, 0, NULL, NULL, true, 1.5f);
    mEmitter268 = fn_8013CC50(field_0x4, &mPos, 0x184, 0, 0, NULL, NULL, true, 1.5f);
}

void InvincibilityFairy_804AB4C0::vfunc_0x44() {
    followEmitters();
}

InvincibilityFairy_804AB4C0::~InvincibilityFairy_804AB4C0() {
    fn_801EE434(mEmitter264);
    fn_801EE434(mEmitter268);
}
