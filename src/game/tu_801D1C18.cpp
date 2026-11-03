// SpriteAnim_801D267C (game/SpriteAnim.h): a sprite-cel animation player
// built on J3DFrameCtrl.

#include "game/GameAudio.h"
#include "game/SpriteAnim.h"
#include "game/GameGlobals.h"

template <typename T>
static inline T clamp(T value, T min, T max) {
    if (value > max) {
        value = max;
    } else if (value < min) {
        value = min;
    }
    return value;
}

// FIXME: calcCel's loop costs fn_801D1C18 (98.3 %), fn_801D1D60 (98.7 %),
// fn_801D1F14 (98.5 %) and fn_801D22DC (98.7 %) the same difference: the
// target keeps time/key/table/p/cel in r6/r5/r9/r7/r8 (ours r8/r7/r6/r5/r9)
// and stores the int-to-float temporary's low word before its high word.
// A while loop over `cel = p[0]` and a separate `p` declaration did the same.
inline void SpriteAnim_801D267C::calcCel() {
    s32 time = 0;
    s32 key = 0;
    const s16* table = mTables[mAnim];
    f32 frame = mFrameCtrl.getFrame();
    for (const s16* p = table; p[0] >= 0; p += 2, key++) {
        s32 start = time;
        time += p[1];
        if (start <= frame && frame < time) {
            mCel = clamp<s32>(p[0], mMinCel, mMaxCel);
            mKey = key;
            return;
        }
    }
    mCel = clamp<s32>(table[0], mMinCel, mMaxCel);
    mKey = 0;
}

inline void SpriteAnim_801D267C::setFrameInline(f32 frame) {
    frame = clamp<f32>(frame, 0.0f, mLength);
    if (frame == mLength) {
        if (mFrameCtrl.getAttribute() == J3DFrameCtrl::EMode_LOOP) {
            frame = 0.0f;
        } else {
            frame -= 0.001f;
        }
    }
    mFrameCtrl.setFrame(frame);
    calcCel();
}

inline bool SpriteAnim_801D267C::isLoopEnd() {
    bool result = false;
    if (mFrameCtrl.getAttribute() == J3DFrameCtrl::EMode_LOOP) {
        if (mFrameCtrl.checkPass(mLength - 0.001f)) {
            result = true;
        }
    }
    return result;
}

inline bool SpriteAnim_801D267C::isStopped() {
    bool result = false;
    if (mFrameCtrl.checkState(J3DFrameCtrl::STATE_STOP_E) ||
        (mFrameCtrl.getAttribute() == J3DFrameCtrl::EMode_NONE && 0.0f == mFrameCtrl.getRate() &&
         mFrameCtrl.getFrame() > mLength - 0.002f))
    {
        result = true;
    }
    return result;
}

void SpriteAnim_801D267C::fn_801D1C18() {
    if (mAnim >= 0 && mAnim < mTableCount) {
        if (mHold) {
            mHold = 0;
        } else {
            mFrameCtrl.update();
            calcCel();
        }
    }
}

void SpriteAnim_801D267C::fn_801D1D60() {
    setFrameInline(fn_801CD664(mLength + 1));
}

void SpriteAnim_801D267C::fn_801D1F14(f32 frame) {
    setFrameInline(frame);
}

// FIXME: 94.4 %. The target tests the first attribute (== 0) through a
// bool (cntlzw; extrwi.) and compares it again inside the inlined
// isStopped with cmplwi; ours compares with cmplwi first and clrlwi. inside.
// `!getAttribute()` gave the same code.
bool SpriteAnim_801D267C::fn_801D208C() {
    bool result = false;
    if (mFrameCtrl.getAttribute() == J3DFrameCtrl::EMode_NONE) {
        result = isStopped();
    } else if (mFrameCtrl.getAttribute() == J3DFrameCtrl::EMode_LOOP) {
        result = isLoopEnd();
    }
    return result;
}

bool SpriteAnim_801D267C::fn_801D217C() {
    return isLoopEnd();
}

// FIXME: 86.2 %. Only the order of the first instructions differs: the
// target loads mState (lbz 0x29) and sets result = 0 before the stack frame
// is made; ours does it after. Splitting the || into two ifs was worse.
bool SpriteAnim_801D267C::fn_801D21F0() {
    return isStopped();
}

bool SpriteAnim_801D267C::fn_801D2264(s32 anim, s32 mode) {
    bool result = false;
    if (mAnim != anim) {
        fn_801D22DC(anim, mode, true);
        result = true;
    }
    return result;
}

bool SpriteAnim_801D267C::fn_801D22A0(s32 anim, s32 mode) {
    bool result = false;
    if (mAnim != anim) {
        fn_801D22DC(anim, mode, false);
        result = true;
    }
    return result;
}

void SpriteAnim_801D267C::fn_801D22DC(s32 anim, s32 mode, bool keepFrame) {
    mAnim = anim;
    const s16* p = mTables[mAnim];
    mLength = 0;
    while (p[0] >= 0) {
        mLength += p[1];
        p += 2;
    }
    if (mode != J3DFrameCtrl::EMode_LOOP) {
        mLength--;
    }
    f32 frame = mFrameCtrl.getFrame();
    mFrameCtrl.init(mLength);
    mFrameCtrl.setAttribute(mode);
    if (keepFrame) {
        setFrameInline(frame);
        mHold = 0;
    } else {
        calcCel();
        mHold = 1;
    }
}

// FIXME: 91.7 %. The target keeps the running min/max in the register it
// stores (blt over `mr r0, r4`), and loads the key's cel before mMaxCel;
// ours branches the other way. min/max templates gave the same code.
void SpriteAnim_801D267C::fn_801D2608(const s16** tables, s32 count) {
    mTables = tables;
    mTableCount = count;
    for (s32 i = 0; i < mTableCount; i++) {
        for (const s16* p = mTables[i]; p[0] >= 0; p += 2) {
            mMinCel = mMinCel < p[0] ? mMinCel : p[0];
            mMaxCel = mMaxCel < p[0] ? p[0] : mMaxCel;
        }
    }
}

SpriteAnim_801D267C::SpriteAnim_801D267C()
    : mTables(NULL), mTableCount(0), mAnim(-1), mLength(0), mCel(0), mMinCel(0x7FFFFFFF),
      mMaxCel(0x80000000), mKey(0), mHold(0) {}
