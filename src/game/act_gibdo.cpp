#include "game/GameAudio.h"
#include "game/act/act_gibdo.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "JSystem/JParticle/JPAEmitter.h"

// Animation sequences of (cel, duration) pairs, ended by -1.
static const s16 lbl_804691B0[10] = {0, 8, 1, 0x12, 0, 8, 2, 0x12, -1, -1};
static const s16 lbl_804691C4[10] = {5, 8, 6, 0x12, 5, 8, 7, 0x12, -1, -1};
static const s16 lbl_804691D8[10] = {10, 8, 11, 0x12, 10, 8, 12, 0x12, -1, -1};
static const s16 lbl_804691EC[10] = {15, 8, 16, 0x12, 15, 8, 17, 0x12, -1, -1};
static const s16 lbl_80469200[12] = {0, 0xF, 3, 0xF, 0, 0xF, 4, 0xF, 0, 0xF, -1, -1};
static const s16 lbl_80469218[12] = {5, 0xF, 8, 0xF, 5, 0xF, 9, 0xF, 5, 0xF, -1, -1};
static const s16 lbl_80469230[12] = {10, 0xF, 13, 0xF, 10, 0xF, 14, 0xF, 10, 0xF, -1, -1};
static const s16 lbl_80469248[12] = {15, 0xF, 18, 0xF, 15, 0xF, 19, 0xF, 15, 0xF, -1, -1};
static const s16 lbl_80469260[10] = {0, 5, 3, 0xA, 13, 0xA, 10, 5, -1, -1};
static const s16 lbl_80469274[10] = {0, 5, 4, 0xA, 18, 0xA, 15, 5, -1, -1};
static const s16 lbl_80469288[10] = {5, 5, 8, 0xA, 14, 0xA, 10, 5, -1, -1};
static const s16 lbl_8046929C[10] = {5, 5, 9, 0xA, 19, 0xA, 15, 5, -1, -1};
static const s16 lbl_804692B0[10] = {10, 5, 13, 0xA, 3, 0xA, 0, 5, -1, -1};
static const s16 lbl_804692C4[10] = {10, 5, 14, 0xA, 8, 0xA, 5, 5, -1, -1};
static const s16 lbl_804692D8[10] = {15, 5, 18, 0xA, 4, 0xA, 0, 5, -1, -1};
static const s16 lbl_804692EC[10] = {15, 5, 19, 0xA, 9, 0xA, 5, 5, -1, -1};

// The sprite's animation table: one pointer per sequence above.
static const s16* lbl_804A3BB8[16] = {
    lbl_804691B0, lbl_804691C4, lbl_804691D8, lbl_804691EC,
    lbl_80469200, lbl_80469218, lbl_80469230, lbl_80469248,
    lbl_80469260, lbl_80469274, lbl_80469288, lbl_8046929C,
    lbl_804692B0, lbl_804692C4, lbl_804692D8, lbl_804692EC,
};

Gibdo_804A3BF8::Gibdo_804A3BF8() {}

// FIXME: 99.45 %. Only the relocations of the sdata2 literals differ
// (lbl_8053EB94 and friends are @NNN here; a data split of the literal pool).
void Gibdo_804A3BF8::init() {
    EnemyBase_804A4808::init();
    if (field_0x90 == 1) {
        mFlags |= 0x4000;
    }
    mBox.set(-8.0f, -24.0f, 8.0f, 8.0f);
    field_0x80.set(-10.0f, -10.0f, 10.0f, 10.0f);
    mFlags |= ENEMY_FLAG_NO_HURT;
    mState = 0;
    mStateTimer = 0;
    field_0x250 = fn_801CD664(0x3D) + 0x3C;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mAnim.fn_801D2608(lbl_804A3BB8, 0x10);
    mAnim.startAnim_801D22DC(field_0x98, 2, false);
    mAnim.fn_801D1D60();
}

