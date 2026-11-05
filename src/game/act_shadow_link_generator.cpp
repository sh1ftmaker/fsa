#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/act_shadow_link_generator.h"
#include "game/act/act_shadow_link_2.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"

// Effect ids of the five emitters fn_801C1768 starts (mEmitters2) and of the
// four it starts without keeping them.
static const u16 lbl_80469424[6] = {0x3FC, 0x3FD, 0x3FE, 0x3FF, 0x400, 0x0};
static const u16 lbl_8053EDB8[4] = {0x401, 0x402, 0x403, 0x404};
// The three effect ids init() starts (mEmitters).
static const u16 lbl_8053EDA8[4] = {0x4B, 0x4C, 0x4D, 0x0};

// Starts the intro effects (mEmitters2 keeps the five of the first loop), a
// sound, and sets state 4.
void ShadowLinkGenerator_804A4310::fn_801C1768() {
    s32 i;
    for (i = 0; i < 5; i++) {
        mEmitters2[i] = fn_8013CC50(field_0x4, &mPos, lbl_80469424[i], 0, 0, NULL, NULL, true, 1.0f);
    }
    for (i = 0; i < 4; i++) {
        fn_8013CC50(field_0x4, &mPos, lbl_8053EDB8[i], 0, 0, NULL, NULL, true, 1.0f);
    }
    fn_801EE434(mEmitter);
    mEmitter = NULL;
    fn_801F0E34(0x50049, 0);
    mStateTimer = 0;
    mState = 4;
}

// Spawns four 'SLK2' actors, one after another, each on a random slot from
// mSlotFirst to mSlotLast; a spawned one that is found is switched to state 0xD.
// FIXME: 99.30 %. Register allocation only: the original keeps the range in r30 and
// the spawn serial (then the found actor) in r29; ours swaps them.
void ShadowLinkGenerator_804A4310::fn_801C186C() const {
    for (s32 i = 0; i < 4; i++) {
        s32 slot = mSlotFirst + fn_801CD664(mSlotLast - mSlotFirst + 1);
        s32 serial = fn_801F856C()->fn_801F9484(ACTOR_SLK2, field_0x4, &mPos, fn_801C5660(8, 0, slot, 0, 0), -1, -1);
        if (serial >= 0) {
            ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(serial);
            if (actor != NULL) {
                static_cast<ShadowLink2_804A43E8*>(actor)->fn_801C26D8();
            }
        }
    }
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
            lbl_8053AB10->field_0x20->fn_802F745C(&mPos, res, &params);
        }
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
        mEmitter = fn_8013CC50(field_0x4, &mPos, 0x4F, 0, 0, NULL, NULL, true, 1.0f);
        for (s32 i = 0; i < 3; i++) {
            mEmitters[i] = fn_8013CC50(field_0x4, &mPos, lbl_8053EDA8[i], 0, 0, NULL, NULL, true, 1.0f);
        }
    }
    fn_801C0D70()->field_0x50 = mSerial;
}

// Destroys the emitters the intro started (fn_801EE434 releases each one).
ShadowLinkGenerator_804A4310::~ShadowLinkGenerator_804A4310() {
    fn_801EE434(mEmitter);
    for (s32 i = 0; i < 3; i++) {
        fn_801EE434(mEmitters[i]);
    }
    for (s32 i = 0; i < 5; i++) {
        fn_801EE434(mEmitters2[i]);
    }
}

ShadowLinkGenerator_804A4310::ShadowLinkGenerator_804A4310()
    : mSlotFirst(0), mSlotLast(0), mWaitTimer(0), field_0x2ac(0), mEmitter(NULL), mTexture(NULL) {
    for (s32 i = 0; i < 3; i++) {
        mEmitters[i] = NULL;
    }
    for (s32 i = 0; i < 5; i++) {
        mEmitters2[i] = NULL;
    }
}
