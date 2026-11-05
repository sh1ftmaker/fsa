#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/GameView.h"
#include "game/DrawShadow.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/act_spotlight.h"

void Spotlight_804A84C8::draw() {
    if (mOn == 0) {
        if (mShadowScale > 0.0f) {
            mShadowScale -= 6.0f;
            if (mShadowScale <= 0.0f) {
                mShadowScale = 0.0f;
            }
        } else {
            mShadowScale = 0.0f;
        }
    } else if (mShadowScale < 200.0f) {
        mShadowScale += 6.0f;
    } else {
        mShadowScale = 200.0f;
    }
    f32 scale = mShadowScale;
    JGeometry::TVec3f pos(8.0f + mPos.x, 8.0f + mPos.y, 0.0f);
    drawShadow(field_0x4, &pos, JUtility::TColor(0, 0, 0, 0x80), scale);
}

void Spotlight_804A84C8::update() {
    switch (mState) {
    case 0:
        if (vfunc_0x34() || getGameView_802D6A9C()->fn_802D800C(mViewBit0) ||
            getGameView_802D6A9C()->fn_802D800C(mViewBit1) || getGameView_802D6A9C()->fn_802D800C(mViewBit2)) {
            mOn = 1;
            if (mEmitter238 == NULL) {
                mEmitter238 = spawnEmitter_8013CC50(field_0x4, &mPos, 0xBE, 0, 0, NULL, NULL, true, 1.0f);
            }
            if (mEmitter23c == NULL) {
                mEmitter23c = spawnEmitter_8013CC50(field_0x4, &mPos, 0xBF, 0, 0, NULL, NULL, true, 1.0f);
            }
            if (mEmitter240 == NULL) {
                mEmitter240 = spawnEmitter_8013CC50(field_0x4, &mPos, 0xC0, 0, 0, NULL, NULL, true, 1.0f);
            }
            if (mEmitter234 == NULL) {
                mEmitter234 = spawnEmitter_8013CC50(field_0x4, &mPos, 0xBD, 0, 0, NULL, NULL, true, 1.0f);
            }
            if (mEmitter244 == NULL) {
                mEmitter244 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x54A, 0, 0, NULL, NULL, true, 1.0f);
            }
            startSound_801F0E34(0x30005, 0);
            startSound_801F0E34(0x30023, 0);
            mState = 1;
        }
        break;
    case 1:
        if (!(vfunc_0x34() || getGameView_802D6A9C()->fn_802D800C(mViewBit0) ||
              getGameView_802D6A9C()->fn_802D800C(mViewBit1) || getGameView_802D6A9C()->fn_802D800C(mViewBit2))) {
            mOn = 0;
            if (mEmitter238 != NULL) {
                mEmitter238->becomeInvalidEmitter();
                mEmitter238 = NULL;
            }
            if (mEmitter23c != NULL) {
                mEmitter23c->becomeInvalidEmitter();
                mEmitter23c = NULL;
            }
            if (mEmitter240 != NULL) {
                mEmitter240->becomeInvalidEmitter();
                mEmitter240 = NULL;
            }
            if (mEmitter234 != NULL) {
                mEmitter234->becomeInvalidEmitter();
                mEmitter234 = NULL;
            }
            if (mEmitter244 != NULL) {
                mEmitter244->becomeInvalidEmitter();
                mEmitter244 = NULL;
            }
            mBox.i.x = 1.0f;
            mBox.i.y = 1.0f;
            mBox.f.x = -1.0f;
            mBox.f.y = -1.0f;
            mState = 0;
        } else {
            mBox.i.x = -56.0f;
            mBox.i.y = -56.0f;
            mBox.f.x = 56.0f;
            mBox.f.y = 56.0f;
            fn_801F2B20(0);
        }
        break;
    }
}

Spotlight_804A84C8::~Spotlight_804A84C8() {
    if (mEmitter234 != NULL) {
        fn_801EE434(mEmitter234);
    }
    if (mEmitter238 != NULL) {
        fn_801EE434(mEmitter238);
    }
    if (mEmitter23c != NULL) {
        fn_801EE434(mEmitter23c);
    }
    if (mEmitter240 != NULL) {
        fn_801EE434(mEmitter240);
    }
    if (mEmitter244 != NULL) {
        fn_801EE434(mEmitter244);
    }
}

void Spotlight_804A84C8::init() {
    mViewBit0 = (field_0x90 >> 22) & 0x1F;
    mViewBit1 = (field_0x90 >> 17) & 0x1F;
    mViewBit2 = (field_0x90 >> 12) & 0x1F;
    mBox.i.x = 1.0f;
    mBox.i.y = 1.0f;
    mBox.f.x = -1.0f;
    mBox.f.y = -1.0f;
    mEmitter234 = NULL;
    mEmitter238 = NULL;
    mEmitter23c = NULL;
    mEmitter240 = NULL;
    mEmitter244 = NULL;
    mState = 0;
    mOn = 0;
    mShadowScale = 0.0f;
}