// FIXME(shotgun): case 3 also copies the fade colour to the three emitters (0x32A, 0x41B,
// 0x41C) it keeps the handles of; those copies are left out (see the note at the top).
// FIXME(shotgun): 79.02 %, missing/extra
void Gibdo_804A3BF8::fn_801B9DD4(s32 mode) {
    mFlags &= ~0x8000;
    switch (mode) {
    case 0:
        if (mState != 0) {
            mFlags |= 0x8000;
            mTargetSpeed = 0.0f;
        }
        field_0x250 = fn_801CD664(0x3D) + 0x3C;
        mAnim.fn_801D22A0(field_0x98, 2);
        break;
    case 1:
        mSpeed = 0.0f;
        mTargetSpeed = 0.0f;
        mAnim.startAnim_801D22DC(field_0x98 + 4, 0, false);
        break;
    case 2: {
        mSpeed = 0.0f;
        mTargetSpeed = 0.0f;
        s32 kind;
        switch (mAnim.mAnim) {
        case 0:
        case 4:
        case 12:
        case 14:
            kind = 0;
            break;
        case 1:
        case 5:
        case 13:
        case 15:
            kind = 1;
            break;
        case 2:
        case 6:
        case 8:
        case 10:
            kind = 2;
            break;
        default:
            kind = 3;
            break;
        }
        s32 dir = field_0x98;
        s32 anim = 8;
        switch (kind) {
        case 0:
            if (dir == 3) {
                anim = 9;
            } else if (dir >= 2) {
                anim = 8;
            } else {
                anim = gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f ? 8 : 9;
            }
            break;
        case 1:
            if (dir == 3) {
                anim = 11;
            } else if (dir >= 2) {
                anim = 10;
            } else {
                anim = gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f ? 10 : 11;
            }
            break;
        case 2:
            if (dir == 1) {
                anim = 13;
            } else if (dir == 0) {
                anim = 12;
            } else {
                anim = gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f ? 12 : 13;
            }
            break;
        case 3:
            if (dir == 1) {
                anim = 15;
            } else if (dir == 0) {
                anim = 14;
            } else {
                anim = gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f ? 14 : 15;
            }
            break;
        }
        mAnim.startAnim_801D22DC(anim, 0, false);
        break;
    }
    case 3:
        mSpeed = 0.0f;
        mTargetSpeed = 0.0f;
        field_0x3c.set(0.0f, 0.0f, 0.0f);
        mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
        mBox2 = mBox;
        field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
        field_0xb0 = 0;
        mFlags |= 0x300;
        mLife = 1;
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x329, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x32A, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x41B, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x41C, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x54F, 0, 0, NULL, NULL, true, 1.0f);
        startSound_801F0E34(0x5007E, 0);
        startSound_801F0E34(0x40003, 0);
        mDrawEnabled = false;
        break;
    }
    mStateTimer = 0;
    mState = mode;
}

