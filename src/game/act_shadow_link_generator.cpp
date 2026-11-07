#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/act_shadow_link_generator.h"
#include "game/act/act_shadow_link_2.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "game/act/act_princess_zelda_demo.h"

// Effect ids of the five emitters fn_801C1768 starts (mEmitters2) and of the
// four it starts without keeping them.
static const u16 lbl_80469424[6] = {0x3FC, 0x3FD, 0x3FE, 0x3FF, 0x400, 0x0};
static const u16 lbl_8053EDB8[4] = {0x401, 0x402, 0x403, 0x404};
// The three effect ids init() starts (mEmitters).
static const u16 lbl_8053EDA8[4] = {0x4B, 0x4C, 0x4D, 0x0};
// The three effect ids state 5 starts, and the five state 9 starts.
static const u16 lbl_8053EDB0[4] = {0xF6, 0xF7, 0xF8, 0x0};
static const u16 lbl_80469418[6] = {0x4CA, 0x4CB, 0x4CC, 0x4CD, 0x4CE, 0x0};

ShadowLinkGenerator_804A4310::ShadowLinkGenerator_804A4310()
    : mSlotFirst(0), mSlotLast(0), mWaitTimer(0), field_0x2ac(0), mEmitter(NULL), mTexture(NULL) {
    for (s32 i = 0; i < 3; i++) {
        mEmitters[i] = NULL;
    }
    for (s32 i = 0; i < 5; i++) {
        mEmitters2[i] = NULL;
    }
}

// Destroys the emitters the intro started (releaseEmitter_801EE434 releases each one).
ShadowLinkGenerator_804A4310::~ShadowLinkGenerator_804A4310() {
    releaseEmitter_801EE434(mEmitter);
    for (s32 i = 0; i < 3; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
    for (s32 i = 0; i < 5; i++) {
        releaseEmitter_801EE434(mEmitters2[i]);
    }
}

void ShadowLinkGenerator_804A4310::init() {
    EnemyBase_804A4808::init();
    mSlotFirst = (field_0x90 >> 8) & 0xFF;
    mSlotLast = (field_0x90 >> 16) & 0xFF;
    mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
    mBox2.set(mBox);
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0xb0 = 0;
    field_0x98 = 2;
    mFlags |= 0x380;
    mLife = 1000;
    mLifeBackup = mLife;
    mState = 0;
    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    static const char* lbl_8053AC18 = "jen_8_3.bti";
    mTexture = fn_804032E8('TIMG', lbl_8053AC18);
    if (vfunc_0x34()) {
        mState = 0xB;
    } else {
        mEmitter = spawnEmitter_8013CC50(field_0x4, &mPos, 0x4F, 0, 0, NULL, NULL, true, 1.0f);
        for (s32 i = 0; i < 3; i++) {
            mEmitters[i] = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_8053EDA8[i], 0, 0, NULL, NULL, true, 1.0f);
        }
    }
    fn_801C0D70()->field_0x50 = mSerial;
}

