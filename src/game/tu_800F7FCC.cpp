#include "game/GameAudio.h"
#include "game/Player.h"
#include "game/act/tu_800F7FCC.h"

StoneArrghusChildPuff_8049C678::~StoneArrghusChildPuff_8049C678() {}

void cls_800F802C::fn_800F802C() {
    if (!mActive) {
        return;
    }
    f32 step;
    s32 duration;
    switch (mType) {
    case 0:
        step = 3.0f;
        duration = 40;
        break;
    case 1:
        step = 4.0f;
        duration = 40;
        break;
    case 2:
        step = 8.0f;
        duration = 20;
        break;
    default:
        step = 3.0f;
        duration = 40;
        break;
    }
    mRadius = mTimer * step;
    mTimer++;
    if (mTimer > duration) {
        mActive = false;
    }
    for (s32 i = 0; i < 4; i++) {
        if (fn_8023F264(i) && fn_8022EA0C(i) == i && !(mHitMask & (1 << i)) &&
            getPlayerLayer_8023DE58(i) == 0) {
            // One stack vector: the player's offset (its length is TVec2's
            // TUtilf::sqrt), then the position the hit is reported at.
            JGeometry::TVec2f v;
            v.x = getPlayerPos_8023E724(i)->x - mPos.x;
            v.y = getPlayerPos_8023E724(i)->y - mPos.y;
            if (v.length() < mRadius) {
                v.x = mPos.x;
                v.y = mPos.y;
                fn_80238548(i, 0, &v, 0xB);
                mHitMask |= 1 << i;
            }
        }
    }
}

cls_8049C718::~cls_8049C718() {}
