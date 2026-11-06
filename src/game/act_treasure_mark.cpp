#include "game/GameAudio.h"
#include "game/act/act_treasure_mark.h"
#include "game/GameGlobals.h"

void TreasureMark_804ABAE8::init() {
    mLife = 10000;
    mFrame = 0;
    mBlinkTimer = 10;
    if ((field_0x90 & 0xF) == 2) {
        mFrame = 1;
    }
}

void TreasureMark_804ABAE8::update() {
    mBlinkTimer--;
    if (mBlinkTimer <= 0) {
        if ((field_0x90 & 0xF) == 0) {
            mFrame++;
            mFrame &= 1;
        }
        mBlinkTimer = 10;
    }
}

void TreasureMark_804ABAE8::draw() {
    drawSprite_801F06F0("enemy_both2_sob.bin", mFrame + 431, NULL, -1, 1.0f, 1.0f);
    fn_803075AC(mSerial, field_0x4, &mPos, mFrame + 431, false, 2, -1, 0, 0, 0);
}

TreasureMark_804ABAE8::~TreasureMark_804ABAE8() {}
