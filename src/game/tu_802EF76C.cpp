#include "game/GameAudio.h"
#include "game/tu_802EF76C.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/tu_80182914.h"

// Steps colour channel i toward its end value; it lands on the end when the
// step would pass it.
static inline void stepChannel(cls_804AA4F0* actor, s32 i) {
    if (actor->mColorCur[i] > actor->mColorEnd[i]) {
        actor->mColorCur[i] += actor->mColorStep[i];
    } else {
        actor->mColorCur[i] = actor->mColorEnd[i];
    }
}

// Passes the frame counter (u8) to fn_80128C58 and the packed colour word to fn_8013A884.
static inline void applyColor(cls_804AA4F0* actor) {
    fn_80128C58((u8)actor->mCounter);
    u32 packed = (actor->mColorCur[0] << 24) | (actor->mColorCur[1] << 16) | (actor->mColorCur[2] << 8) |
                 actor->mColorCur[3];
    fn_8013A884(&packed);
}

cls_804AA4F0::~cls_804AA4F0() {}

void cls_804AA4F0::draw() {}

// FIXME: 99.95 %. The only difference left is the switch's jump table: the
// original's relocation names it jumptable_804AA548 (dtk's split of .data),
// ours is MWCC's anonymous @NNN table, which the honest report compares by name.
void cls_804AA4F0::init() {
    if (!fn_802DEBA0()) {
        field_0x244 = field_0x90 & 0xFF;
    } else {
        field_0x244 = -1;
    }
    s32 index = (field_0x90 >> 22) & 0x1F;
    field_0x240 = index;
    mState = 0;
    mColor.mColor = fn_8018330C();
    mColorCur[0] = (u32)mColor.mWord >> 24;
    mColorCur[1] = ((u32)mColor.mWord >> 16) & 0xFF;
    mColorCur[2] = ((u32)mColor.mWord >> 8) & 0xFF;
    mColorCur[3] = (u32)mColor.mWord & 0xFF;
    mCounter = fn_801B7550();
    Camera_804AA9E4* camera = lbl_8053AB10->field_0x24;
    field_0x234 = -camera->field_0x4[0][3];
    field_0x238 = -camera->field_0x4[1][3];
    field_0x23c = 0.0f;
    switch (field_0x244) {
    case 0:
        vfunc_0x54(0xD2, 0xC8, 0xAA, 0, 0xA, 4);
        break;
    case 1:
        vfunc_0x54(0x96, 0xC8, 0xFF, 0, 0xA, 4);
        break;
    case 2:
        vfunc_0x54(0xFF, 0xC8, 0xA0, 0, 0x82, 0);
        break;
    case 3:
        vfunc_0x54(0xFF, 0xC8, 0xA0, 0, 0xA, 4);
        break;
    case 4:
        vfunc_0x54(0xFF, 0xC8, 0xA0, 0, 0xC8, 4);
        break;
    case 5:
        vfunc_0x54(0xFF, 0xC8, 0xA0, 0, 0xC8, 0);
        break;
    case 6:
        vfunc_0x54(0xC8, 0xD2, 0xFF, 0, 0xA, 4);
        break;
    case 7:
        vfunc_0x54(0xC8, 0xD2, 0xFF, 0, 0xC8, 4);
        break;
    case 8:
        vfunc_0x54(0xC8, 0xD2, 0xFF, 0, 0xC8, 0);
        break;
    case 9:
        vfunc_0x54(0x96, 0xC8, 0xFF, 0, 0xA, 4);
        break;
    case 10:
        vfunc_0x54(0x96, 0xC8, 0xFF, 0, 0x82, 4);
        break;
    case 11:
        vfunc_0x54(0x96, 0xC8, 0xFF, 0, 0x82, 0);
        break;
    case 12:
        vfunc_0x54(0xD2, 0xFF, 0xFF, 0x19, 0xDC, 4);
        break;
    case 13:
        vfunc_0x54(0xFF, 0x80, 0xFF, 0, 0xA, 4);
        break;
    case 14:
        vfunc_0x54(0xFF, 0x80, 0xFF, 0, 0xC8, 4);
        break;
    case 15:
        vfunc_0x54(0xFF, 0x80, 0xFF, 0, 0x82, 0);
        mState = 4;
        break;
    case 16:
        vfunc_0x54(0xFF, 0xC8, 0xA0, 0, 0xA, 4);
        mDuration = 0xC8;
        mState = 5;
        break;
    case 17:
        vfunc_0x54(0xFF, 0xFF, 0xFF, 0, 0x80, 4);
        break;
    default:
        mState = 4;
        break;
    }
    applyColor(this);
    JGeometry::TVec3f savedPos = mPos;
    update();
    mPos = savedPos;
    field_0x11f = 1;
}

void cls_804AA4F0::update() {
    switch (mState) {
    case 0:
        mCounter -= 4;
        stepChannel(this, 0);
        stepChannel(this, 1);
        stepChannel(this, 2);
        if (mCounter <= mDuration) {
            mCounter = mDuration;
            mColorCur[0] = mColorEnd[0];
            mColorCur[1] = mColorEnd[1];
            mColorCur[2] = mColorEnd[2];
            mState = 4;
        }
        applyColor(this);
        break;
    case 1:
        mCounter += 4;
        stepChannel(this, 0);
        stepChannel(this, 1);
        stepChannel(this, 2);
        if (mCounter >= mDuration) {
            mCounter = mDuration;
            mColorCur[0] = mColorEnd[0];
            mColorCur[1] = mColorEnd[1];
            mColorCur[2] = mColorEnd[2];
            mState = 4;
        }
        applyColor(this);
        break;
    case 2:
        mCounter += 4;
        if (mCounter >= mDuration) {
            mCounter = mDuration;
            mState = 4;
            if (field_0x244 == 0x10 && !fn_802D6A9C()->fn_802D800C(field_0x240)) {
                lbl_8053AF80->fn_80458FF0(0x1000021, false, false);
            }
        }
        applyColor(this);
        break;
    case 3:
    default:
        applyColor(this);
        break;
    case 5:
        if ((field_0x90 >> 27) == 0 || vfunc_0x34()) {
            mState = 2;
        }
        applyColor(this);
        break;
    }
}

void cls_804AA4F0::vfunc_0x54(s32 r, s32 g, s32 b, s32 a, s32 frames, s32 state) {
    mState = state;
    if (state == 0) {
        mColorEnd[0] = r;
        mColorEnd[1] = g;
        mColorEnd[2] = b;
        mColorCur[3] = a;
        mDuration = frames;
        mColorStep[0] = (mColorEnd[0] - mColorCur[0]) / 4;
        mColorStep[1] = (mColorEnd[1] - mColorCur[1]) / 4;
        mColorStep[2] = (mColorEnd[2] - mColorCur[2]) / 4;
        if (mCounter < mDuration) {
            mState = 1;
        }
    } else {
        mColorCur[0] = r;
        mColorCur[1] = g;
        mColorCur[2] = b;
        mColorCur[3] = a;
        mCounter = frames;
    }
}
