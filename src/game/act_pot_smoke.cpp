#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/act/act_pot_smoke.h"

cls_804A8418::~cls_804A8418() {}

// The smoke puff animation; kinds 2 to 6 draw a fixed sprite set, any other kind the
// "enemy_both2_sob.bin" sprites plus the per-player lists.
void cls_804A8418::draw() {
    SpriteParams_801F5AE0 params;
    s32 flag = mFlag != 0; // the original converts it to bool again at every call
    if (mKind == 2) {
        fn_803075AC(mSerial, field_0x4, &mPos, 0x1C2, flag != 0, 2, -1, 1, 0, 0);
    } else if (mKind == 3) {
        fn_803075AC(mSerial, field_0x4, &mPos, mFrame + 0x2D5, flag != 0, 2, -1, 1, 0, 0);
    } else if (mKind == 4) {
        fn_803075AC(mSerial, field_0x4, &mPos, mFrame + 0x2EE, flag != 0, 2, -1, 1, 0, 0);
    } else if (mKind == 5) {
        fn_803075AC(mSerial, field_0x4, &mPos, mFrame + 0x2F7, flag != 0, 2, -1, 1, 0, 0);
    } else if (mKind == 6) {
        fn_803075AC(mSerial, field_0x4, &mPos, mFrame + 0x129, flag != 0, 2, -1, 1, 0, 0);
    } else {
        s32 frame = mFrame + 0x70;
        if (field_0x4 < 8) {
            void* res = lbl_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
            lbl_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
        }
        fn_803075AC(mSerial, field_0x4, &mPos, mFrame + 0x70, flag != 0, 2, -1, 1, 0, 0);
    }
}

void cls_804A8418::update() {
    mTimer--;
    if (mTimer != 0) {
        return;
    }
    if (mKind == 2) {
        mAlive = false;
        return;
    }
    mTimer = 2;
    mFrame++;
    if (mKind == 4) {
        mTimer = 4;
        if (mFrame >= 7) {
            mAlive = false;
        }
        return;
    }
    if (mFrame >= 4 && mKind == 6) {
        mAlive = false;
        return;
    }
    if (mFrame < 8) {
        return;
    }
    if (mKind == 1) {
        mCount++;
        if (mCount >= 8) {
            mAlive = false;
        } else {
            mFrame = 0;
        }
    } else {
        mAlive = false;
    }
}

void cls_804A8418::init() {
    mKind = field_0x90 & 0x3F;
    mFlag = (field_0x90 >> 7) & 0x1F;
    mFrame = 0;
    if (mKind == 2) {
        mTimer = 10;
    } else if (mKind == 4) {
        mTimer = 4;
    } else {
        mTimer = 2;
    }
    mCount = 0;
}
