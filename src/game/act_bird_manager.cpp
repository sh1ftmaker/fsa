#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/act_bird_manager.h"
#include "game/act/tu_8020B354.h"
#include "game/ActorManager.h"
#include "game/GameView.h"
#include "JSystem/JMath/JMath.h"

// Moves mPos 1.2 along x toward mTarget (forward) or mStart; returns whether it arrived.
static inline bool stepLine(cls_804A5C88::Path* path, bool forward) {
    bool done = false;
    if (forward) {
        path->mPos.x += 1.2f;
        if (path->mPos.x >= path->mTarget.x) {
            path->mPos.x = path->mTarget.x;
            done = true;
        }
    } else {
        path->mPos.x -= 1.2f;
        if (path->mPos.x <= path->mStart.x) {
            path->mPos.x = path->mStart.x;
            done = true;
        }
    }
    return done;
}

// Advances mAngle by mAngleStep and places mPos on the radius-125 arc around mTarget
// (atTarget) or mStart; returns whether the angle reached the end of the arc.
static inline bool stepArc(cls_804A5C88::Path* path, bool atTarget) {
    bool done = false;
    path->mAngle += path->mAngleStep;
    if (atTarget) {
        if (path->mAngle <= 0) {
            if (path->mAngleStep > 0) {
                path->mAngle = -0x8000;
            } else {
                path->mAngle = 0;
            }
            done = true;
        }
    } else {
        if (path->mAngle >= 0) {
            if (path->mAngleStep > 0) {
                path->mAngle = 0;
            } else {
                path->mAngle = -0x8000;
            }
            done = true;
        }
    }
    f32 sinVal = JMath::sincosTable_.sinShort(path->mAngle) * 125.0f;
    f32 cosVal = JMath::sincosTable_.cosShort(path->mAngle) * 125.0f;
    if (atTarget) {
        path->mPos.x = path->mTarget.x;
        path->mPos.y = path->mTarget.y;
    } else {
        path->mPos.x = path->mStart.x;
        path->mPos.y = path->mStart.y;
    }
    path->mPos.x += sinVal;
    path->mPos.y += cosVal;
    return done;
}

// FIXME: 92.20% (missing/extra). The target tests mDirection once into a bool (cntlzw + extrwi.) and
// branches on it; ours compares with cmpwi at each use, so 9 instructions are missing (227 vs 236).
void cls_804A5C88::Path::fn_80209640(JGeometry::TVec2f* delta) {
    f32 oldX = mPos.x;
    f32 oldY = mPos.y;

    switch (mState) {
    case 0:
        if (stepLine(this, mDirection == 0)) {
            mState = 1;
        }
        break;
    case 1:
        if (stepArc(this, mDirection == 0)) {
            mState = 2;
        }
        break;
    case 2:
        if (stepLine(this, mDirection != 0)) {
            mState = 3;
        }
        break;
    case 3:
        if (stepArc(this, mDirection != 0)) {
            mState = 4;
        }
        break;
    case 4:
        if (mDirection == 0) {
            mPos.x += 1.2f;
        } else {
            mPos.x -= 1.2f;
        }
        break;
    }

    if (delta != NULL) {
        delta->set(mPos);
        delta->x -= oldX;
        delta->y -= oldY;
    }
}

cls_804A5C88::cls_804A5C88() {
    field_0x340 = 0;
    field_0x344 = 0;
    field_0x348 = 0;
    mPath.mState = 0;
    mPath.mPos.x = 0.0f;
    mPath.mPos.y = 0.0f;
    mPath.mDirection = 0;
    mPath.mAngle = -0x8000;
    mPath.mAngleStep = 0;
    mPath.mStart.x = 0.0f;
    mPath.mStart.y = 0.0f;
    mPath.mTarget.x = 0.0f;
    mPath.mTarget.y = 0.0f;
}

