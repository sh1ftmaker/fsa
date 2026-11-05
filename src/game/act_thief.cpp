#include <stdlib.h>
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/EnemyBase.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/GameView.h"
#include "game/act/act_thief.h"

static JGeometry::TVec2f lbl_8053AC60; // the sort origin, set by fn_802063FC
static int fn_802068A0(const void* a, const void* b);

bool ActorBase_804A5770::fn_80204D44() {
    bool result = false;
    if (field_0x94 == 1 || field_0xc4 != 0) {
        result = true;
    }
    return result;
}

void Thief_804A5A30::vfunc_0xA0() {
    if (mState != 9 && mState != 8) {
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
    }
}

void Thief_804A5A30::vfunc_0x98() {
    if (mState == 3) {
        if (field_0xc4 != 0) {
            field_0x98 = fn_80230CFC(field_0xc8);
            field_0x33c = fn_801F6874(field_0x98);
            JGeometry::TVec3f* pos = fn_8023E7B0(field_0xc8);
            mPos.x = pos->x;
            mPos.y = pos->y;
            mPos.z = pos->z;
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.z = 0.0f;
            if (field_0x4 != getPlayerLayer_8023DE58(field_0xc8)) {
                field_0x4 = getPlayerLayer_8023DE58(field_0xc8);
                mPathCount = 0;
                s32 player = field_0xc8;
                if (fn_8022F514(player)) {
                    player = fn_8022F3DC(player);
                }
                JGeometry::TVec3f* playerPos = getPlayerPos_8023E724(player);
                field_0x32c.x = playerPos->x;
                field_0x32c.y = playerPos->y;
                field_0x32c.z = playerPos->z;
            }
        }
    } else if ((mFlags & 7) == 0) {
        fn_801CE32C();
    }
}

static inline bool contains(const JGeometry::TBox2f& b, const JGeometry::TVec3f& p) {
    return p.x >= b.i.x && p.y >= b.i.y && p.x < b.f.x && p.y < b.f.y;
}

// True in state 1 when no fn_801CD2F8 hit is set for the layer and mPos lies
// outside the view box of that layer.
bool Thief_804A5A30::fn_8020609C() const {
    s32 layer;
    bool result = false;
    if (field_0x94 == 1) {
        if (!fn_801CD2F8(field_0x4)) {
            layer = field_0x4;
            GameView_804A9950* view = getGameView_802D6A9C();
            const JGeometry::TBox2f& box = layer < 8 ? view->mVisibleBox : view->field_0xdc;
            if (!contains(box, mPos)) {
                result = true;
            }
        }
    }
    return result;
}

s32 Thief_804A5A30::fn_802063FC() {
    ActorBase_804A5770* actors[0x400];
    JGeometry::TVec2f from;
    from.x = mPos.x;
    from.y = mPos.y;
    s32 count = getActorManager_801F856C()->fn_801F7274(field_0x4, actors, -1);
    if (count >= 2) {
        lbl_8053AC60 = from;
        qsort(actors, count, 4, fn_802068A0);
    }
    s32 result = -1;
    ActorBase_804A5770** p = actors;
    s32 end = count > 0x14 ? 0x14 : count;
    for (s32 i = 0; i < end; i++, p++) {
        if (!fn_801CF888(&from, &(*p)->mPos, field_0x4)) {
            result = (*p)->mSerial;
            break;
        }
    }
    return result;
}

bool Thief_804A5A30::fn_802064E0() {
    bool result = false;
    s32 bit = field_0x354;
    if (bit != 0) {
        if (getGameView_802D6A9C()->fn_802D7F40(bit)) {
            result = true;
        }
    }
    return result;
}

Thief_804A5A30::~Thief_804A5A30() {
    releaseEmitter_801EE434(mEmitter358);
    fn_802049E0(fn_80204638(), this);
}

Thief_804A5A30::Thief_804A5A30() {
    field_0x340 = -1;
    field_0x344 = 0;
    field_0x348 = 0;
    field_0x349 = 0;
    field_0x34a = 0;
    field_0x34b = 0;
    field_0x34c = 0;
    mType = 0;
    field_0x354 = 0;
    mEmitter358 = NULL;
    field_0x35c.frames = 0;
    field_0x35c.vel.x = 0.0f;
    field_0x35c.vel.y = 0.0f;
    field_0x368.fn_80217484();
    field_0x374.fn_8021BA30(0);
}

// qsort comparator (0x802068A0): orders actors by squared distance from lbl_8053AC60.
static int fn_802068A0(const void* a, const void* b) {
    const ActorBase_804A5770* actorA = *(ActorBase_804A5770* const*)a;
    const ActorBase_804A5770* actorB = *(ActorBase_804A5770* const*)b;
    f32 distA = square(actorA->mPos.x - lbl_8053AC60.x) + square(actorA->mPos.y - lbl_8053AC60.y);
    f32 distB = square(actorB->mPos.x - lbl_8053AC60.x) + square(actorB->mPos.y - lbl_8053AC60.y);
    int result = 0;
    if (distA < distB) {
        return -1;
    }
    if (distA > distB) {
        result = 1;
    }
    return result;
}
