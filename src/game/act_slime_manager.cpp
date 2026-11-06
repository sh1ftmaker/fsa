#include "game/ActorId.h"
#include "game/GameAudio.h"
#include "game/act/act_slime_manager.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/ActorManager.h"
#include "game/act/tu_80209C14.h"

// Steering wrapper: without it the arguments are evaluated after
// getActorManager_801F856C() (init 97.8 %, vfunc_0x90 98.0 %); with it the
// call is made on the arguments' registers as in the original.
static inline s32 spawnActorInline(u32 id, s32 layer, const JGeometry::TVec3f* pos, u32 arg, s32 a, s32 b) {
    return getActorManager_801F856C()->spawnActor_801F9484(id, layer, pos, arg, a, b);
}

inline void cls_804A5BD8::spawnSlimes() {
    fn_801F3604();
    JGeometry::TVec3f pos(mPos);
    JGeometry::TVec2f step;
    step.x = 0.0f;
    step.y = 0.0f;
    switch (field_0x2ac) {
    case 0:
        pos.x -= 24.0f;
        step.x = 16.0f;
        break;
    case 1:
        pos.y -= 24.0f;
        step.y = 16.0f;
        break;
    }
    for (s32 i = 0; i < 4; i++) {
        mSlots[i].mSerial = spawnActorInline(ACTOR_FOSL, field_0x4, field_0x340 == 3 ? &mPos : &pos, mSerial, -1, -1);
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(mSlots[i].mSerial);
        if (actor != NULL) {
            actor->field_0x94 = i;
            if (field_0x340 == 3) {
                actor->mSpawnPos = pos;
            }
        }
        pos.x += step.x;
        pos.y += step.y;
    }
}

inline bool cls_804A5BD8::checkTrigger() {
    f32 range = 16.0f * field_0x344;
    bool trigger = false;
    switch (field_0x340) {
    case 0:
        trigger = true;
        break;
    case 1:
        for (s32 i = 0; i < 4; i++) {
            if (fn_801D08B4(i, &mPos, range)) {
                trigger = true;
                break;
            }
        }
        break;
    case 2:
        for (s32 i = 0; i < 4; i++) {
            if (field_0x345 & (1 << i)) {
                if (fn_801D0B44(i) && !fn_801D0D50(i, &mPos, range)) {
                    trigger = true;
                    break;
                }
            } else {
                if (fn_801D08B4(i, &mPos, range)) {
                    field_0x345 |= (1 << i);
                }
            }
        }
        break;
    case 3:
        if (field_0x346 != fn_8022461C(&mPos, field_0x4) || field_0x348 != fn_80226850(&mPos, field_0x4)) {
            trigger = true;
        }
        break;
    }
    return trigger;
}

inline void cls_804A5BD8::calcCenter(f32* outX, f32* outY) {
    JGeometry::TVec2f sum;
    sum.x = 0.0f;
    sum.y = 0.0f;
    s32 count = 0;
    for (s32 i = 0; i < 4; i++) {
        if (!mSlots[i].mUsed && !mSlots[i].mDone) {
            s32 serial = mSlots[i].mSerial;
            if (serial >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (actor != NULL) {
                    count++;
                    sum.x += actor->mPos.x;
                    sum.y += actor->mPos.y;
                }
            }
        }
    }
    if (count > 0) {
        f32 inv = 1.0f / (f32)count;
        *outX = sum.x * inv;
        *outY = sum.y * inv;
    } else {
        *outX = mPos.x;
        *outY = mPos.y;
    }
}

inline s32 cls_804A5BD8::chooseDir() {
    JGeometry::TVec3f center;
    calcCenter(&center.x, &center.y);
    s32 player = fn_801D05F8(&center, 0.0f);
    if (player < 0) {
        return fn_801CD664(4);
    }
    bool turn = false;
    if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.1f) {
        turn = true;
    }
    return fn_801D0020(&center, player, turn);
}

cls_804A5BD8::cls_804A5BD8() {
    field_0x340 = 0;
    field_0x344 = 0;
    field_0x345 = 0;
    field_0x346 = 0;
    field_0x348 = 0;
    field_0x34a = 0;
}

void cls_804A5BD8::init() {
    EnemyBase_804A4808::init();
    field_0x2ac = field_0x90 & 3;
    if (field_0x2ac >= 2 || field_0x2ac < 0) {
        field_0x2ac = fn_801CD664(2);
    }
    s32 mode = (field_0x90 >> 4) & 3;
    if (mode > 3) {
        mode = 3;
    } else if (mode < 0) {
        mode = 0;
    }
    field_0x340 = mode;
    field_0x344 = (field_0x90 >> 8) & 0xFF;

    mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
    mBox2.set(mBox);
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0xb0 = 0;
    field_0x98 = 2;
    mFlags |= ENEMY_FLAG_NO_ATTACK | ENEMY_FLAG_NO_HURT | ENEMY_FLAG_KEEP_LIFE;
    mLife = 1000;
    mLifeBackup = mLife;

    if (field_0x340 == 0) {
        mState = 2;
        field_0x250 = fn_801CD664(0x79) + 60;
        spawnSlimes();
    } else {
        mState = 0;
        field_0x250 = -1;
    }
    mStateTimer = 0;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
}

