// The actor manager (include/game/ActorManager.h) and its helpers.

#include "game/GameAudio.h"
#include <float.h>
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/Player.h"
#include "game/cls_804AE9B0.h"

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

void ActorManager_804A5800::fn_801F6884() {
    for (s32 i = 0; i < 4; i++) {
        fn_80230224(i, false);
    }
    field_0x3041 = 0;
    lbl_8053AD70->fn_8037AE30();
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

// The live `id` actor on `layer` nearest to pos (x, y), other than
// excludeSerial; its serial, or -1.
s32 ActorManager_804A5800::fn_801F76F0(u32 id, s32 layer, const JGeometry::TVec3f* pos,
                                       s32 excludeSerial) {
    s32 serial = -1;
    f32 best = FLT_MAX;
    for (s32 i = 0; i < mActorEnd; i++) {
        if (mActors[i] != NULL && mActors[i]->mAlive && id == mActors[i]->mActorId &&
            layer == mActors[i]->field_0x4 && excludeSerial != mActors[i]->mSerial)
        {
            f32 dist = square(mActors[i]->mPos.x - pos->x) + square(mActors[i]->mPos.y - pos->y);
            if (dist < best) {
                best = dist;
                serial = mActors[i]->mSerial;
            }
        }
    }
    return serial;
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

s32 ActorManager_804A5800::fn_801F8448(ActorBase_804A5770* actor) {
    u8 unlinked = false;
    if (actor->mListPrev != NULL) {
        actor->mListPrev->mListNext = actor->mListNext;
        if (actor->mListNext != NULL) {
            actor->mListNext->mListPrev = actor->mListPrev;
        }
        unlinked = true;
    } else if (actor == mListHead) {
        mListHead = actor->mListNext;
        if (mListHead != NULL) {
            mListHead->mListPrev = NULL;
        }
        unlinked = true;
    }
    if (unlinked) {
        actor->mListPrev = NULL;
        actor->mListNext = NULL;
    }
    return unlinked;
}

s32 ActorManager_804A5800::fn_801F8544(u32 id, s32 arg_0x4, const JGeometry::TVec3f* pos, u32 arg_0x90) {
    return fn_801F9484(id, arg_0x4, pos, arg_0x90, -1, -1);
}

ActorManager_804A5800* fn_801F856C(void) {
    return lbl_8053AAF8->field_0x4->mActorManager;
}
