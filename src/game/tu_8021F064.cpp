#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/tu_8021F064.h"

cls_8021F064* fn_8021F064() {
    static cls_8021F064 lbl_804CE640;
    return &lbl_804CE640;
}

bool cls_8021F064::fn_8021F094() const {
    if (mCount <= 0) {
        return false;
    }
    s32 count = mCount;
    bool result = true;
    for (s32 i = 0; i < count; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(mSerials[i]);
        if (actor != NULL && !fn_8021D794(actor)) {
            result = false;
            break;
        }
    }
    return result;
}

void cls_8021F064::fn_8021F130() {
    // FIXME: 97.7%. Only the null test of the looked-up actor differs: the
    // original does mr r0, r3 then mr. r30, r0; here it is a single mr. r30, r3.
    if (mCount > 0 && (mFlags & 3) != 0) {
        ActorBase_804A5770* actor;
        for (s32 i = 0; i < mCount; i++) {
            actor = getActorManager_801F856C()->findActorBySerial_801F666C(mSerials[i]);
            if (actor) {
                if (mFlags & 1) {
                    fn_8021D764(actor);
                }
                if (mFlags & 2) {
                    fn_8021D734(actor);
                }
            }
        }
        mFlags &= ~3;
    }
}

void cls_8021F064::fn_8021F1F0(s32 serial) {
    // FIXME: 93%. The original tests mCount twice (a second blelr before the
    // loop, i copied from the zeroed removed flag in r6) and keeps mCount in
    // r8 at the loop test; here i takes r8 and mCount r5.
    if (mCount > 0) {
        bool removed = false;
        s32* p = mSerials;
        for (s32 i = 0; i < mCount; i++, p++) {
            if (!removed) {
                if (serial == *p) {
                    removed = true;
                }
            } else {
                p[-1] = *p;
            }
        }
        if (removed) {
            mCount--;
        }
    }
}

void cls_8021F064::fn_8021F25C(s32 serial) {
    mSerials[mCount] = serial;
    mCount++;
}

void cls_8021F064::fn_8021F27C() {
    mFlags = 0;
    mCount = 0;
}
