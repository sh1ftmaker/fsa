#include "game/GameAudio.h"
#include "game/act/act_chain_switch.h"
#include "game/GameView.h"

ChainSwitch_804AA3E8::~ChainSwitch_804AA3E8() {}

void ChainSwitch_804AA3E8::draw() {}

void ChainSwitch_804AA3E8::update() {
    if (mTimer != 0) {
        mTimer--;
        if (mTimer == 0) {
            if (mSoundMode == 1) {
                vfunc_0x30(0xFF, true);
            } else if (mSoundMode == 2) {
                vfunc_0x30(0xFF, false);
            }
            if (mSwitchMode == 0) {
                fn_802D6A9C()->fn_802D8050(mSwitchBit, true, 0xFF);
            } else if (mSwitchMode == 1) {
                fn_802D6A9C()->fn_802D8050(mSwitchBit, false, 0xFF);
            }
            if (mCallWhenDone == 1) {
                vfunc_0x24();
            }
        }
    } else {
        if (!mWasOn) {
            if (vfunc_0x34() == 1) {
                mTimer = mDelay;
            }
        }
        mWasOn = vfunc_0x34();
    }
}

void ChainSwitch_804AA3E8::init() {
    mDelay = field_0x90 & 0x1FFFF;
    mCallWhenDone = (field_0x90 >> 17) & 1;
    mSwitchMode = (field_0x90 >> 18) & 3;
    mSoundMode = (field_0x90 >> 20) & 3;
    mSwitchBit = (field_0x90 >> 22) & 0x1F;
    field_0x242 = field_0x90 >> 27;
    mTimer = 0;
    mWasOn = vfunc_0x34();
}
