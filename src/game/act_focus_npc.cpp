#include "game/GameAudio.h"
#include "game/act/act_focus_npc.h"
#include "game/GameGlobals.h"

FocusNPC_804A8EA0::~FocusNPC_804A8EA0() {}

void FocusNPC_804A8EA0::draw() {}

void FocusNPC_804A8EA0::update() {
    s32 sw;
    switch (mState) {
    case 0:
        sw = mSwitch;
        if (sw == 0 || getGameView_802D6A9C()->fn_802D800C(sw)) {
            gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
            if (gGameRoot2_8053AB10->field_0x24->fn_8030C2A8(&mPos) == 1) {
                if ((field_0x90 >> 27) != 0) {
                    vfunc_0x30(0xFF, true);
                }
                mState = 1;
                mTimer = mDuration;
            }
        }
        break;
    case 1:
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        if (mTimer == 0) {
            mAlive = false;
        } else {
            mTimer--;
        }
        break;
    case 2:
        sw = mSwitch;
        if (sw == 0 || getGameView_802D6A9C()->fn_802D800C(sw)) {
            gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
            if (gGameRoot2_8053AB10->field_0x24->fn_8030C2A8(&mPos) == 1) {
                if ((field_0x90 >> 27) != 0) {
                    vfunc_0x30(0xFF, true);
                }
            }
        }
        break;
    }
}

void FocusNPC_804A8EA0::init() {
    mSwitch = (field_0x90 >> 22) & 0x1F;
    mDuration = field_0x90 & 0xFFF;
    mTimer = 0;
    if (mDuration == 0) {
        mState = 2;
    } else {
        mState = 0;
    }
}
