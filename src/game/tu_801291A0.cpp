#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/ViewLayer.h"
#include "game/act/tu_801291A0.h"

// The 11 bits of field_0x90 that select an item, the bit each sets in
// field_0x280 and the item id it adds to mItems.
static const s32 lbl_80465000[11] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
static const s32 lbl_8046502C[11] = {18, 19, 20, 21, 22, 23, 24, 25, 26, 4, 4};
static const s32 lbl_80465058[11] = {10, 3, 4, 8, 6, 5, 7, 1, 9, 2, 11};
// The bits of field_0x90 that add kind 0..3 to mKinds.
static const s32 lbl_80465084[4] = {16, 17, 18, 19};

cls_8049E5B8::~cls_8049E5B8() {}

// FIXME: 99.4%. try.py class: regalloc-only. In the CASE 2 spawn path the
// original keeps the LCG state in r5 and the multiplier in r4 (ours r4 / r5),
// loads mItemCount into r5, and recomputes the multiplier (lis/addi) for the
// third random call where ours reuses the register. The .sdata2 pool is also
// ordered 1.0f, 10.0f, 0.8f in the original (ours 10.0f, 1.0f, 0.8f).
bool cls_8049E5B8::fn_80129200() {
    JGeometry::TVec3f pos = mPos;
    s32 layer = field_0x4;
    if (viewLayerFlag(layer) == 0) {
        pos.z = 10.0f;
    }
    s32 kind = mKind;
    if (kind == 15) {
        if (mKindCount <= 0) {
            kind = 1;
        } else {
            kind = mKinds[(s32)(mKindCount * lbl_8053AAF8->mRandom.get_ufloat_1())];
        }
    }
    switch (kind) {
    case 0:
        mSpawned = fn_801F856C()->fn_801F9484(ACTOR_CRTL, field_0x4, &pos, field_0x280, -1, -1);
        break;
    case 2: {
        s32 item = mItems[(s32)(mItemCount * lbl_8053AAF8->mRandom.get_ufloat_1())];
        if (mItemCount == 0) {
            item = mItems[(s32)(10.0f * lbl_8053AAF8->mRandom.get_ufloat_1())];
        }
        u32 arg;
        if ((lbl_8053AAF8->mRandom.get_ufloat_1() >= 0.8f && item != 2) || item == 11) {
            arg = item | 0x800180;
        } else {
            arg = item | 0x400180;
        }
        mSpawned = fn_801F856C()->fn_801F9484(ACTOR_LSTI, field_0x4, &pos, arg, -1, -1);
        fn_801F0E34(0x3004A, 0);
        break;
    }
    case 3:
        mSpawned = fn_801F856C()->fn_801F9484(ACTOR_NNJN, field_0x4, &pos, 1, -1, -1);
        break;
    default:
        mSpawned = fn_801F856C()->fn_801F9484(ACTOR_CRTL, field_0x4, &pos, 0, -1, -1);
        break;
    }
    mTimer = 0;
    return true;
}

// The state setter: 1 clears the spawn, 2 spawns a new one. State 0 is
// never entered with work to do, but the original's switch has a case 0.
void cls_8049E5B8::fn_8012952C(s32 state) {
    switch (state) {
    case 0:
        break;
    case 1:
        mSpawned = -1;
        mTimer = 0;
        break;
    case 2:
        fn_80129200();
        break;
    }
    mState = state;
}

// Whether the actor is one of the item actors this one spawns.
static inline bool isSpawnedKind(u32 id) {
    switch (id) {
    case ACTOR_LSTI:
    case ACTOR_CRTL:
    case ACTOR_NNJN:
        return false;
    }
    return true;
}

// FIXME: 80.6%. try.py class: missing/extra. The original calls fn_8012952C
// (bl, three sites); MWCC auto-inlines it into update here, which also shifts
// the saved registers (this in r30, the serial in r31). Defining it after
// update un-inlines it (95.6%) but swaps the .text order, so it stays here.
void cls_8049E5B8::update() {
    if (vfunc_0x3C() > 0 && !vfunc_0x34()) {
        return;
    }
    switch (mState) {
    case 0: {
        s32 serial = mSpawned;
        bool gone;
        if (serial < 0) {
            gone = true;
        } else {
            ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(serial);
            if (actor == NULL) {
                gone = true;
            } else {
                gone = isSpawnedKind(actor->mActorId);
            }
        }
        if (gone) {
            fn_8012952C(1);
        }
        break;
    }
    case 1: {
        mTimer++;
        s32 limit = 600;
        if (mKind == 2) {
            limit = 300;
        }
        if (mTimer >= limit) {
            fn_8012952C(2);
        }
        break;
    }
    case 2:
        fn_8012952C(0);
        break;
    }
}

// FIXME: 86.5%. try.py class: missing/extra (3 instrs). The original stores
// the first ten mItems with no loop guard (i = 10 set up at the top, only the
// last iteration guarded by cmpwi i, 11); ours emits an extra `cmpwi 0, 11 /
// bge` guard before the ten stores. The remaining loops match. Tried: i <= 10,
// a shared `i`, a 10 + 1 loop pair, do/while, mItems[11].
void cls_8049E5B8::init() {
    mKind = field_0x90 & 0xF;
    field_0x280 |= 2;
    // 11 entries, one more than mItems holds: the last lands on mKinds[0],
    // which is set again below.
    s32 i;
    for (i = 0; i < 11; i++) {
        mItems[i] = 10;
    }
    for (i = 0; i < 11; i++) {
        if (((field_0x90 >> lbl_80465000[i]) & 1) == 1) {
            field_0x280 |= 1 << lbl_8046502C[i];
            mItems[mItemCount] = lbl_80465058[i];
            mItemCount++;
        }
    }
    mKinds[0] = 1;
    mKinds[1] = 1;
    mKinds[2] = 1;
    mKinds[3] = 1;
    for (i = 0; i < 4; i++) {
        if (((field_0x90 >> lbl_80465084[i]) & 1) == 1) {
            mKinds[mKindCount] = i;
            mKindCount++;
        }
    }
    mTimer = 150;
}
