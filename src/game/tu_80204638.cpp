#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/ActorBase.h"
#include <float.h>
#include "game/act/act_thief.h"
#include "JSystem/JAudio/JAUAudience.h"
#include "JSystem/JAudio/System/JASTrack.h"
#include "JSystem/JAudio/System/JASChannel.h"
#include "JSystem/JAudio/Interface/JAISe.h"
#include "JSystem/JAudio/Interface/JAISeq.h"
#include "JSystem/JAudio/Interface/JAISoundChild.h"
#include "JSystem/JAudio/Interface/JAIStream.h"

// Out-of-line copy used by the repel step: pushes `a` away from `b` (2D) into *out, true when it moved.
bool fn_801CB85C(const JGeometry::TVec2f* a, const JGeometry::TVec2f* b, JGeometry::TVec2f* out, f32 min, f32 max);

inline cls_80204638::cls_80204638() : field_0x0(0), mCount(0) {
    fn_80204ABC(true);
}

void cls_80204638::fn_80204ABC(bool reset) {
    if (reset) {
        field_0x0 = 0;
        for (s32 i = 0; i < 10; i++) {
            for (s32 j = 0; j < 2; j++) {
                mSlots[i][j] = NULL;
            }
        }
    }
}

// FIXME(shotgun): 92.82 %, missing/extra; behaviour near.
void cls_80204638::fn_80204A20(ActorBase_804A5770* actor) {
    ActorBase_804A5770** slot = mSlots[static_cast<Thief_804A5A30*>(actor)->field_0x34a];
    if (slot[0] == NULL) {
        slot[0] = actor;
        if (slot[1] != NULL && actor->fn_80204D44()) {
            actor->mAlive = false;
        }
    } else if (slot[1] == NULL) {
        slot[1] = actor;
        if (slot[0] != NULL && actor->fn_80204D44()) {
            actor->mAlive = false;
        }
    }
}

// FIXME(shotgun): 96.25 %, different; behaviour near.
void cls_80204638::fn_802049E0(ActorBase_804A5770* actor) {
    ActorBase_804A5770** slot = mSlots[static_cast<Thief_804A5A30*>(actor)->field_0x34a];
    if (slot[0] == actor) {
        slot[0] = NULL;
    } else if (slot[1] == actor) {
        slot[1] = NULL;
    }
}

// FIXME(shotgun): 98.14 %, regalloc-only; behaviour clean.
void cls_80204638::fn_802048F4() {
    GameView_804A9950* view = getGameView_802D6A9C();
    for (s32 i = 0; i < 10; i++) {
        for (s32 j = 0; j < 2; j++) {
            ActorBase_804A5770* actor = mSlots[i][j];
            if (actor != NULL && actor->mAlive && actor->field_0x4 == 0) {
                bool inside = false;
                if (actor->mPos.x >= view->mVisibleBox.i.x && actor->mPos.y >= view->mVisibleBox.i.y &&
                    actor->mPos.x < view->mVisibleBox.f.x && actor->mPos.y < view->mVisibleBox.f.y) {
                    inside = true;
                }
                if (!inside && !actor->fn_80204D44()) {
                    actor->mAlive = false;
                }
            }
        }
    }
}

void cls_80204638::fn_80204834() {
    for (s32 i = 0; i < 10; i++) {
        for (s32 j = 0; j < 2; j++) {
            if (mSlots[i][j] != NULL) {
                mSlots[i][j]->mAlive = false;
            }
        }
    }
}

// FIXME(shotgun): 78.79 %, missing/extra; behaviour drift.
bool cls_80204638::fn_802046FC(JGeometry::TVec3f* vel, JGeometry::TVec3f* pos) {
    if (mCount >= 10) {
        return false;
    }
    bool moved = false;
    if (mCount >= 1) {
        JGeometry::TVec2f p(pos->x, pos->y);
        s32 nearest = 0;
        if (mCount > 1) {
            f32 best = FLT_MAX;
            for (s32 i = 0; i < mCount; i++) {
                f32 dy = mPositions[i].y - pos->y;
                f32 dx = mPositions[i].x - pos->x;
                f32 dist = dx * dx + dy * dy;
                if (dist < best) {
                    best = dist;
                    nearest = i;
                }
            }
        }
        JGeometry::TVec2f from(mPositions[nearest].x, mPositions[nearest].y);
        JGeometry::TVec2f dir(vel->x, vel->y);
        moved = fn_801CB85C(&from, &dir, &p, 20.0f, 0.5f);
        if (moved) {
            pos->x = p.x;
            pos->y = p.y;
        }
    }
    JGeometry::TVec3f* slot = &mPositions[mCount];
    slot->x = pos->x;
    slot->y = pos->y;
    slot->z = pos->z;
    mCount++;
    return moved;
}

extern "C" cls_80204638* fn_80204638() {
    static cls_80204638 sInstance;
    return &sInstance;
}

// Pool statics this unit's static initializer registers. The others come in through the headers.
// FIXME: JASTrack, JASChannel, JAIStream, JAISoundChild, JAISeq and JAISe pools still differ.
template class JASPoolAllocObject_MultiThreaded<JASTrack::TChannelMgr>;
template class JASPoolAllocObject<JAUDopplerAudible<4> >;
template class JASPoolAllocObject<JAUAudible<4> >;
