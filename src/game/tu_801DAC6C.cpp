#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/act/tu_801D8400.h"
#include "game/act/tu_801DAC6C.h"

Singleton_801DAC6C* fn_801DAC6C() {
    static Singleton_801DAC6C lbl_804C92D0;
    return &lbl_804C92D0;
}

// FIXME: a counter (0-4) other units step with lbz/stb at byte 0. In the
// original it sits at 0x8053AC30, 8-aligned; ours lands at 0x8053AC29, so
// .sbss does not match. Its real type is unknown.
u8 lbl_8053AC30;

// FIXME: 81%. Same instructions; the original loads the serial (lwz r31) and
// clears the result (li r3, 0) before it saves r30 and copies out into it
// (mr r30, r4), and issues the cmpwi before the second and third stfs. Ours
// copies out first.
bool Singleton_801DAC6C::fn_801DACAC(JGeometry::TVec3f* out) {
    bool result = false;
    s32 serial = mSerial0;
    out->set(0.0f, 0.0f, 0.0f);
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            out->x = actor->mPos.x;
            out->y = actor->mPos.y;
            out->z = actor->mPos.z;
        }
        result = true;
    }
    return result;
}

// FIXME: 91%. Same instructions; the original loads the serial (lwz r31)
// before saving r30 and clearing the result (li r30, 0). Ours clears first.
s32 Singleton_801DAC6C::fn_801DAD2C() {
    s32 serial = mSerial4;
    s32 result = 0;
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            result = fn_801DC264(actor);
        }
    }
    return result;
}

void Singleton_801DAC6C::fn_801DAD88() {
    s32 serial = mSerial0;
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            static_cast<cls_804A4C58*>(actor)->fn_801D8400();
        }
    }
}

void Singleton_801DAC6C::fn_801DADD0() {
    s32 serial = mSerial0;
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            actor->startSound_801F0E34(0x40005, 0);
        }
    }
}

// FIXME: 87%. The original reads mCount into r31 first, then r30 = -1,
// r29 = list pointer, r28 = i; ours allocates them in the reverse order
// (r31 = -1, r30 = pointer, r29 = i, r28 = count).
void Singleton_801DAC6C::fn_801DAE24() {
    s32 count = mCount;
    s32 i = 0;
    while (i < count) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(mList[i]);
        if (actor != NULL) {
            actor->field_0x10c = -1;
            actor->vfunc_0x24();
        }
        i++;
    }
}

// FIXME: 96%. Only registers: the original starts i from the same zero as
// found (mr r5, r6; i = r5, count reloaded into r8); ours uses a separate li
// and gets i = r8, count = r5.
void Singleton_801DAC6C::fn_801DAEAC(s32 serial) {
    bool found = false;
    s32* p = mList;
    if (mCount <= 0) {
        return;
    }
    for (s32 i = 0; i < mCount; i++, p++) {
        if (!found) {
            if (serial == *p) {
                found = true;
            }
        } else {
            p[-1] = *p;
        }
    }
    if (found) {
        mCount--;
    }
}

void Singleton_801DAC6C::fn_801DAF14(s32 serial) {
    if (mCount < 30) {
        mList[mCount] = serial;
        mCount++;
    }
}

void Singleton_801DAC6C::fn_801DAF3C(s32 serial) {
    mSerial4 = serial;
}

void Singleton_801DAC6C::fn_801DAF44(s32 serial) {
    mSerial0 = serial;
}

void Singleton_801DAC6C::fn_801DAF4C() {
    mSerial0 = -1;
    mSerial4 = -1;
    mCount = 0;
    field_0x84 = 0;
}