// FIXME(shotgun): 93.49 %, missing/extra
void ShadowLinkGenerator_804A4310::update() {
    mPieces.fn_8021C2B4();
    mStateTimer++;
    if (mStateTimer < 0) {
        mStateTimer = 0;
    }
    switch (mState) {
    case 0:
        if (fn_801C0D70()->field_0x0 & 0x40) {
            mStateTimer = 0;
            mState = 1;
        }
        break;
    case 1:
        if (mStateTimer >= 0xB4) {
            mWaitTimer = fn_801CD664(0x3D) + 0xB4;
            mStateTimer = 0;
            mState = 2;
        }
        break;
    case 2:
        if (mWaitTimer != 0) {
            mWaitTimer--;
            if (mWaitTimer == 0) {
                fn_801C186C();
            }
        } else if (!(fn_801C0D70()->field_0x0 & 0x40)) {
            mStateTimer = 0;
            mState = 3;
        }
        break;
    case 4:
        if (mStateTimer >= 0x3C) {
            for (s32 i = 0; i < 3; i++) {
                deleteEmitter(&mEmitters[i]);
            }
            mStateTimer = 0;
            mState = 5;
        } else {
            f32 t = 0.016666668f * (f32)(0x3C - mStateTimer);
            if (mEmitters[1] != NULL) {
                mEmitters[1]->mGlobalPScl.x = t;
                mEmitters[1]->mGlobalPScl.y = 1.0f;
            }
            if (mEmitters[2] != NULL) {
                mEmitters[2]->setGlobalAlpha((s32)(255.0f * t));
            }
        }
        break;
    case 5:
        if (mStateTimer == 0xB4) {
            startSound_801F0E34(0x5004B, 0);
        }
        if (fn_801C0D70()->field_0x0 & 0x80) {
            vfunc_0x30(0xFF, true);
            for (s32 i = 0; i < 3; i++) {
                spawnEmitter_8013CC50(field_0x4, &mPos, lbl_8053EDB0[i], 0, 0, NULL, NULL, true, 1.0f);
            }
            mStateTimer = 0;
            mState = 6;
        }
        break;
    case 6:
        startSound_801F0E34(0x5004C, 0);
        if (mStateTimer >= 0x78) {
            static const char* lbl_8053AC20 = "jen_8_3_tsume.bti";
            mTexture = fn_804032E8('TIMG', lbl_8053AC20);
            JGeometry::TVec3f at(mPos.x, mPos.y + -32.0f, 0.0f);
            mSequence.fn_8021F9AC(&at);
            mStateTimer = 0;
            mState = 7;
        }
        break;
    case 7:
        startSound_801F0E34(0x5004C, 0);
        if (mSequence.fn_8021F4B0()) {
            for (s32 i = 0; i < 5; i++) {
                deleteEmitter(&mEmitters2[i]);
            }
            s32 serial = getActorManager_801F856C()->fn_801F7884('DMZL', field_0x4);
            if (serial >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (actor != NULL) {
                    ((PrincessZeldaDemo_8049D208*)actor)->fn_8010F9F0();
                }
            }
            fn_801C0D70()->field_0x0 |= 0x1000;
            mStateTimer = 0;
            mState = 8;
        } else if (field_0x2ac != 0) {
            field_0x2ac--;
            if (field_0x2ac == 0) {
                for (s32 i = 0; i < 5; i++) {
                    deleteEmitter(&mEmitters2[i]);
                }
            } else {
                u8 alpha = (s32)(0.008333334f * (f32)field_0x2ac * 255.0f);
                for (s32 i = 0; i < 5; i++) {
                    if (mEmitters2[i] != NULL) {
                        mEmitters2[i]->setGlobalAlpha(alpha);
                    }
                }
            }
        } else if (mStateTimer == 0xF0) {
            field_0x2ac = 0x78;
        }
        break;
    case 8:
        startSound_801F0E34(0x5004C, 0);
        if (!(fn_801C0D70()->field_0x0 & 0x1000)) {
            mSequence.fn_8021F914(5);
            mStateTimer = 0;
            mState = 9;
        }
        break;
    case 9:
        if (mStateTimer >= 0x5A) {
            mSequence.fn_8021F924();
            for (s32 i = 0; i < 5; i++) {
                spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80469418[i], 0, 0, NULL, NULL, true, 1.0f);
            }
            startSound_801F0E34(0x5006F, 0);
            mStateTimer = 0;
            mState = 10;
        } else {
            mSequence.fn_8021F4B0();
        }
        break;
    case 10:
        if (mStateTimer >= 0xB4) {
            fn_8022ADCC();
            mStateTimer = 0;
            mState = 11;
        }
        break;
    }
    if (mEmitters[0] != NULL) {
        u32 frame = mEmitters[0]->mCurrentFrame;
        if (frame >= 0x14) {
            if ((frame - 0x14) % 0x5A == 0) {
                startSound_801F0E34(0x50048, 0);
            }
        }
    }
    if (mState >= 4 && mState < 7) {
        startSound_801F0E34(0x5004A, 0);
    }
    mLife = mLifeBackup;
}

// Draws the "TIMG" texture at the generator's position, on the layers below 8,
// unless the generator is in state 0xB.
void ShadowLinkGenerator_804A4310::draw() {
    if (mState != 0xB) {
        SpriteParams_801F5AE0 params;
        params.field_0x1d = -2;
        s32 layer = field_0x4;
        void* res = mTexture;
        if (layer < 8) {
            gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, res, &params);
        }
    }
}

// Spawns four 'SLK2' actors, one after another, each on a random slot from
// mSlotFirst to mSlotLast; a spawned one that is found is switched to state 0xD.
// FIXME: 99.30 %. Register allocation only: the original keeps the range in r30 and
// the spawn serial (then the found actor) in r29; ours swaps them.
void ShadowLinkGenerator_804A4310::fn_801C186C() const {
    for (s32 i = 0; i < 4; i++) {
        s32 slot = mSlotFirst + fn_801CD664(mSlotLast - mSlotFirst + 1);
        s32 serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_SLK2, field_0x4, &mPos, fn_801C5660(8, 0, slot, 0, 0), -1, -1);
        if (serial >= 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != NULL) {
                static_cast<ShadowLink2_804A43E8*>(actor)->fn_801C26D8();
            }
        }
    }
}

// Starts the intro effects (mEmitters2 keeps the five of the first loop), a
// sound, and sets state 4.
void ShadowLinkGenerator_804A4310::fn_801C1768() {
    s32 i;
    for (i = 0; i < 5; i++) {
        mEmitters2[i] = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80469424[i], 0, 0, NULL, NULL, true, 1.0f);
    }
    for (i = 0; i < 4; i++) {
        spawnEmitter_8013CC50(field_0x4, &mPos, lbl_8053EDB8[i], 0, 0, NULL, NULL, true, 1.0f);
    }
    releaseEmitter_801EE434(mEmitter);
    mEmitter = NULL;
    startSound_801F0E34(0x50049, 0);
    mStateTimer = 0;
    mState = 4;
}
