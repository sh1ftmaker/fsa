#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/act/act_wait_switch.h"

WaitSwitch_804A8B20::~WaitSwitch_804A8B20() {}

void WaitSwitch_804A8B20::draw() {}

void WaitSwitch_804A8B20::update() {
    switch (mState) {
    case 0:
        if (fn_802D6A9C()->fn_802D800C(mBit)) {
            mState = 1;
            if (mMode >= 1 && mMode <= 5) {
                if (!fn_802D6A9C()->fn_802D75BC(field_0x16c)) {
                    lbl_8053AAF8->fn_80134DE4(10, mCount);
                }
            }
        }
        break;
    case 1:
        if (!fn_802D6A9C()->fn_802D75BC(field_0x16c)) {
            if (mMode >= 1 && mMode <= 5) {
                fn_802D6A9C()->fn_802D79E8(field_0x4, mValue, 0.0f, false);
                mStepTimer--;
                if (mStepTimer == 0) {
                    mStepTimer = mStepPeriod;
                    if (mMode == 4) {
                        mValue += 1.0f;
                        if (mValue >= 5.0f) {
                            mValue = 5.0f;
                        }
                    } else if (mMode == 5) {
                        mValue -= 1.0f;
                        if (mValue <= 1.0f) {
                            mValue = 1.0f;
                        }
                    }
                }
            }
            mTimer--;
        } else {
            mTimer = 0;
        }
        if (mTimer == 0) {
            if (mMode == 6) {
                mTimer = mCount;
                mToggle ^= 1;
                if (mToggle) {
                    vfunc_0x30(0xFF, true);
                } else {
                    vfunc_0x30(0xFF, false);
                }
            } else {
                vfunc_0x30(0xFF, true);
                mState = 2;
            }
        }
        break;
    case 2:
        fn_801F3518();
        break;
    }
}

void WaitSwitch_804A8B20::init() {
    static const f32 lbl_8046D448[6] = {0.0f, 2.0f, 4.0f, 5.0f, 1.0f, 5.0f};

    mCount = field_0x90 & 0x3FFFF;
    mMode = (field_0x90 >> 19) & 7;
    mBit = (field_0x90 >> 22) & 0x1F;
    field_0x254 = (field_0x90 >> 18) & 1;
    mToggle = 0;
    mTimer = mCount;
    mValue = lbl_8046D448[mMode];
    mStepTimer = 1;
    mStepPeriod = 1;
    if (mMode == 4 || mMode == 5) {
        mStepTimer = mTimer / 5;
        mStepPeriod = mStepTimer;
    }
    mState = 0;
}
