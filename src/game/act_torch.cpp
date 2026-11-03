#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/GameView.h"
#include "game/DrawShadow.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/act_torch.h"

// FIXME: 99.88%. Only the stack slots of the four TColor temporaries differ:
// the original pairs each call's by-value copy and its temporary (copy at
// sp+0x20/0x18/0x10/0x8, temporary 4 bytes above it); ours puts the four
// temporaries at 0x24/0x20/0x1c/0x18 and the copies at 0x14/0x10/0xc/0x8.
// Tried a named TColor local, a static inline wrapper taking the alpha, and
// declaring scale after pos (worse).
void Torch_804A8520::draw() {
    if (fn_802D6A9C()->field_0x131f5 != 0) {
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
            drawShadow(field_0x4, &pos, JUtility::TColor(0, 0, 0, 0x80), scale);
        }
        break;
    case 1: {
        f32 scale = mShadowScale;
        JGeometry::TVec3f pos(mPos.x, mPos.y - 4.0f, mPos.z);
        drawShadow(field_0x4, &pos, JUtility::TColor(0, 0, 0, 0xC8), scale);
        break;
    }
    case 2:
        if (mOn == 1) {
            lbl_8053AB10->field_0x2c->field_0xd[0] = 1;
            f32 scale = mShadowScale;
            JGeometry::TVec3f pos(8.0f + mPos.x, 8.0f + mPos.y, 0.0f);
            drawShadow(field_0x4, &pos, JUtility::TColor(0, 0, 0, 0x80), scale);
        }
        break;
    case 3: {
        lbl_8053AB10->field_0x2c->field_0xd[0] = 1;
        f32 scale = mShadowScale;
        JGeometry::TVec3f pos(mPos.x, mPos.y - 4.0f, mPos.z);
        drawShadow(field_0x4, &pos, JUtility::TColor(0, 0, 0, 0xC8), scale);
        break;
    }
    }
}

void Torch_804A8520::update() {
    switch (mState) {
    case 0:
        if (fn_802D6A9C()->fn_802D800C(mViewBit) || field_0x94 != 0) {
            mOn = 1;
            if (mType == 0 || mType == 2 || mType == 4) {
                mEmitter230 = fn_8013CC50(field_0x4, &mPos, 0xBD, 0, 0, NULL, NULL, true, 1.0f);
                if (mType != 4) {
                    mEmitter234 = fn_8013CC50(field_0x4, &mPos, 0x54A, 0, 0, NULL, NULL, true, 1.0f);
                }
            }
            fn_801F0E34(0x30005, 0);
            field_0xb0 = 12;
            mState = 1;
            field_0x24c = 1;
            field_0x94 = 0;
        }
        break;
    case 1: {
        s32 bit = mViewBit;
        if (bit != 0 && !fn_802D6A9C()->fn_802D800C(bit)) {
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
    if (bit == 0 || fn_802D6A9C()->fn_802D800C(bit)) {
        mOn = 1;
        if (mType == 0 || mType == 2 || mType == 4) {
            mEmitter230 = fn_8013CC50(field_0x4, &mPos, 0xBD, 0, 0, NULL, NULL, true, 1.0f);
            if (mType != 4) {
                mEmitter234 = fn_8013CC50(field_0x4, &mPos, 0x54A, 0, 0, NULL, NULL, true, 1.0f);
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
