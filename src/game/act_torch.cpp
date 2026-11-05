#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/GameView.h"
#include "game/DrawShadow.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/act_torch.h"

// Builds the colour inside, which pairs each temporary with its by-value copy as the original does.
static inline void drawTorchShadow(s32 layer, const JGeometry::TVec3f* pos, u8 alpha, f32 scale) {
    gGameRoot2_8053AB10->field_0x2c->drawShadow_80158B5C(layer, pos, JUtility::TColor(0, 0, 0, alpha), scale);
}

void Torch_804A8520::draw() {
    if (getGameView_802D6A9C()->field_0x131f5 != 0) {
        if (mShadowScale > 0.0f) {
            mShadowScale -= 8.0f;
            if (mShadowScale <= 0.0f) {
                mShadowScale = 0.0f;
            }
        }
    } else if (field_0x24c == 0) {
        if (mShadowScale > 0.0f) {
            mShadowScale -= 4.0f;
            if (mShadowScale <= 0.0f) {
                mShadowScale = 0.0f;
            }
        } else {
            mShadowScale = 0.0f;
        }
    } else if (mShadowScale < mShadowMax) {
        mShadowScale += 4.0f;
    } else {
        mShadowScale = mShadowMax;
    }
    switch (mType) {
    case 0:
        if (mOn == 1) {
            f32 scale = mShadowScale;
            JGeometry::TVec3f pos(8.0f + mPos.x, 8.0f + mPos.y, 0.0f);
            drawTorchShadow(field_0x4, &pos, 0x80, scale);
        }
        break;
    case 1: {
        f32 scale = mShadowScale;
        JGeometry::TVec3f pos(mPos.x, mPos.y - 4.0f, mPos.z);
        drawTorchShadow(field_0x4, &pos, 0xC8, scale);
        break;
    }
    case 2:
        if (mOn == 1) {
            gGameRoot2_8053AB10->field_0x2c->field_0xd[0] = 1;
            f32 scale = mShadowScale;
            JGeometry::TVec3f pos(8.0f + mPos.x, 8.0f + mPos.y, 0.0f);
            drawTorchShadow(field_0x4, &pos, 0x80, scale);
        }
        break;
    case 3: {
        gGameRoot2_8053AB10->field_0x2c->field_0xd[0] = 1;
        f32 scale = mShadowScale;
        JGeometry::TVec3f pos(mPos.x, mPos.y - 4.0f, mPos.z);
        drawTorchShadow(field_0x4, &pos, 0xC8, scale);
        break;
    }
    }
}

void Torch_804A8520::update() {
    switch (mState) {
    case 0:
        if (getGameView_802D6A9C()->fn_802D800C(mViewBit) || field_0x94 != 0) {
            mOn = 1;
            if (mType == 0 || mType == 2 || mType == 4) {
                mEmitter230 = spawnEmitter_8013CC50(field_0x4, &mPos, 0xBD, 0, 0, NULL, NULL, true, 1.0f);
                if (mType != 4) {
                    mEmitter234 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x54A, 0, 0, NULL, NULL, true, 1.0f);
                }
            }
            startSound_801F0E34(0x30005, 0);
            field_0xb0 = 12;
            mState = 1;
            field_0x24c = 1;
            field_0x94 = 0;
        }
        break;
    case 1: {
        s32 bit = mViewBit;
        if (bit != 0 && !getGameView_802D6A9C()->fn_802D800C(bit)) {
            if (mEmitter230 != NULL) {
                mEmitter230->becomeInvalidEmitter();
                mEmitter230 = NULL;
            }
            if (mEmitter234 != NULL) {
                mEmitter234->becomeInvalidEmitter();
                mEmitter234 = NULL;
            }
            field_0xb0 = 13;
            mOn = 0;
            mState = 0;
            field_0x24c = 0;
        }
        break;
    }
    }
}

Torch_804A8520::~Torch_804A8520() {
    if (mEmitter230 != NULL) {
        fn_801EE434(mEmitter230);
    }
    if (mEmitter234 != NULL) {
        fn_801EE434(mEmitter234);
    }
}

void Torch_804A8520::init() {
    mType = field_0x90 & 0x7F;
    mViewBit = field_0x90 >> 27;
    field_0x24c = 0;
    mShadowScale = 0.0f;
    if (mType == 0 || mType == 2) {
        mShadowMax = 70.0f;
    } else {
        mShadowMax = 20.0f;
    }
    field_0x94 = 0;
    mEmitter230 = NULL;
    mEmitter234 = NULL;
    s32 bit = mViewBit;
    if (bit == 0 || getGameView_802D6A9C()->fn_802D800C(bit)) {
        mOn = 1;
        if (mType == 0 || mType == 2 || mType == 4) {
            mEmitter230 = spawnEmitter_8013CC50(field_0x4, &mPos, 0xBD, 0, 0, NULL, NULL, true, 1.0f);
            if (mType != 4) {
                mEmitter234 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x54A, 0, 0, NULL, NULL, true, 1.0f);
            }
        }
        field_0xb0 = 12;
        mState = 1;
        field_0x24c = 1;
    } else {
        field_0xb0 = 13;
        mState = 0;
        mOn = 0;
    }
}
