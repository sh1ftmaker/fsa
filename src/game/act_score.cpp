#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/cls_804B1E1C.h"
#include "JSystem/JMath/JMath.h"
#include "game/act/act_score.h"

// FIXME(shotgun): 98.0 %, different
void Score_804AF4C0::init() {
    mValue = field_0x90 & 0xFFFF;
    mPlayer = (field_0x90 >> 16) & 0xFF;
    mState = field_0x90 >> 24;
    field_0x248 = mPos.x - getPlayerPos_8023E724(mPlayer)->x;
    field_0x24c = mPos.y - getPlayerPos_8023E724(mPlayer)->y - 24.0f;
    field_0x240 = mValue;
    field_0x244 = lbl_80529DEC.field_0x1f4[mPlayer];
    if (mState == 0 || mState == 4) {
        if (gGameRoot_8053AAF8->field_0x7e == 0 || fn_802DE814()) {
            mAlive = false;
        }
        if (mValue == 0) {
            mAlive = false;
        } else if (mValue >= 100) {
            mValue = 100;
        }
        if (mState == 0) {
            field_0x3c.x = 0.0f;
            field_0x3c.y = -0.2f;
            field_0x3c.z = 0.0f;
        } else {
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.z = 0.0f;
        }
        mTimer = 90;
        lbl_80529DEC.field_0x1f4[mPlayer] = field_0x240;
    } else if (mState == 1) {
        field_0x3c.x = 0.0f;
        field_0x3c.y = -1.0f;
        field_0x3c.z = 0.0f;
        mTimer = 90;
        fn_801F0D20(0x41);
    } else if (mState == 2) {
        if (mValue >= 12) {
            mAlive = false;
        }
        field_0x3c.x = 0.0f;
        field_0x3c.y = -0.2f;
        field_0x3c.z = 0.0f;
        mTimer = 90;
    } else if (mState == 3) {
        mTimer = 0x257;
        f32 z = getPlayerPos_8023E724(mPlayer)->z;
        f32 y = getPlayerPos_8023E724(mPlayer)->y - 32.0f;
        mPos.x = getPlayerPos_8023E724(mPlayer)->x;
        mPos.y = y;
        mPos.z = z;
    } else if (mState == 5) {
        mTimer = 0x1a3;
        f32 z = getPlayerPos_8023E724(mPlayer)->z;
        f32 y = getPlayerPos_8023E724(mPlayer)->y - 32.0f;
        mPos.x = getPlayerPos_8023E724(mPlayer)->x;
        mPos.y = y;
        mPos.z = z;
    } else if (mState == 6) {
        mTimer = 0x12b;
        f32 z = getPlayerPos_8023E724(mPlayer)->z;
        f32 y = getPlayerPos_8023E724(mPlayer)->y - 32.0f;
        mPos.x = getPlayerPos_8023E724(mPlayer)->x;
        mPos.y = y;
        mPos.z = z;
    } else if (mState == 7) {
        mTimer = 0xb3;
        f32 z = getPlayerPos_8023E724(mPlayer)->z;
        f32 y = getPlayerPos_8023E724(mPlayer)->y - 32.0f;
        mPos.x = getPlayerPos_8023E724(mPlayer)->x;
        mPos.y = y;
        mPos.z = z;
    }
}

// FIXME(shotgun): 99.6 %, regalloc-only
void Score_804AF4C0::update() {
    if (mState == 0 || mState == 4) {
        fn_80393514();
    } else if (mState == 1) {
        if (mTimer >= 0x50) {
            s32 d = 90 - mTimer;
            field_0x3c.y = -3.0f * (1.0f - (f32)(d * d / 100));
            mPos.add(field_0x3c);
        }
        if (mTimer-- == 0) {
            mAlive = false;
        }
    } else if (mState == 2) {
        field_0x3c.x = 0.2f * JMath::sincosTable_.sinShort((mTimer % 60) << 10);
        mPos.add(field_0x3c);
        if (mTimer-- == 0) {
            mAlive = false;
        }
    } else if (mState == 3 || mState == 5 || mState == 6 || mState == 7) {
        f32 z = getPlayerPos_8023E724(mPlayer)->z;
        f32 y = getPlayerPos_8023E724(mPlayer)->y - 32.0f;
        mPos.x = getPlayerPos_8023E724(mPlayer)->x;
        mPos.y = y;
        mPos.z = z;
        if (!fn_80230188(mPlayer) && fn_8023BC38(mPlayer) <= 0) {
            mAlive = false;
        }
    }
}

Score_804AF4C0::~Score_804AF4C0() {}

// FIXME(shotgun): 99.7 %, reloc-name
void Score_804AF4C0::draw() {
    if (mState == 0 || mState == 4) {
        fn_80393228();
    } else if (mState == 1) {
    } else if (mState == 2) {
        fn_80391F58();
    } else if (mState == 3 || mState == 5 || mState == 6 || mState == 7) {
        if (!fn_80230188(mPlayer)) {
            fn_80391D44();
        }
    }
}

// Per-player digit colours.
static const u8 sDigitRed[4] = {0x28, 0xFF, 0x32, 0xBE};
static const u8 sDigitGreen[4] = {0xFF, 0x50, 0xC8, 0x78};
static const u8 sDigitBlue[4] = {0x28, 0x50, 0xFF, 0xFF};

// Not auto-inlined: draw calls fn_80391D44 out of line in the original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// FIXME(shotgun): 63.6 %, missing/extra (the original builds a TVec3f copy of mPos and passes it on)
void Score_804AF4C0::fn_80391D44() {
    s32 n = fn_8023BC38(mPlayer) / 60 + 1;
    if (n < 10) {
        Vec pos;
        pos.x = mPos.x;
        pos.y = mPos.y;
        pos.z = mPos.z;
        if (field_0x4 < 8) {
            lbl_8053AF50->fn_80455770(0.5f, n, &pos,
                                      JUtility::TColor(sDigitRed[mPlayer], sDigitGreen[mPlayer], sDigitBlue[mPlayer], 255));
        }
    } else {
        Vec pos;
        pos.x = mPos.x - 10.0f;
        pos.y = mPos.y;
        pos.z = mPos.z;
        if (field_0x4 < 8) {
            lbl_8053AF50->fn_80455770(0.5f, 1, &pos,
                                      JUtility::TColor(sDigitRed[mPlayer], sDigitGreen[mPlayer], sDigitBlue[mPlayer], 255));
        }
        pos.x = mPos.x + 10.0f;
        pos.y = mPos.y;
        pos.z = mPos.z;
        if (field_0x4 < 8) {
            lbl_8053AF50->fn_80455770(0.5f, 0, &pos,
                                      JUtility::TColor(sDigitRed[mPlayer], sDigitGreen[mPlayer], sDigitBlue[mPlayer], 255));
        }
    }
}
// FIXME(shotgun): not written (size 0x12D0): fn_80391F58
// FIXME(shotgun): not written (size 0x2EC): fn_80393228
// FIXME(shotgun): not written (size 0x438): fn_80393514
#pragma auto_inline reset
