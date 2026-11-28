#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/navi_list_8021F064.h"
#include "game/act/tu_801F9484.h"

void NaviList_8021F064::fn_8021F27C() {
    mFlags = 0;
    mCount = 0;
}

void NaviList_8021F064::fn_8021F25C(s32 serial) {
    mSerials[mCount] = serial;
    mCount++;
}

// FIXME: 92.96 %, missing/extra; behaviour clean.
void NaviList_8021F064::fn_8021F1F0(s32 serial) {
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

// FIXME: 99.38 %, regalloc-only; behaviour clean.
void NaviList_8021F064::fn_8021F130() {
    Navi_804A6708* navi;
    if (mCount > 0 && (mFlags & 3) != 0) {
        for (s32 i = 0; i < mCount; i++) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(mSerials[i]);
            navi = static_cast<Navi_804A6708*>(actor);
            if (navi) {
                if (mFlags & 1) {
                    navi->fn_8021D764();
                }
                if (mFlags & 2) {
                    navi->fn_8021D734();
                }
            }
        }
        mFlags &= ~3;
    }
}

bool NaviList_8021F064::fn_8021F094() const {
    if (mCount <= 0) {
        return false;
    }
    s32 count = mCount;
    bool result = true;
    for (s32 i = 0; i < count; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(mSerials[i]);
        if (actor != NULL && !static_cast<Navi_804A6708*>(actor)->fn_8021D794()) {
            result = false;
            break;
        }
    }
    return result;
}

NaviList_8021F064* fn_8021F064() {
    static NaviList_8021F064 lbl_804CE640;
    return &lbl_804CE640;
}
