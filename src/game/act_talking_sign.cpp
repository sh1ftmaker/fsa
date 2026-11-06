#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/act/act_talking_sign.h"
#include "game/ViewLayer.h"

void TalkingSign_804AF428::init() {
    mBox.set(0.0f, 0.0f, 16.0f, 16.0f);
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    mState = 0;
    mPlayer = -1;
    if (gGameRoot_8053AAF8->field_0x7e == 1) {
        if (field_0x90 & 0x80000000) {
            mAlive = false;
        } else if (fn_80138AB0() == true && (field_0x90 & 0x20000000)) {
            mAlive = false;
        } else if (field_0x90 & 0x10000000) {
            mAlive = false;
        }
    } else {
        if (field_0x90 & 0x40000000) {
            mAlive = false;
        } else if (fn_80138AB0() == true && (field_0x90 & 0x20000000)) {
            mAlive = false;
        } else if (field_0x90 & 0x10000000) {
            mAlive = false;
        }
    }
}

// FIXME: 90.26%. Both fn_8038C678 calls: the original loads mSerial and field_0x4 into r29/r28
// before the getPlayerPos_8023E724 call (callee-saved across it); ours reloads them into r5/r6 after it.
// The view helper's layer then lands in r30/r29 instead of r28/r30. Also the area box's
// f.x store (0x28) is scheduled after the mPos.y load in the original.
void TalkingSign_804AF428::update() {
    s32 tile = fn_80226850(&mPos, field_0x4);
    if (tile == 0x1C7) {
        mBox.set(0.0f, 0.0f, 32.0f, 32.0f);
    }
    JGeometry::TBox2f area(2.0f + mBox.i.x, mBox.i.y, mBox.f.x - 2.0f, mBox.f.y - 2.0f);
    area.i.x += mPos.x;
    area.f.x += mPos.x;
    area.i.y += mPos.y;
    area.f.y += mPos.y;
    switch (mState) {
    case 0:
        if (tile == 0x71 || tile == 0x3C || tile == 0x1C7) {
            if (viewLayerFlag(field_0x4)) {
                fn_8022FEB8(&area, field_0x4, mSerial, true, false);
            } else {
                fn_8022FEB8(&area, field_0x4, mSerial, true, true);
            }
            mPlayer = fn_8022FD8C(mSerial);
            u32 flags = 0x400;
            if (tile == 0x71) {
                flags = 0x200;
            }
            if (viewLayerFlag(field_0x4)) {
                if (mPlayer >= 0) {
                    if (fn_8038C678(mPlayer, (field_0x90 & 0xFFFF) + 0x12B, mSerial, field_0x4,
                                    *getPlayerPos_8023E724(mPlayer), flags)) {
                        mState = 1;
                    }
                }
            } else if (mPlayer >= 0) {
                if (fn_80230CFC(mPlayer) == 3 && mPos.y <= getPlayerPos_8023E724(mPlayer)->y) {
                    if (fn_8038C678(mPlayer, (field_0x90 & 0xFFFF) + 0x12B, mSerial, field_0x4,
                                    *getPlayerPos_8023E724(mPlayer), flags)) {
                        mState = 1;
                    }
                }
            }
        } else {
            mAlive = false;
        }
        break;
    case 1:
        fn_8022FEB8(&area, field_0x4, mSerial, true, false);
        if (fn_80388CAC(mPlayer, mSerial)) {
            mState = 0;
        }
        break;
    }
}

void TalkingSign_804AF428::draw() {}

TalkingSign_804AF428::~TalkingSign_804AF428() {}
