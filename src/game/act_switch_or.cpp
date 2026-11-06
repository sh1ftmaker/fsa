#include "game/GameAudio.h"
#include "game/act/act_switch_or.h"
#include "game/GameView.h"


// Four 5-bit switch ids packed into field_0x90 above the count (bits 7-26).
static const u32 lbl_8046CB40[4] = {0x07C00000, 0x003E0000, 0x0001F000, 0x00000F80};
static const s32 lbl_8046CB50[4] = {22, 17, 12, 7};

void SwitchOR_804A8800::init() {
    mState = 0;
    mCount = field_0x90 & 0x1F;
}

void SwitchOR_804A8800::update() {
    switch (mState) {
    case 0: {
        for (s32 i = 0; i < mCount; i++) {
            if (getGameView_802D6A9C()->fn_802D800C((field_0x90 & lbl_8046CB40[i]) >> lbl_8046CB50[i])) {
                vfunc_0x30(0xFF, true);
                mState = 1;
            }
        }
        break;
    }
    case 1:
    default: {
        s32 on = 0;
        for (s32 i = 0; i < mCount; i++) {
            if (getGameView_802D6A9C()->fn_802D800C((field_0x90 & lbl_8046CB40[i]) >> lbl_8046CB50[i])) {
                on++;
            }
        }
        if (on == 0) {
            vfunc_0x30(0xFF, false);
            mState = 0;
        }
        break;
    }
    }
}

void SwitchOR_804A8800::draw() {}

SwitchOR_804A8800::~SwitchOR_804A8800() {}
