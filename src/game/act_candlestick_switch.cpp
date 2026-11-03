#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/Effect.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/DrawShadow.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/act_candlestick_switch.h"

// FIXME: 90.09 %. Same instructions; the target loads the shadow scale
// (0x254) before the 0.0f for pos.z and keeps (8, y, x, scale) in f3, f2,
// f0, f1; ours loads 0.0f first, so every float register is shifted.
void CandlestickSwitch_804A8B78::draw() {
    if (mLit == 0) {
        if (mShadowScale > 0.0f) {
            mShadowScale -= 4.0f;
            if (mShadowScale <= 0.0f) {
                mShadowScale = 0.0f;
            }
        } else {
            mShadowScale = 0.0f;
        }
    } else if (mShadowScale < 70.0f) {
        mShadowScale += 4.0f;
    } else {
        mShadowScale = 70.0f;
    }
    JGeometry::TVec3f pos(8.0f + mPos.x, 8.0f + mPos.y, 0.0f);
    drawShadow(field_0x4, &pos, JUtility::TColor(0, 0, 0, 0x80), mShadowScale);
}

void CandlestickSwitch_804A8B78::update() {
    if (mOutTimer == 0) {
        if (mEmitter23c != NULL) {
            mEmitter23c->becomeInvalidEmitter();
            mEmitter23c = NULL;
        }
    } else {
        mOutTimer--;
    }
    JGeometry::TBox2f box(mBox);
    JGeometry::TVec2<f32> p(8.0f + mPos.x, 8.0f + mPos.y);
    box.addPos(p);
    switch (mState) {
    case 0:
        if (field_0x94 == 0 && fn_8023AB94(&box, field_0x4, &p) < 0 && fn_801F2718(ACTOR_FIRE) < 0) {
            break;
        }
        if (fn_80226998(&mPos, field_0x4) == 0x3D || fn_80226998(&mPos, field_0x4) == 0x1DE) {
            mEmitter234 = fn_8013CC50(field_0x4, &mPos, 0xBD, 0, 0, NULL, NULL, true, 1.0f);
            mEmitter238 = fn_8013CC50(field_0x4, &mPos, 0x54A, 0, 0, NULL, NULL, true, 1.0f);
            vfunc_0x30(0xFF, true);
            fn_801F0E34(0x30005, 0);
            field_0xb0 = 12;
            if (mEmitter23c != NULL) {
                mEmitter23c->becomeInvalidEmitter();
                mEmitter23c = NULL;
            }
            mState = 1;
            mLitTimer = mLitTime;
            mLit = 1;
            field_0x94 = 0;
        }
        break;
    case 1:
        if (mLitTimer != 0) {
            mLitTimer--;
        }
        if (!vfunc_0x34() || (mMode == 1 && mLitTimer == 0)) {
            mEmitter23c = fn_8013CC50(field_0x4, &mPos, 0x57, 1, 0, NULL, NULL, true, 1.0f);
            mOutTimer = 40;
            if (mEmitter234 != NULL) {
                mEmitter234->becomeInvalidEmitter();
                mEmitter234 = NULL;
            }
            if (mEmitter238 != NULL) {
                mEmitter238->becomeInvalidEmitter();
                mEmitter238 = NULL;
            }
            vfunc_0x30(0xFF, false);
            field_0xb0 = 13;
            mState = 0;
            mLit = 0;
        }
        break;
    }
}

CandlestickSwitch_804A8B78::~CandlestickSwitch_804A8B78() {
    if (mEmitter234 != NULL) {
        fn_801EE434(mEmitter234);
    }
    if (mEmitter238 != NULL) {
        fn_801EE434(mEmitter238);
    }
    if (mEmitter23c != NULL) {
        fn_801EE434(mEmitter23c);
    }
}

void CandlestickSwitch_804A8B78::init() {
    mMode = (field_0x90 >> 22) & 0x1F;
    mLitTime = field_0x90 & 0xFFF;
    field_0xb0 = 13;
    field_0x94 = 0;
    mState = 0;
    mPos.x += 8.0f;
    mPos.y += 8.0f;
    mBox.i.x = -10.0f;
    mBox.i.y = -10.0f;
    mBox.f.x = 10.0f;
    mBox.f.y = 10.0f;
    mEmitter234 = NULL;
    mEmitter238 = NULL;
    mEmitter23c = NULL;
    mOutTimer = 0;
    mLitTimer = 0;
    mLit = 0;
    mShadowScale = 0.0f;
}