void cls_804A5BD8::vfunc_0x54() {
    field_0x346 = fn_8022461C(&mPos, field_0x4);
    field_0x348 = fn_80226850(&mPos, field_0x4);
}

void cls_804A5BD8::vfunc_0xA4() {
    if (mState != 5) {
        fn_80208244(5);
    }
    for (s32 i = 0; i < 4; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(mSlots[i].mSerial);
        if (actor != NULL) {
            static_cast<cls_804A5D60*>(actor)->fn_80209D44();
        }
    }
}

void cls_804A5BD8::vfunc_0xA8() {
    if (mState != 7) {
        fn_80208244(7);
    }
    for (s32 i = 0; i < 4; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(mSlots[i].mSerial);
        if (actor != NULL) {
            static_cast<cls_804A5D60*>(actor)->fn_80209CA4(
                mSlots[i].mPlayer, mSlots[i].field_0xc, &mSlots[i].mTarget, mSlots[i].field_0x1c);
        }
    }
}

// FIXME: 98.86% (regalloc-only). r25-r28 shifted by 1 in calcCenter loops and f2/f3/f4 vs f30/f31 in offset addition.
void cls_804A5BD8::fn_80208520(JGeometry::TVec2f* outPos, JGeometry::TVec2f* outStep) {
    if (field_0x2ac == 0) {
        outStep->x = 0.0f;
        outStep->y = 16.0f;
    } else {
        outStep->x = 16.0f;
        outStep->y = 0.0f;
    }
    JGeometry::TVec2f center;
    calcCenter(&center.x, &center.y);
    s32 dir = fn_801CD354(chooseDir());
    JGeometry::TVec2f offset;
    offset.x = 0.0f;
    offset.y = 0.0f;
    switch (dir) {
    case 0:
        offset.x += 48.0f;
        break;
    case 1:
        offset.x -= 48.0f;
        break;
    case 2:
        offset.y += 48.0f;
        break;
    case 3:
        offset.y -= 48.0f;
        break;
    }
    center.x += offset.x;
    center.y += offset.y;
    center.x -= 1.5f * outStep->x;
    center.y -= 1.5f * outStep->y;
    outPos->x = center.x;
    outPos->y = center.y;
}

// FIXME: 98.24% (missing/extra). 1 fewer instruction (182 vs 183: missing mr r28, r3 for steps in case 3), plus r26-r29 regalloc and stack frame offset swap (0xc vs 0x1c) between chooseDir and case 4.
void cls_804A5BD8::fn_80208244(int state) {
    switch (state) {
    case 1:
        break;
    case 2:
        field_0x250 = fn_801CD664(0x79) + 60;
        break;
    case 3: {
        s32 dir = chooseDir();
        u8 steps = fn_801CD664(3) + 1;
        field_0x34a += steps;
        for (s32 i = 0; i < 4; i++) {
            if (!mSlots[i].mUsed && !mSlots[i].mDone) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(mSlots[i].mSerial);
                if (actor != NULL) {
                    static_cast<cls_804A5D60*>(actor)->fn_80209D20(dir, steps);
                }
            }
        }
        break;
    }
    case 4: {
        JGeometry::TVec2f step;
        JGeometry::TVec2f pos;
        fn_80208520(&pos, &step);
        for (s32 i = 0; i < 4; i++) {
            if (!mSlots[i].mUsed && !mSlots[i].mDone) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(mSlots[i].mSerial);
                if (actor != NULL) {
                    static_cast<cls_804A5D60*>(actor)->fn_80209CF4(&pos);
                }
            }
            pos.x += step.x;
            pos.y += step.y;
        }
        mFlags |= 0x2000;
        break;
    }
    case 5:
    case 6:
    case 7:
        break;
    }
    mStateTimer = 0;
    mState = state;
}

// FIXME: 99.80% (regalloc-only). r29 vs r27 for loop counter i in checkTrigger case 2.
void cls_804A5BD8::vfunc_0x90(u8 mapHit) {
    fn_80207A60();
    switch (mState) {
    case 0:
        if (checkTrigger()) {
            spawnSlimes();
            fn_80208244(1);
        }
        break;
    case 2:
        if (mStateTimer >= field_0x250) {
            if (!(mFlags & 0x2000) && field_0x34a >= 5) {
                fn_80208244(4);
            } else {
                fn_80208244(3);
            }
        }
        break;
    case 1:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        break;
    }
}

void cls_804A5BD8::vfunc_0x88() {
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.z = 0.0f;
}

void cls_804A5BD8::fn_80207ECC() {
    switch (mState) {
    case 1:
        fn_80208244(3);
        break;
    }
}

bool cls_804A5BD8::fn_80207E58() {
    bool result = false;
    switch (mState) {
    case 3:
        if (field_0x34a >= 10) {
            fn_80208244(6);
            result = true;
        } else {
            fn_80208244(2);
        }
        break;
    case 6:
        result = true;
        break;
    }
    return result;
}

void cls_804A5BD8::fn_80207E24() {
    switch (mState) {
    case 4:
        fn_80208244(2);
        break;
    }
}

cls_804A5BD8::~cls_804A5BD8() {}
