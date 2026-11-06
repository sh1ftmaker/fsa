#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/ActorManager.h"
#include "game/ActorId.h"
#include "JSystem/JMath/JMath.h"
#include "game/act/act_fire_bar.h"
#include "game/act/tu_801BD8C0.h"

FireBarManager_804A3F90::FireBarManager_804A3F90() {
    mCount = 0;
    mFlag = 0;
    mFlagBit = 0;
    mStep = 0;
    mSpeed = 0;
    mAngle = 0;
    mTangent = 0;
}

// FIXME: 86.8%. A named local for the `!mFlag` spawn argument (kept in a
// register across the call, as the target does) gives 96.95%, but it only steers
// register allocation, so the natural `!mFlag` stays: it costs one saved
// register (four stw instead of stmw r27). Otherwise the same instructions; saved registers are numbered differently
// (target: this copy r27, loop counter r29, spawn flag r28, bit r27 in the
// second getSpeed()) and the sin/cos table base and angle take r3/r4 swapped.
void FireBarManager_804A3F90::init() {
    EnemyBase_804A4808::init();
    if (!((field_0x90 >> 17) & 1)) {
        fn_801D0E58();
    }
    mFlag = (field_0x90 >> 31) ^ 1;
    mFlagBit = (field_0x90 >> 26) & 0x1F;
    s32 speedSel = (field_0x90 >> 24) & 3;
    if (speedSel > 2) {
        speedSel = 2;
    } else if (speedSel < 0) {
        speedSel = 0;
    }
    mStep = ((field_0x90 >> 20) & 0xF) + 1;
    s32 dir = (field_0x90 >> 8) & 0xFF;
    bool forward = (field_0x90 >> 16) & 1;
    if (dir > 3) {
        dir = 3;
    } else if (dir < 0) {
        dir = 0;
    }
    s32 count = field_0x90 & 0xFF;
    if (count > 16) {
        count = 16;
    } else if (count < 1) {
        count = 1;
    }
    mCount = count;
    mAngle = fn_801F6874(dir);
    switch (speedSel) {
    case 0:
        mSpeed = 0x80;
        break;
    case 1:
        mSpeed = 0x100;
        break;
    default:
        mSpeed = 0x40;
        break;
    }
    if (!forward) {
        mSpeed = -mSpeed;
    }
    mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
    mBox2.set(mBox);
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0xb0 = 0;
    field_0x98 = 2;
    if (getSpeed() < 0) {
        mTangent = mAngle - 0x4000;
    } else {
        mTangent = mAngle + 0x4000;
    }
    f32 dx = JMASinShort(mAngle) * 16.0f;
    f32 dy = JMACosShort(mAngle) * 16.0f;
    JGeometry::TVec3f pos = mPos;
    pos.x += mStep * dx;
    pos.y += mStep * dy;
    for (s32 i = 0; i < mCount; i++) {
        mBars[i] = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_FBOB, field_0x4, &pos, !mFlag, -1, -1);
        static_cast<cls_804A4038*>(getActorManager_801F856C()->findActorBySerial_801F666C(mBars[i]))->fn_801BD8C0(&pos, mTangent);
        pos.x += dx;
        pos.y += dy;
    }
}

// FIXME: 93.6%. Same instructions; `mr r31, r3` is scheduled before the first
// loads of mFlagBit/mSpeed instead of after, the second getSpeed() inline gets
// r30/r29 for bit/speed (target r29/r30), and the sin/cos table base and angle
// take r3/r4 swapped (target angle in r3, base in r4).
void FireBarManager_804A3F90::update() {
    mAngle += getSpeed();
    if (getSpeed() < 0) {
        mTangent = mAngle - 0x4000;
    } else {
        mTangent = mAngle + 0x4000;
    }
    f32 dx = JMASinShort(mAngle) * 16.0f;
    f32 dy = JMACosShort(mAngle) * 16.0f;
    JGeometry::TVec3f pos = mPos;
    pos.x += mStep * dx;
    pos.y += mStep * dy;
    for (s32 i = 0; i < mCount; i++) {
        static_cast<cls_804A4038*>(getActorManager_801F856C()->findActorBySerial_801F666C(mBars[i]))->fn_801BD8C0(&pos, mTangent);
        pos.x += dx;
        pos.y += dy;
    }
}

FireBarManager_804A3F90::~FireBarManager_804A3F90() {}