// FIXME: 99.47% (regalloc-only). Minor register differences: dir in r30 vs r29, centerX/centerY in f3/f2 vs f2/f4, and loop counters r28/r29 swapped.
void cls_804A5C88::init() {
    EnemyBase_804A4808::init();
    field_0x2ac = field_0x90 & 3;
    if (field_0x2ac >= 2 || field_0x2ac < 0) {
        field_0x2ac = fn_801CD664(2);
    }
    field_0x340 = (field_0x90 >> 4) & 3;
    if (field_0x340 >= 2 || field_0x340 < 0) {
        field_0x340 = fn_801CD664(2);
    }
    field_0x344 = (field_0x90 >> 8) & 1;
    field_0x348 = (u32)field_0x90 >> 27;

    mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
    mBox2.set(mBox);
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0xb0 = 0;
    field_0x98 = 2;
    mFlags |= ENEMY_FLAG_NO_ATTACK | ENEMY_FLAG_NO_HURT | ENEMY_FLAG_KEEP_LIFE;
    mLife = 1000;
    mLifeBackup = mLife;
    field_0x194 = 1;
    mState = 0;
    mStateTimer = 0;
    field_0x250 = fn_801CD664(0x4B1) + 600;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;

    s32 dir = (field_0x340 != 0);
    GameView_804A9950* view = getGameView_802D6A9C();
    f32 centerY = (view->mVisibleBox.i.y + view->mVisibleBox.f.y) * 0.5f;
    f32 centerX = (view->mVisibleBox.i.x + view->mVisibleBox.f.x) * 0.5f;
    mPath.mState = 0;
    switch (dir) {
    case 0:
        mPath.mPos.x = view->mVisibleBox.i.x - 48.0f;
        break;
    case 1:
        mPath.mPos.x = 48.0f + view->mVisibleBox.f.x;
        break;
    }
    mPath.mPos.y = centerY - 125.0f;
    mPath.mDirection = dir;
    mPath.mAngle = -0x8000;
    mPath.mAngleStep = 100;
    if (dir == 0) {
        mPath.mAngleStep = -mPath.mAngleStep;
    }
    mPath.mStart.x = centerX - 100.0f;
    mPath.mStart.y = centerY;
    mPath.mTarget.x = centerX + 100.0f;
    mPath.mTarget.y = centerY;

    JGeometry::TVec2f spawnPos2D;
    mPath.getPos(&spawnPos2D);
    mSpawnPos.x = spawnPos2D.x;
    if (field_0x344 == 0) {
        mSpawnPos.y = spawnPos2D.y;
    }
    mSpawnPos.z = 24.0f;

    f32 stepX = 0.0f;
    f32 stepY = 0.0f;
    JGeometry::TVec3f spawnPos = mSpawnPos;
    switch (field_0x2ac) {
    case 0:
        stepX = 16.0f;
        spawnPos.x -= 24.0f;
        break;
    case 1:
        stepY = 16.0f;
        spawnPos.y -= 24.0f;
        break;
    }

    for (s32 i = 0; i < 4; i++) {
        mSlots[i].mSerial = getActorManager_801F856C()->spawnActor_801F9484(
            'FOBR', field_0x4, &spawnPos, mSerial, -1, -1);
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(mSlots[i].mSerial);
        if (actor != NULL) {
            actor->field_0x94 = i;
        }
        spawnPos.x += stepX;
        spawnPos.y += stepY;
    }
}

void cls_804A5C88::vfunc_0x54() {
    mPos.x = mSpawnPos.x;
    mPos.y = mSpawnPos.y;
    mPos.z = mSpawnPos.z;
}

void cls_804A5C88::vfunc_0xA4() {
    if (mState != 2) {
        mStateTimer = 0;
        mState = 2;
    }
    for (s32 i = 0; i < 4; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(mSlots[i].mSerial);
        if (actor != NULL) {
            static_cast<cls_804A5E98*>(actor)->fn_8020B434();
        }
    }
}

void cls_804A5C88::vfunc_0xA8() {
    if (mState != 3) {
        mStateTimer = 0;
        mState = 3;
    }
    for (s32 i = 0; i < 4; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(mSlots[i].mSerial);
        if (actor != NULL) {
            static_cast<cls_804A5E98*>(actor)->fn_8020B3E4(
                mSlots[i].mPlayer, mSlots[i].field_0xc, &mSlots[i].mTarget, mSlots[i].field_0x1c);
        }
    }
}

// FIXME: 97.74% (missing/extra). In the mAlive-clearing loop of state 1 the slot pointer and the loop counter
// trade registers (r29/r30), which costs ours one extra mr (85 vs 84 instructions); the rest matches.
void cls_804A5C88::vfunc_0x90(u8 mapHit) {
    fn_80207A60();
    switch (mState) {
    case 0:
        if ((field_0x348 == 0 && mStateTimer >= field_0x250) || vfunc_0x34()) {
            for (s32 i = 0; i < 4; i++) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(mSlots[i].mSerial);
                if (actor != NULL) {
                    static_cast<cls_804A5E98*>(actor)->fn_8020B500();
                }
            }
            mStateTimer = 0;
            mState = 1;
        }
        break;
    case 1: {
        s32 layer = field_0x4;
        GameView_804A9950* view = getGameView_802D6A9C();
        const JGeometry::TBox2f* box;
        if (layer < 8) {
            box = &view->mVisibleBox;
        } else {
            box = &view->field_0xdc;
        }
        if (mPos.x < box->i.x - 96.0f || mPos.x > box->f.x + 96.0f) {
            mAlive = false;
            for (s32 i = 0; i < 4; i++) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(mSlots[i].mSerial);
                if (actor != NULL) {
                    actor->mAlive = false;
                }
            }
        }
        break;
    }
    case 2:
    case 3:
        break;
    }
}

// FIXME: 99.43% (regalloc-only). Loop register allocation: loop counter i and slot pointer registers r30 and r31 are swapped.
void cls_804A5C88::vfunc_0x88() {
    switch (mState) {
    case 0:
    case 2:
    case 3:
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        break;
    case 1:
        if (field_0x344 == 0) {
            JGeometry::TVec2f delta;
            mPath.fn_80209640(&delta);
            field_0x3c.set(delta.x, delta.y, 0.0f);
        } else if (field_0x340 == 0) {
            field_0x3c.set(1.2f, 0.0f, 0.0f);
        } else {
            field_0x3c.set(-1.2f, 0.0f, 0.0f);
        }
        for (s32 i = 0; i < 4; i++) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(mSlots[i].mSerial);
            if (actor != NULL) {
                actor->field_0x3c = field_0x3c;
            }
        }
        break;
    }
}

cls_804A5C88::~cls_804A5C88() {}
