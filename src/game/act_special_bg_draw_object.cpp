#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/act_special_bg_draw_object.h"

SpecialBGDrawObject_804AC760::~SpecialBGDrawObject_804AC760() {}

void SpecialBGDrawObject_804AC760::draw() {}

void SpecialBGDrawObject_804AC760::update() {
    switch (mState) {
    case 0:
        if (vfunc_0x34() == true) {
            mTimer = 0;
            mState = 1;
            switch (mKind) {
            case 0:
                if (!fn_802DEBA0()) {
                    fn_801F0E34(0x30043, 0);
                }
                break;
            case 1:
                if (!fn_802DEBA0()) {
                    fn_801F0E34(0x30044, 0);
                }
                break;
            }
        }
        break;
    case 1:
        switch (mKind) {
        case 0:
            if (mTimer >= 120) {
                if (!fn_802D6A9C()->fn_802D800C(mBit1)) {
                    fn_802D6A9C()->fn_802D8050(mBit1, true, 0xFF);
                }
                mTimer = 120;
            }
            if (mTimer < 120) {
                mTimer++;
                if (mTimer == 120) {
                    if (!fn_802DEBA0()) {
                        fn_801F0D20(3);
                    }
                }
            }
            break;
        case 1:
            if (mTimer >= 30) {
                if (!fn_802D6A9C()->fn_802D800C(mBit1)) {
                    fn_802D6A9C()->fn_802D8050(mBit1, true, 0xFF);
                }
                mTimer = 30;
            }
            if (mTimer < 30) {
                mTimer++;
                if (mTimer == 30) {
                    if (!fn_802DEBA0()) {
                        fn_801F0D20(3);
                    }
                }
            }
            break;
        case 2:
            if (vfunc_0x34() == true) {
                if (mTimer < 120) {
                    if (mSoundState != 1) {
                        fn_801F0E34(0x30045, 0);
                        mSoundState = 1;
                    }
                    mTimer++;
                    if (mTimer >= 60 && mTriggered == 0) {
                        fn_801F0D20(3);
                        mTriggered = 1;
                    }
                    if (mTimer >= 120) {
                        mTimer = 120;
                        mSoundState = 0;
                    }
                }
            } else if (mTimer > 0) {
                if (mSoundState != -1) {
                    fn_801F0E34(0x30046, 0);
                    mSoundState = -1;
                }
                mTimer -= 4;
                if (mTimer <= 0) {
                    mState = 0;
                    mTimer = 0;
                    mSoundState = 0;
                }
            }
            break;
        }
        break;
    }

    switch (mKind) {
    case 0: {
        f32 value = mTimer / 120.0f;
        GameRootSub20_801902C0::Request& req = fn_801902C0()->mRequests[0];
        if (req.field_0xc != NULL && req.mActive == 0) {
            req.mActive = 1;
            req.mPos.x = mPos.x;
            req.mPos.y = mPos.y;
            req.mPos.z = mPos.z;
            req.mValue = value;
        }
        break;
    }
    case 1:
        if (!fn_802D6A9C()->fn_802D800C(mBit2)) {
            f32 value = mTimer / 30.0f;
            GameRootSub20_801902C0::Request& req = fn_801902C0()->mRequests[1];
            if (req.field_0xc != NULL && req.mActive == 0) {
                req.mActive = 1;
                req.mPos.x = mPos.x;
                req.mPos.y = mPos.y;
                req.mPos.z = mPos.z;
                req.mValue = value;
            }
        }
        break;
    case 2: {
        f32 value = mTimer / 120.0f;
        GameRootSub20_801902C0::Request& req = fn_801902C0()->mRequests[2];
        if (req.field_0xc != NULL && req.mActive == 0) {
            req.mActive = 1;
            req.mPos.x = mPos.x;
            req.mPos.y = mPos.y;
            req.mPos.z = mPos.z;
            req.mValue = value;
        }
        break;
    }
    }
}

void SpecialBGDrawObject_804AC760::init() {
    mKind = field_0x90 & 0xF;
    mBit1 = (field_0x90 >> 22) & 0x1F;
    mBit2 = (field_0x90 >> 17) & 0x1F;
    mSoundState = 0;
    mTriggered = 0;
    mTimer = 0;
    if (fn_802D6A9C()->fn_802D800C(mBit1) == true) {
        switch (mKind) {
        case 0:
            mTimer = 120;
            break;
        case 1:
            mTimer = 30;
            break;
        case 2:
            mTimer = 120;
            break;
        }
        mState = 1;
    } else {
        mTimer = 0;
        mState = 0;
    }
    update();
}
