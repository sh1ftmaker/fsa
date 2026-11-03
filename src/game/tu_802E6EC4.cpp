#include "game/GameAudio.h"
#include "game/act/tu_802E6EC4.h"
#include "game/GameGlobals.h"

cls_804A9E50::~cls_804A9E50() {}

void cls_804A9E50::draw() {
    s32 flag = 0;
    if (mFlag != 0) {
        flag = 1;
    }
    fn_803075AC(mSerial, field_0x4, &mPos, mStep + 0x26D, flag, 2, -1, 1, 0, 0);
}

void cls_804A9E50::update() {
    mTimer--;
    if (mTimer != 0) {
        return;
    }
    mTimer = 2;
    if (mMode == 0) {
        mStep++;
        mCount++;
        if (mStep > 6) {
            mStep = 5;
        }
        if (mCount == 8) {
            mAlive = false;
        }
    } else if (mStep == 0) {
        mStep = 2;
    } else if (mStep == 2) {
        mStep = 6;
    } else {
        mAlive = false;
    }
}

void cls_804A9E50::init() {
    mMode = field_0x90 & 0x7F;
    mFlag = (field_0x90 >> 7) & 0x1F;
    mStep = 0;
    mCount = 0;
    if (mMode == 0) {
        mTimer = 1;
    } else {
        mTimer = 2;
    }
}
