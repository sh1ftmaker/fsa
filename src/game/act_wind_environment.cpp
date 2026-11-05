// Actor WIND ("Wind environment"): pushes the players on its layer in a
// fixed direction and shows a wind effect over the visible area.

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/Effect.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/Map.h"
#include "game/act/act_wind_environment.h"
#include "JSystem/JParticle/JPAEmitter.h"

// FIXME: 97.0 %. After fn_80226850 the original does `subi r0, r3, 0x3ee;
// clrlwi r0, r0, 16; cmplwi r0, 1; ble` (one unsigned range test on the call
// result); this compiles to two compares (cmplwi 0x3ee, cmplwi 0x3ef). The
// forms that give the single test (`- 0x3EE > 1u`, with or without a u16
// local) add a clrlwi of the result and make MWCC inline this function into
// update, so they were not kept.
void WindEnvironment_8049CC98::fn_80106ADC(s32 state) {
    JGeometry::TVec3f push(0.0f, 0.0f, 0.0f);
    f32 speed = 1.0f;
    switch (state) {
    case 2:
        speed *= 2.0f;
    case 1:
        switch (mKind) {
        case 3:
            push.set(0.0f, -2.5f, 0.0f);
            break;
        case 2:
            push.set(0.0f, 2.5f, 0.0f);
            break;
        case 1:
            push.set(-speed, 0.0f, 0.0f);
            break;
        case 0:
            push.set(speed, 0.0f, 0.0f);
            break;
        }
        break;
    case 0: // update's state 1 can reset mState to 0 before this call; the empty case gives the original's dispatch
        break;
    }
    for (s32 i = 0; i < 4; i++) {
        if (field_0x4 == getPlayerLayer_8023DE58(i) && fn_8023F264(i) && !fn_80234D94(i) && !fn_8022D75C(i) &&
            !fn_80230188(i) && (fn_8022CAF8(i) != 4 || !fn_8022F478(i))) {
            u16 tile = fn_80226850(getPlayerPos_8023E724(i), getPlayerLayer_8023DE58(i));
            if (tile < 0x3EE || tile > 0x3EF) {
                fn_8023DFF4(i, &push);
            }
        }
    }
}

inline bool WindEnvironment_8049CC98::shouldBlow() {
    switch (mCondition) {
    case 1:
        if (getGameView_802D6A9C()->field_0x171 != 0) {
            return true;
        }
        return false;
    case 2:
        if (getGameView_802D6A9C()->field_0x171 == 0) {
            return true;
        }
        return false;
    case 0:
    default:
        if (vfunc_0x3C() <= 0) {
            return true;
        }
        return vfunc_0x34();
    }
}

void WindEnvironment_8049CC98::update() {
    ActorBase_804A5770::update();
    switch (mState) {
    case 0:
        if (shouldBlow()) {
            if (mEmitter != NULL) {
                mEmitter->playCreateParticle();
            }
            mState = 1;
        }
        break;
    case 1:
        if (!shouldBlow()) {
            if (mEmitter != NULL) {
                mEmitter->stopCreateParticle();
            }
            mState = 0;
        }
        if (mSound != NULL) {
            mSound->startSound(0x50027);
        }
        fn_80106ADC(mState);
        break;
    case 2:
        fn_80106ADC(mState);
        break;
    }
    mTimer++;
}

JAISoundHandle* ActorSoundObj_804578F4::startSound(JAISoundID id) {
    return fn_80456334(id, 0);
}

// The visible box is passed by value: that is what makes the original copy it to
// the stack (lwz/stw) before reading it with lfs.
static inline void setCenter(JGeometry::TVec3f& pos, JGeometry::TBox2f box) {
    pos.x = box.f.x + 0.5f * (box.i.x - box.f.x);
    pos.y = box.f.y + 0.5f * (box.i.y - box.f.y);
    pos.z = 0.0f;
}

// FIXME: 99.7 %, regalloc only: the centre's x and y are computed into f2 and
// f0 where the original has f0 and f1 (the box copy, with its stack slot, matches).
void WindEnvironment_8049CC98::init() {
    ActorBase_804A5770::init();
    mKind = field_0x90 & 0xF;
    mCondition = (field_0x90 >> 4) & 0xF;
    s32 kind = mKind;
    JGeometry::TVec3f pos;
    setCenter(pos, getGameView_802D6A9C()->mVisibleBox);
    switch (kind) {
    case 0:
        mEmitter = spawnEmitter_8013CC50(field_0x4, &pos, 0x505, 0, 0, NULL, NULL, true, 1.0f);
        break;
    case 1:
        mEmitter = spawnEmitter_8013CC50(field_0x4, &pos, 0x504, 0, 0, NULL, NULL, true, 1.0f);
        break;
    }
    if (mEmitter != NULL) {
        mEmitter->stopCreateParticle();
    }
}

WindEnvironment_8049CC98::~WindEnvironment_8049CC98() {
    releaseEmitter_801EE434(mEmitter);
    mEmitter = NULL;
    if (mSound != NULL) {
        delete mSound;
        mSound = NULL;
    }
}

WindEnvironment_8049CC98::WindEnvironment_8049CC98() {
    mState = 0;
    mKind = 3;
    mCondition = 0;
    mTimer = 0;
    mEmitter = NULL;
    mSound = NULL;
    mSound = new ActorSoundObj_804578F4(NULL, NULL, 8);
}
