#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/act_fairy_revival.h"

void RevivalFairy_804AC250::draw() {
    SpriteParams_801F5AE0 params;
    if (mLifeTime >= 60 || mTimer % 2 == 0) {
        params.field_0x2d = 1;
        drawSprite_801F06F0("enemy_both2_sob.bin", mFrame + 0x5e, &params, 1, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, mFrame + 0x5e, false, 2, -1, 0, 0, 0);
    }
}

void RevivalFairy_804AC250::update() {
    addCalc2_801F71A4(&field_0x3c.x, mTargetVel.x, 0.1f, 0.5f);
    addCalc2_801F71A4(&field_0x3c.y, mTargetVel.y, 0.1f, 0.5f);
    addCalc2_801F71A4(&mPos.z, mTargetVel.z, 0.3f, 0.5f);
    mPos += field_0x3c;
    if (mEmitter254 != NULL) {
        mEmitter254->mGlobalTrs.set(mPos);
    }
    if (mEmitter258 != NULL) {
        mEmitter258->mGlobalTrs.set(mPos);
    }
    switch (mState) {
    case 0:
        if (mTimer % 5 == 0) {
            f32 dx = mPos.x - fn_8023E724(mPlayer)->x;
            f32 dy = mPos.y - fn_8023E724(mPlayer)->y;
            mAngle = JMath::atanTable_.atan2_(-dx, -dy) * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
        }
        mTargetVel.x = 5.0f * JMASinShort(mAngle);
        mTargetVel.y = 5.0f * JMACosShort(mAngle);
        break;
    }
    if (mTimer % 4 == 0) {
        mFrame++;
        if (mFrame > 2) {
            mFrame = 0;
        }
    }
    mLifeTime--;
    if (mLifeTime <= 0) {
        if (mEmitter254 != NULL) {
            mEmitter254->becomeInvalidEmitter();
        }
        if (mEmitter258 != NULL) {
            mEmitter258->becomeInvalidEmitter();
        }
        fn_801F3668();
    }
    mTimer++;
}

void RevivalFairy_804AC250::init() {
    mPlayer = field_0x90 & 3;
    mLife = 8;
    mFrame = 0;
    mTargetVel.set(0.0f, 0.0f, 0.0f);
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    field_0x3c.x = 5.0f;
    mTargetVel.z = 10.0f;
    field_0x3c.y = -3.0f;
    mTimer = 0;
    mLifeTime = 180;
    mAngle = 0;
    mState = 0;
    mEmitter254 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x183, 0, 0, NULL, NULL, true, 1.0f);
    mEmitter258 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x184, 0, 0, NULL, NULL, true, 1.0f);
}

RevivalFairy_804AC250::~RevivalFairy_804AC250() {
    fn_801EE434(mEmitter254);
    fn_801EE434(mEmitter258);
}
