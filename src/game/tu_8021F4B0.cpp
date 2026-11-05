#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "game/GameGlobals.h"
#include "game/tu_8021F4B0.h"
#include "JSystem/JParticle/JPAEmitter.h"

// Effect ids of the three emitters (spawnEmitter_8013CC50).
static const u16 lbl_8053FDD0[3] = {0x3CC, 0x3CD, 0x3CE};

// The loop of fn_8021F924, inlined into the end of state 5.
static inline void deleteEmitters(cls_8021FB44* self) {
    for (s32 i = 0; i < 3; i++) {
        deleteEmitter(&self->mEmitters[i]);
    }
}

// Moves the three emitters to pos.
static inline void followEmitters(JPABaseEmitter** emitters, const JGeometry::TVec3f* pos) {
    for (s32 i = 0; i < 3; i++) {
        if (emitters[i]) {
            setEmitterPos_8013CB44(emitters[i], pos, 0, 0, true);
        }
    }
}

// One step of an approach to `target` with `remaining` frames left.
static inline void approach(f32& cur, f32 target, f32 remaining) {
    if (remaining < FLT_EPSILON) {
        cur = target;
    } else {
        cur += (1.0f / remaining) * (target - cur);
    }
}

bool cls_8021FB44::fn_8021F4B0() {
    bool result = false;
    mTimer++;
    if (mTimer < 0) {
        mTimer = 0;
    }
    switch (mState) {
    case 1:
        if (mTimer >= 240) {
            mTimer = 0;
            mState = 2;
        } else {
            f32 t = 240 - mTimer;
            approach(mPos.x, mTarget1.x, t);
            approach(mPos.y, mTarget1.y, t);
            approach(mPos.z, mTarget1.z, t);
            followEmitters(mEmitters, &mPos);
        }
        break;
    case 2:
        if (mTimer >= 60) {
            mTimer = 0;
            mState = 3;
        }
        break;
    case 3:
        if (mTimer >= 120) {
            mTimer = 0;
            mState = 4;
            result = true;
        } else {
            f32 t = 120 - mTimer;
            approach(mPos.x, mTarget2.x, t);
            approach(mPos.y, mTarget2.y, t);
            approach(mPos.z, mTarget2.z, t);
            followEmitters(mEmitters, &mPos);
        }
        break;
    case 4:
        result = true;
        break;
    case 5:
        result = true;
        if (mTimer >= 60) {
            deleteEmitters(this);
            mTimer = 0;
            mState = 0;
        } else {
            f32 t = 60 - mTimer;
            approach(mPos.z, mEndZ, t);
            approach(mScale, 0.0f, t);
            followEmitters(mEmitters, &mPos);
            f32 scale = mScale;
            for (s32 i = 0; i < 3; i++) {
                if (mEmitters[i]) {
                    mEmitters[i]->setScale(scale);
                }
            }
        }
        break;
    }
    return result;
}

void cls_8021FB44::fn_8021F914(s32 state) {
    mTimer = 0;
    mState = state;
}

void cls_8021FB44::fn_8021F924() {
    for (s32 i = 0; i < 3; i++) {
        deleteEmitter(&mEmitters[i]);
    }
    mTimer = 0;
    mState = 0;
}

void cls_8021FB44::fn_8021F9AC(const JGeometry::TVec3f* pos) {
    mPos.x = pos->x;
    mPos.y = pos->y;
    mPos.z = pos->z;
    mTarget1.set(mPos.x, mPos.y, 96.0f + mPos.z);
    mTarget2.set(mPos.x, -32.0f + mPos.y, 54.0f + mPos.z);
    mEndZ = 32.0f + mPos.z;
    mTimer = 0;
    mState = 1;
    for (s32 i = 0; i < 3; i++) {
        mEmitters[i] = spawnEmitter_8013CC50(0, &mPos, lbl_8053FDD0[i], 0, 0, NULL, NULL, true, mScale);
    }
}

cls_8021FB44::~cls_8021FB44() {
    fn_8021F924();
}

cls_8021FB44::cls_8021FB44() {
    mState = 0;
    mTimer = 0;
    mPos.x = 0.0f;
    mPos.y = 0.0f;
    mPos.z = 0.0f;
    mTarget1.x = 0.0f;
    mTarget1.y = 0.0f;
    mTarget1.z = 0.0f;
    mTarget2.x = 0.0f;
    mTarget2.y = 0.0f;
    mTarget2.z = 0.0f;
    mEndZ = 0.0f;
    mScale = 1.0f;
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
    mEmitters[2] = NULL;
}
