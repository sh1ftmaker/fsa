// The actor manager (include/game/ActorManager.h) and its helpers.

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"

// The angle of each direction (fn_801F6874; fn_801F681C is the inverse).
static const u16 lbl_8053F800[4] = {0x4000, 0xC000, 0x0000, 0x8000};

s32 ActorManager_804A5800::fn_801F6058(s32 i) {
    return field_0x3030[i];
}

void ActorManager_804A5800::fn_801F6204(u8 value) {
    field_0x305c = value;
}

ActorSoundObj_804578F4* ActorManager_804A5800::fn_801F6644(s32 slot) {
    if (mActors[slot]->field_0x4 == 0) {
        return mSounds[slot];
    }
    return field_0x200c;
}

ActorBase_804A5770* ActorManager_804A5800::fn_801F666C(s32 serial) {
    ActorBase_804A5770* found = NULL;
    if (serial >= 0) {
        // The slot of the last spawn first.
        if (mLastSlot >= 0 && mActors[mLastSlot] != NULL && mActors[mLastSlot]->mAlive &&
            serial == mActors[mLastSlot]->mSerial)
        {
            found = mActors[mLastSlot];
        } else {
            for (s32 i = 0; i < mActorEnd; i++) {
                if (mActors[i] != NULL && mActors[i]->mAlive && serial == mActors[i]->mSerial) {
                    found = mActors[i];
                    break;
                }
            }
        }
    }
    return found;
}

s32 fn_801F681C(u16 angle) {
    s32 dir = 1;
    if (angle < 0x2000 || angle >= 0xE000) {
        dir = 2;
    } else if (angle >= 0x2000 && angle < 0x6000) {
        dir = 0;
    } else if (angle >= 0x6000 && angle <= 0xA000) {
        dir = 3;
    }
    return dir;
}

u16 fn_801F6874(s32 dir) {
    return lbl_8053F800[dir & 3];
}

// FIXME: 95.2 %. In the last branch the target zero-extends delta (clrlwi 16)
// before adding it to *value; this code adds the s16 directly.
void fn_801F714C(u16* value, u16 target, u16 step, u16 maxStep) {
    s16 diff = target - *value;
    s16 delta = diff / step;
    if (delta > maxStep) {
        *value += maxStep;
    } else if (delta < -maxStep) {
        *value -= maxStep;
    } else {
        *value += delta;
    }
}

void fn_801F71A4(f32* value, f32 target, f32 rate, f32 maxStep) {
    if (*value != target) {
        f32 delta = rate * (target - *value);
        if (delta > maxStep) {
            delta = maxStep;
        } else if (delta < -maxStep) {
            delta = -maxStep;
        }
        *value += delta;
    }
}

// The bound is read once (as fn_801F7274 and fn_801F732C keep it in a register
// across calls).
s32 ActorManager_804A5800::fn_801F7818(u32 id, s32 layer, ActorBase_804A5770** out,
                                       s32 excludeSerial) {
    s32 count = 0;
    s32 end = mActorEnd;
    for (s32 i = 0; i < end; i++) {
        if (mActors[i] != NULL && mActors[i]->mAlive && layer == mActors[i]->field_0x4 &&
            id == mActors[i]->mActorId && excludeSerial != mActors[i]->mSerial)
        {
            *out++ = mActors[i];
            count++;
        }
    }
    return count;
}

s32 ActorManager_804A5800::fn_801F7884(u32 id, s32 layer) {
    for (s32 i = 0; i < mActorEnd; i++) {
        if (mActors[i] != NULL && mActors[i]->mAlive && layer == mActors[i]->field_0x4 &&
            id == mActors[i]->mActorId)
        {
            return mActors[i]->mSerial;
        }
    }
    return -1;
}

s32 ActorManager_804A5800::fn_801F7A08(u32 id) {
    for (s32 i = 0; i < mActorEnd; i++) {
        if (mActors[i] != NULL && mActors[i]->mAlive && id == mActors[i]->mActorId) {
            return mActors[i]->mSerial;
        }
    }
    return -1;
}

s32 ActorManager_804A5800::fn_801F7A6C() {
    s32 count = 0;
    for (s32 i = 0; i < mActorEnd; i++) {
        if (mActors[i] != NULL && mActors[i]->mAlive && mActors[i]->field_0x1a8) {
            count++;
        }
    }
    return count;
}

// FIXME: 97.7 %, regalloc only: the target counts down mActorEnd in r0 and
// keeps the stored 0 in r6; this code swaps them.
s32 ActorManager_804A5800::fn_801F7AB8(u32 id, s32 excludeSerial) {
    s32 count = 0;
    s32 end = mActorEnd;
    for (s32 i = 0; i < end; i++) {
        if (mActors[i] != NULL && mActors[i]->mAlive && id == mActors[i]->mActorId &&
            excludeSerial != mActors[i]->mSerial)
        {
            mActors[i]->mAlive = false;
            count++;
        }
    }
    return count;
}

ActorManager_804A5800* fn_801F856C(void) {
    return lbl_8053AAF8->field_0x4->mActorManager;
}