// FIXME: 96.73 %. The original reloads mFlags before clearing 0x8000 (lwz after
// the timer compare); ours reuses the first load. The two jump tables
// (jumptable_804A3C9C, jumptable_804A3CE4) and the 120.0f / 0.1f literals are
// @NNN here (reloc-name, a data split).
void Gibdo_804A3BF8::vfunc_0x90(u8 mapHit) {
    switch (mState) {
    case 0:
        if (mFlags & 0x8000) {
            if (mStateTimer >= 5) {
                mFlags &= ~0x8000;
            }
        }
        if (mapHit) {
            s32 dir = fn_801CF1B0(true, 120.0f, 0.1f);
            if (dir == field_0x98) {
                field_0x250 += 10;
            } else if (fn_801D05F8(&mPos, 120.0f) < 0) {
                mFlags |= 0x2000;
                fn_801B9DD4(1);
            } else {
                field_0x98 = dir;
                fn_801B9DD4(2);
            }
        } else if (mStateTimer >= field_0x250) {
            if (fn_801D05F8(&mPos, 120.0f) < 0) {
                fn_801B9DD4(1);
            } else {
                bool turn = false;
                if (mFlags & 0x2000) {
                    mFlags &= ~0x2000;
                    turn = true;
                }
                s32 dir = fn_801CF1B0(turn, 120.0f, 0.1f);
                if (dir == field_0x98) {
                    fn_801B9DD4(0);
                } else {
                    field_0x98 = dir;
                    fn_801B9DD4(2);
                }
            }
        }
        break;
    case 1:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            bool turn = false;
            if (mFlags & 0x2000) {
                mFlags &= ~0x2000;
                turn = true;
            }
            s32 dir = fn_801CF1B0(turn, 120.0f, 0.1f);
            if (dir == field_0x98) {
                fn_801B9DD4(0);
            } else {
                field_0x98 = dir;
                fn_801B9DD4(2);
            }
        }
        break;
    case 2:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            s32 dir = 0;
            switch (mAnim.mAnim) {
            case 0:
            case 4:
            case 12:
            case 14:
                dir = 0;
                break;
            case 1:
            case 5:
            case 13:
            case 15:
                dir = 1;
                break;
            case 2:
            case 6:
            case 8:
            case 10:
                dir = 2;
                break;
            case 3:
            case 7:
            case 9:
            case 11:
                dir = 3;
                break;
            }
            if (field_0x98 == dir) {
                fn_801B9DD4(0);
            } else {
                fn_801B9DD4(2);
            }
        }
        break;
    case 3:
        if (mStateTimer >= 180) {
            JGeometry::TVec3f pos;
            fn_801CD150(&mPos, &pos);
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484('STAL', 8, &pos, 3, field_0x16c, -1);
            if (serial >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (actor != NULL) {
                    actor->field_0x98 = 2;
                }
            }
            mAlive = false;
        }
        break;
    }

    if (mState != 0 || (mFlags & 0x8000)) {
        mTargetSpeed = 0.0f;
    } else {
        switch (mAnim.mCel) {
        case 1:
        case 2:
        case 6:
        case 7:
        case 11:
        case 12:
        case 16:
        case 17:
            mTargetSpeed = 0.0f;
            mAccel = 0.05f;
            break;
        default:
            mTargetSpeed = 0.8f;
            mAccel = 0.2f;
            break;
        }
    }
}

// FIXME(shotgun): 100.00 %, matched
void Gibdo_804A3BF8::vfunc_0x24() {
    if (mFlags & 0x4000) {
        ActorBase_804A5770::vfunc_0x24();
        JPABaseEmitter* emitter = spawnEmitter_8013CC50(field_0x4, &mPos, 0x32B, 1, 1, NULL, NULL, true, 1.0f);
        if (emitter != NULL) {
            JUtility::TColor color = gGameRoot2_8053AB10->field_0x2c->field_0x4;
            emitter->setColorRGB(color.r, color.g, color.b);
        }
    } else {
        fn_801B9DD4(3);
    }
}

void Gibdo_804A3BF8::vfunc_0x98() {
    bool step = false;
    const u32 hits = ENEMY_FLAG_HIT | ENEMY_FLAG_HIT_PLAYER | ENEMY_FLAG_HIT_ACTOR;
    if (mState == 3 || (mLife > 0 && ((mFlags & hits) == 0 || field_0xfc != 5))) {
        step = true;
    }
    if (step) {
        fn_801CE32C();
    }
}

void Gibdo_804A3BF8::vfunc_0x9C(u8 mapHit) {
    bool step = false;
    const u32 hits = ENEMY_FLAG_HIT | ENEMY_FLAG_HIT_PLAYER | ENEMY_FLAG_HIT_ACTOR;
    if (mState == 3 || (mLife > 0 && ((mFlags & hits) == 0 || field_0xfc != 5))) {
        step = true;
    }
    if (step) {
        fn_801CE2BC(mapHit);
    }
}

void Gibdo_804A3BF8::draw() {
    SpriteParams_801F5AE0 params;
    if (mFlags & 0x4000) {
        params.field_0x0[3] = 6;
    }
    drawSprite_801F06F0("enemy_gc_sob.bin", mAnim.mCel + 0xC1, &params, 7, 1.0f, 1.0f);
}

Gibdo_804A3BF8::~Gibdo_804A3BF8() {}
