#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/tu_80212AC0.h"

cls_80212AC0* fn_80212AC0() {
    static cls_80212AC0 lbl_804CCDB8;
    return &lbl_804CCDB8;
}

void cls_80212AC0::fn_80212AF0(JGeometry::TVec3f* pos) const {
    // FIXME: 88.5 %. The original sets found (li r29, 0) before the mCount
    // load and copies it into i (mr r30, r29), and numbers the registers
    // differently (pos r26, this r27, actor r28, found r29, i r30; ours this
    // r30, actor r27, i r28); ours sets i and found with two li after it.
    ActorBase_804A5770* found = NULL;
    for (s32 i = 0; i < mCount; i++) {
        ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(mSerials[i]);
        if (actor != NULL && fn_80211000(actor, pos)) {
            found = actor;
            break;
        }
    }
    if (found != NULL) {
        JGeometry::TVec3f push;
        fn_80210E90(found, pos, &push);
        *pos += push;
    }
}

void cls_80212AC0::fn_80212BA0(s32 serial) {
    bool removed = false;
    for (s32 i = 0; i < mCount; i++) {
        if (!removed) {
            if (serial == mSerials[i]) {
                removed = true;
            }
        } else {
            mSerials[i - 1] = mSerials[i];
        }
    }
    if (removed) {
        mCount--;
    }
}

void cls_80212AC0::fn_80212BFC(s32 serial) {
    mSerials[mCount] = serial;
    mCount++;
}
