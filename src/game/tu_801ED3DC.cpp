#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/tu_801ED3DC.h"
#include "game/tu_804455E0.h"
#include "JSystem/JUtility/JUTVideo.h"


// Derives the displayed seconds and hundredths from the frame count.
static inline void updateDisplay(cls_801ED3DC& timer) {
    if (!timer.mStopped) {
        u8 seconds = timer.mFrames / 60;
        timer.mSeconds = seconds > 119 ? 119 : seconds;
        f32 centis = (1.0f / 60.0f) * (timer.mFrames % 60 * 100);
        s32 rounded = centis + (centis > 0.0f ? 0.5f : -0.5f);
        timer.mCentis = (u8)rounded > 99 ? 99 : rounded;
    }
}

void cls_801ED828::fn_801ED974() {
    mFrames = 0;
}

void cls_801ED828::fn_801ED968() {
    mFrames = 180;
}

void cls_801ED828::fn_801ED8F0() {
    if (mFrames != 0) {
        if (mFrames % 60 == 0) {
            gAudioMgr_8053AF80->fn_80458880(0x3f, -1);
        }
        mFrames--;
    }
}

// const: the target loads mFrames above the prologue's stw r0 (79.8 -> 83.8); the
// u16 sizes convert with the signed form (xoris), an explicit (s32) (83.8 -> 90.1).
// FIXME: 90.12. Scheduling and registers only: the target loads efbHeight (lhz 6) before
// fbWidth (lhz 4), both through r3; ours keeps the RenderModeObj in r8 and loads fbWidth
// first.
void cls_801ED828::fn_801ED828() const {
    if (mFrames != 0) {
        for (int i = 0; i < 3; i++) {
            if (30 + 60 * i <= mFrames && mFrames <= (i + 1) * 60) {
                GXRenderModeObj* mode = JUTVideo::getManager()->getRenderMode();
                lbl_8053AED0->fn_8043CB24(i + 1, 255, 0, 1, false, 0.5f * (s32)mode->fbWidth,
                                          0.5f * (s32)mode->efbHeight, 3.0f);
                return;
            }
        }
    }
}

void cls_801ED3DC::fn_801ED80C() {
    mStopped = true;
    mFrames = 0;
    mSeconds = 0;
    mCentis = 0;
}

void cls_801ED3DC::fn_801ED7F0() {
    mStopped = true;
    mFrames = 0;
    mSeconds = 0;
    mCentis = 0;
}

bool cls_801ED3DC::fn_801ED7B0() const {
    bool result = false;
    if (mStopped == true && mFrames == 7200 && mSeconds == 119 && mCentis == 99) {
        result = true;
    }
    return result;
}

void cls_801ED3DC::fn_801ED798() {
    mStopped = false;
    mFrames = 0;
    mSeconds = 0;
    mCentis = 0;
}

void cls_801ED3DC::fn_801ED5C8(f32 fraction) {
    if (fraction != 1.0f) {
        mFrames--;
        bool counted = true;
        updateDisplay(*this);
        if (fraction > 1.0f) {
            fraction = 1.0f;
        } else if (fraction < 0.0f) {
            fraction = 0.0f;
        }
        f32 centis = (1.0f / 60.0f) * (100.0f * fraction);
        s32 rounded = centis + (centis > 0.0f ? 0.5f : -0.5f);
        mCentis += rounded;
        if (mCentis > 99) {
            mCentis -= 100;
            mSeconds++;
            if (mSeconds > 119) {
                mStopped = true;
                mFrames = 7200;
                mSeconds = 119;
                mCentis = 99;
                counted = false;
            }
        }
        if (counted) {
            mFrames++;
        }
    }
    mStopped = true;
}

void cls_801ED3DC::fn_801ED458(u8 jitter) {
    if (!mStopped) {
        mFrames++;
        if (mFrames >= 7200) {
            mStopped = true;
            mFrames = 7200;
            mSeconds = 119;
            mCentis = 99;
        } else if (!mStopped) {
            updateDisplay(*this);
            if (mCentis % 10 != 9) {
                mCentis += jitter % 2;
            }
        }
    }
}

// const: the original loads mSeconds and mCentis before the first store through
// lbl_8053AEF8 (MWCC hoists loads through a const `this` past stores; RULES_CARD).
void cls_801ED3DC::fn_801ED3DC(s32 slot, s32 rank) const {
    lbl_8053AEF8->field_0x44 |= (u8)(1 << slot);
    lbl_8053AEF8->field_0x3c[slot] = mSeconds;
    lbl_8053AEF8->field_0x38[slot] = mCentis;
    if (rank >= 0 && rank < 4) {
        lbl_8053AEF8->field_0x44 |= (u8)(1 << (slot + 4));
        lbl_8053AEF8->field_0x40[slot] = rank + 1;
    }
}
