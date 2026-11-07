// The actor manager (include/game/ActorManager.h) and its helpers.

#include "game/GameAudio.h"
#include "game/ActorId.h"
#include <float.h>
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/ViewLayer.h"
#include "JSystem/JKernel/JKRUnitHeap.h"
#include "game/Player.h"
#include "game/cls_804AE9B0.h"
#include "game/tu_801E9AFC.h"
#include "game/act/tu_8028DD78.h"
#include "game/PlayerBit.h"
#include "game/Map.h"
#include "game/tu_8021F064.h"

// game/tu_801A3C50.cpp (not written yet): nonzero while the actor is held (it then only
// runs vfunc_0x18 when its 0xA4 is set). Name and return type from the call in fn_801F871C.
s32 fn_801A6160(ActorBase_804A5770* actor);

// The position every per-slot sound object starts at (the constructor zeroes it).
static JGeometry::TVec3f lbl_804CABD8;

// Whether two boxes overlap (edges touching counts), on a copy of `a`: inlined
// the same way, with no store back to either box, into fn_801F7F58,
// fn_801F8108 and fn_801F82CC.
static inline bool boxesOverlap(const JGeometry::TBox2f& a, const JGeometry::TBox2f& b) {
    JGeometry::TBox2f overlap(a);
    return overlap.intersect(b);
}

// Whether p (x, y) is in box (min inclusive, max exclusive).
static inline bool contains(const JGeometry::TBox2f& box, const JGeometry::TVec3f& p) {
    return p.x >= box.i.x && p.y >= box.i.y && p.x < box.f.x && p.y < box.f.y;
}

// The angle of each direction (fn_801F6874; fn_801F681C is the inverse).
static const u16 lbl_8053F800[4] = {0x4000, 0xC000, 0x0000, 0x8000};

// The slot fn_801F78F4 found last.
static s32 lbl_8053A168 = -1;

// Deletes the actor in `slot` (releasing its sound object first) and, when it
// was the last used slot, moves mActorEnd down past the empty slots below it.
// Inlined into fn_801F857C, fn_801F871C and fn_801F8E4C.
inline void ActorManager_804A5800::deleteSlot(s32 slot) {
    if (mActors[slot] != NULL) {
        mSounds[slot]->fn_80456120();
        delete mActors[slot];
        mActors[slot] = NULL;
        if (slot + 1 == mActorEnd) {
            s32 i;
            for (i = mActorEnd - 2; i >= 0; i--) {
                if (mActors[i] != NULL) {
                    break;
                }
            }
            mActorEnd = i + 1;
        }
    }
}

// Forward declarations.
ActorManager_804A5800* getActorManager_801F856C(void);

// FIXME: 98.1 %, scheduling+regalloc: the target keeps `small` in r4 and the
// mode word in r5 (ours swaps them, so lbl_80529DEC's address is formed one
// slot later); the 0.0f literal is lbl_8053F828, the pool's last, until the
// functions before it are written.
ActorManager_804A5800::ActorManager_804A5800(JKRHeap* parent) {
    // Half the room in two UI modes.
    bool small = false;
    if ((lbl_80529DEC.field_0x118 == 9 && lbl_80529DEC.field_0x11c == 3) ||
        (lbl_80529DEC.field_0x118 == 8 && lbl_80529DEC.field_0x11c == 2))
    {
        small = true;
    }
    u32 units = small ? 0x200 : 0x400;
    mHeap = JKRUnitHeap::create(units, (units << 10) + 0x1a8, 0x20, parent, false);
    for (int i = 0; i < ACTOR_MANAGER_MAX_ACTORS; i++) {
        mActors[i] = NULL;
    }
    lbl_804CABD8.set(0.0f, 0.0f, 0.0f);
    for (u32 i = 0; i < ACTOR_MANAGER_MAX_ACTORS; i++) {
        mSounds[i] = new (parent, 0) ActorSoundObj_804578F4(&lbl_804CABD8, NULL, 8);
        mSounds[i]->fn_80456120();
    }
    field_0x200c = new (parent, 0) ActorSoundObj_804578F4(NULL, NULL, 0x18);
    fn_801F62AC(parent);
}

ActorManager_804A5800::~ActorManager_804A5800() {
    delete field_0x200c;
    for (s32 i = ACTOR_MANAGER_MAX_ACTORS - 1; i >= 0; i--) {
        delete mSounds[i];
    }
    for (s32 i = ACTOR_MANAGER_MAX_ACTORS - 1; i >= 0; i--) {
        if (mActors[i] != NULL) {
            delete mActors[i];
            mActors[i] = NULL;
        }
    }
    delete[] mParamRowIds;
    delete[] mParamRowNames;
    delete[] mParamColumnIds;
    delete[] mParamValues;
    mHeap->destroy();
}

// FIXME(shotgun): 96.6 %, missing/extra + reloc-name (fn_801A6160 named in C++)
// The per-frame update pass. Clears the per-frame serial list (field_0x201c), steps the
// collision box lists, then updates each live actor: with GameView's fn_802DCD0C only those
// flagged for it (0x11E / 0xC4 / 0xCC); otherwise all of them unless paused (0x11F keeps them
// going; 'SWDL' in GameRoot 0x88 mode 2), and while GameRoot 0x94 / 0x96 holds only the
// listed actor ids. Dead actors are deleted, the others checked against the collision boxes
// (vfunc_0x24). Then fn_801F6A4C, the list pass (actors not re-linked this frame are unlinked)
// and field_0x305c when no countable actor is left.
void ActorManager_804A5800::fn_801F871C() {
    s32 count = 0;
    fn_801F6068();
    field_0x3040 = 0;
    for (u32 i = 0; i < 0x400; i++) {
        field_0x201c[i] = -1;
    }
    field_0x301c = 0;
    bool paused = false;
    if ((gGameRoot_8053AAF8->field_0x7f != 0 && (fn_80239D38() || gGameRoot_8053AAF8->fn_80134CC8())) ||
        lbl_80529DEC.field_0x240 != 0)
    {
        paused = true;
    }
    u8 limited = gGameRoot_8053AAF8->field_0x94;
    if (gGameRoot_8053AAF8->field_0x96 != 0) {
        limited = 1;
    }
    fn_8020D58C()->fn_8020D660();
    fn_8021F064()->fn_8021F130();
    fn_80204638()->mCount = 0;
    if (!paused) {
        fn_801C0D70()->fn_801C0DCC();
    }
    if (getGameView_802D6A9C()->fn_802DCD0C()) {
        for (u32 i = 0; i < ACTOR_MANAGER_MAX_ACTORS; i++) {
            if (mActors[i] == NULL) {
                continue;
            }
            if (mActors[i]->mAlive) {
                bool update = false;
                if (mActors[i]->field_0x11e || mActors[i]->field_0xc4 || mActors[i]->field_0xcc) {
                    update = true;
                }
                if (update) {
                    mActors[i]->fn_801F32F4();
                    mActors[i]->field_0x1dc.fn_801EAB0C();
                    mActors[i]->savePrevPos();
                    mActors[i]->fn_801F33C4();
                    mActors[i]->update();
                    mActors[i]->fn_801EE508();
                }
                mSounds[i]->fn_80456154();
                if (mActors[i]->field_0x8 == 0 && mActors[i]->field_0x1a8 == true) {
                    count++;
                }
            }
            if (!mActors[i]->mAlive) {
                deleteSlot(i);
            }
        }
        field_0x200c->fn_80456154();
    } else {
        for (u32 i = 0; i < ACTOR_MANAGER_MAX_ACTORS; i++) {
            if (mActors[i] == NULL) {
                continue;
            }
            if (mActors[i]->mAlive) {
                bool skip = false;
                if (paused == true) {
                    skip = true;
                    if (mActors[i]->field_0x11f) {
                        skip = false;
                    }
                    if (mActors[i]->field_0x8 == 0 && mActors[i]->field_0x1a8 == true) {
                        count++;
                    }
                    if (gGameRoot_8053AAF8->field_0x88 == 2 && mActors[i]->mActorId == 'SWDL') {
                        skip = false;
                    }
                }
                if (!skip) {
                    u32 id;
                    if (!limited || (id = mActors[i]->mActorId) == 'FORM' || id == 'ALCE' ||
                        id == 'IMUP' || id == 'SWEV' || id == 'SEV2' || id == 'LTEV' ||
                        id == 'PRTC' || id == 'STPN' || id == 'TUGF' || id == 'TUTL' ||
                        id == 'TUTM' || id == 'PEFF' || id == 'SWDL')
                    {
                        mActors[i]->fn_801F32F4();
                        mActors[i]->field_0x1dc.fn_801EAB0C();
                        if (fn_801A6160(mActors[i]) == 0) {
                            mActors[i]->savePrevPos();
                            mActors[i]->fn_801F33C4();
                            mActors[i]->update();
                        } else if (mActors[i]->field_0xa4 != 0) {
                            mActors[i]->vfunc_0x18();
                        }
                        mActors[i]->fn_801EE508();
                    }
                    u8 kind = mActors[i]->field_0xb8;
                    if (kind == 1 || kind == 2) {
                        if (field_0x201c[field_0x301c] == -1) {
                            field_0x201c[field_0x301c] = mActors[i]->mSerial;
                            field_0x301c++;
                        }
                    }
                    mSounds[i]->fn_80456154();
                    if (mActors[i]->field_0x8 == 0 && mActors[i]->field_0x1a8 == true) {
                        count++;
                    }
                }
            }
            ActorBase_804A5770* actor = mActors[i];
            if (!actor->mAlive) {
                deleteSlot(i);
            } else if (actor->field_0x1a8 && !actor->field_0x196 &&
                       actor->field_0x80.f.x - actor->field_0x80.i.x > 0.0f &&
                       actor->field_0x80.f.y - actor->field_0x80.i.y > 0.0f && actor->mLife > 0)
            {
                s32 serial = actor->mSerial;
                s32 layer = actor->field_0x4;
                CollisionBoxes_8020D58C* boxes = fn_8020D58C();
                if (boxes->mLists[boxes->field_0x1].fn_8020D6AC(layer, &actor->mPos, serial, NULL)) {
                    mActors[i]->vfunc_0x24();
                }
            }
        }
        field_0x200c->fn_80456154();
    }
    fn_801F6A4C();
    ActorBase_804A5770* next;
    for (ActorBase_804A5770* actor = mListHead; actor != NULL; actor = next) {
        next = actor->mListNext;
        if (actor->mAlive) {
            if (!actor->mInList) {
                getActorManager_801F856C()->fn_801F8448(actor);
            } else {
                actor->mInList = 0;
            }
        }
    }
    if (count == 0) {
        getActorManager_801F856C()->fn_801F6204(1);
    }
}

void ActorManager_804A5800::fn_801F8E4C() {
    fn_801F857C();
    field_0x200c->setField_0x31();
    mSpawnCount = 0;
    mLastSlot = -1;
    mActorEnd = -1;
    getActorManager_801F856C()->fn_801F6204(0);
    mListHead = NULL;
    field_0x3041 = 0;
}

void ActorManager_804A5800::fn_801F8658() {
    bool paused = false;
    if (gGameRoot_8053AAF8->fn_80134CC8()) {
        paused = true;
    }
    for (u32 i = 0; i < ACTOR_MANAGER_MAX_ACTORS; i++) {
        if (mActors[i] != NULL && mActors[i]->mAlive && mActors[i]->mDrawEnabled &&
            (!paused || mActors[i]->field_0x11f))
        {
            mActors[i]->fn_801A3CFC();
            mActors[i]->draw();
        }
    }
    if (!paused) {
        fn_801E9AFC()->fn_801E9C74();
    }
}

// FIXME: 91.9 %. The target walks the slots with a pointer advanced by 4
// (lwz 0x8(r31)); this code recomputes this + 4 * i each pass (add r30), as
// the original does where this function is inlined into fn_801F8E4C.
void ActorManager_804A5800::fn_801F857C() {
    for (u32 i = 0; i < ACTOR_MANAGER_MAX_ACTORS; i++) {
        deleteSlot(i);
    }
    field_0x200c->fn_80456120();
}

ActorManager_804A5800* getActorManager_801F856C(void) {
    return gGameRoot_8053AAF8->field_0x4->mActorManager;
}

s32 ActorManager_804A5800::fn_801F8544(u32 id, s32 arg_0x4, const JGeometry::TVec3f* pos, u32 arg_0x90) {
    return spawnActor_801F9484(id, arg_0x4, pos, arg_0x90, -1, -1);
}

// FIXME: 93.8 %, scheduling: the target loads last->mListNext before
// comparing last with actor; this code loads it after the compare.
s32 ActorManager_804A5800::fn_801F84C4(ActorBase_804A5770* actor) {
    u8 linked = false;
    if (mListHead != NULL) {
        ActorBase_804A5770* last = mListHead;
        while (true) {
            if (last == actor) {
                actor->mInList = true;
                break;
            }
            if (last->mListNext == NULL) {
                break;
            }
            last = last->mListNext;
        }
        if (last != actor) {
            last->mListNext = actor;
            actor->mListPrev = last;
            actor->mInList = true;
            linked = true;
        }
    } else {
        mListHead = actor;
        actor->mListPrev = NULL;
        actor->mListNext = NULL;
        actor->mInList = true;
        linked = true;
    }
    return linked;
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

s32 ActorManager_804A5800::fn_801F82CC(ActorBase_804A5770* actor, u8 includeArij, f32 maxDz) {
    JGeometry::TBox2f otherBox;
    JGeometry::TBox2f box;
    s32 found = -1;
    actor->fn_801F31C4(&box);
    u8 anyZ = viewLayerFlag(actor->field_0x4);
    for (ActorBase_804A5770* other = mListHead; other != NULL; other = other->mListNext) {
        if (!other->mAlive || other->field_0x4 != actor->field_0x4 || other->field_0xc4) {
            continue;
        }
        if (!includeArij && other->mActorId == ACTOR_ARIJ) {
            continue;
        }
        if (!anyZ && __fabsf(actor->mPos.z - other->mPos.z) > maxDz) {
            continue;
        }
        other->fn_801F31C4(&otherBox);
        if (boxesOverlap(otherBox, box)) {
            found = other->mSerial;
            break;
        }
    }
    return found;
}

extern "C" s32 fn_801F8108(const ActorBase_804A5770* actor, u32 id, s32 excludeSerial, f32 maxDz) {
    u32 ids[2];
    ids[0] = id;
    ids[1] = ACTOR_NULL;
    return getActorManager_801F856C()->fn_801F7F58(const_cast<ActorBase_804A5770*>(actor), ids,
                                                  excludeSerial, maxDz);
}

s32 ActorManager_804A5800::fn_801F7F58(ActorBase_804A5770* actor, const u32* ids, s32 excludeSerial,
                                       f32 maxDz) const {
    s32 found = -1;
    u8 anyZ = viewLayerFlag(actor->field_0x4);
    for (s32 i = 0; i < mActorEnd; i++) {
        ActorBase_804A5770* other = mActors[i];
        if (other == NULL || !other->mAlive || other->field_0x4 != actor->field_0x4 ||
            excludeSerial == other->mSerial)
        {
            continue;
        }
        if (!anyZ && __fabsf(actor->mPos.z - other->mPos.z) > maxDz) {
            continue;
        }
        for (const u32* id = ids; *id != ACTOR_NULL; id++) {
            if (*id == other->mActorId) {
                JGeometry::TBox2f actorBox;
                JGeometry::TBox2f otherBox;
                actor->fn_801F31C4(&actorBox);
                other->fn_801F31C4(&otherBox);
                if (boxesOverlap(actorBox, otherBox)) {
                    found = other->mSerial;
                    break;
                }
            }
        }
        if (found >= 0) {
            break;
        }
    }
    return found;
}

void ActorManager_804A5800::fn_801F7E88() {
    GameView_804A9950* view = getGameView_802D6A9C();
    s32 end = mActorEnd;
    for (s32 i = 0; i < end; i++) {
        ActorBase_804A5770* actor = mActors[i];
        if (actor != NULL && actor->mAlive &&
            (actor->field_0x4 != 0 || (actor->field_0x194 && !contains(view->mVisibleBox, actor->mPos))))
        {
            actor->mAlive = false;
        }
    }
}

// FIXME(shotgun): 98.7 %, regalloc-only
void ActorManager_804A5800::fn_801F7CAC() {
    GameView_804A9950* view = getGameView_802D6A9C();
    for (s32 i = 0; i < 4; i++) {
        getGameView_802D6A9C()->fn_802D39B8(i, -1);
        if (fn_8023EBB8(i)) {
            s32 v = fn_8023EF50(i);
            getGameView_802D6A9C()->fn_802D39B8(i, v);
        }
    }
    s32 end = mActorEnd;
    for (s32 i = 0; i < end; i++) {
        ActorBase_804A5770* actor = mActors[i];
        if (actor == NULL || !actor->mAlive || actor->field_0x4 != 0) {
            continue;
        }
        if (actor->mActorId == ACTOR_TZOK || actor->mActorId == ACTOR_FAZR ||
            actor->mActorId == ACTOR_DMGA) {
            continue;
        }
        if (contains(view->mVisibleBox, actor->mPos)) {
            continue;
        }
        s32 serial = actor->mSerial;
        if (serial == getGameView_802D6A9C()->field_0x131a8[0] ||
            serial == getGameView_802D6A9C()->field_0x131a8[1] ||
            serial == getGameView_802D6A9C()->field_0x131a8[2] ||
            serial == getGameView_802D6A9C()->field_0x131a8[3]) {
            continue;
        }
        actor = mActors[i];
        if (actor->mActorId == ACTOR_RANK || actor->mActorId == ACTOR_GBAO) {
            continue;
        }
        if (actor->mActorId == ACTOR_RASN) {
            if (static_cast<cls_804A7A68*>(actor)->field_0x230 == 0) {
                actor->mAlive = false;
            }
        } else {
            actor->mAlive = false;
        }
    }
    fn_80204638()->fn_802048F4();
}

// FIXME(shotgun): 94.1 %, missing/extra
void ActorManager_804A5800::fn_801F7B18() {
    for (s32 i = 0; i < 4; i++) {
        getGameView_802D6A9C()->fn_802D39B8(i, -1);
        if (fn_8023EBB8(i)) {
            s32 v = fn_8023EF50(i);
            getGameView_802D6A9C()->fn_802D39B8(i, v);
        }
    }
    s32 end = mActorEnd;
    for (s32 i = 0; i < end; i++) {
        ActorBase_804A5770* actor = mActors[i];
        if (actor == NULL || !actor->mAlive) {
            continue;
        }
        if (actor->mActorId == ACTOR_TZOK) {
            continue;
        }
        s32 serial = actor->mSerial;
        if (serial == getGameView_802D6A9C()->field_0x131a8[0] ||
            serial == getGameView_802D6A9C()->field_0x131a8[1] ||
            serial == getGameView_802D6A9C()->field_0x131a8[2] ||
            serial == getGameView_802D6A9C()->field_0x131a8[3]) {
            continue;
        }
        actor = mActors[i];
        if (actor->mActorId == ACTOR_RANK || actor->mActorId == ACTOR_GBAO ||
            actor->mActorId == ACTOR_ENDM || actor->mActorId == ACTOR_DMGA) {
            continue;
        }
        if (actor->mActorId == ACTOR_RASN) {
            if (static_cast<cls_804A7A68*>(actor)->field_0x230 == 0) {
                actor->mAlive = false;
            }
        } else {
            actor->mAlive = false;
        }
    }
    fn_80204638()->fn_802048F4();
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

s32 ActorManager_804A5800::fn_801F7A6C() {
    s32 count = 0;
    for (s32 i = 0; i < mActorEnd; i++) {
        if (mActors[i] != NULL && mActors[i]->mAlive && mActors[i]->field_0x1a8) {
            count++;
        }
    }
    return count;
}

s32 ActorManager_804A5800::fn_801F7A08(u32 id) {
    for (s32 i = 0; i < mActorEnd; i++) {
        if (mActors[i] != NULL && mActors[i]->mAlive && id == mActors[i]->mActorId) {
            return mActors[i]->mSerial;
        }
    }
    return -1;
}

s32 ActorManager_804A5800::fn_801F78F4(u32 id, s32 afterSerial) {
    s32 start = 0;
    if (afterSerial >= 0) {
        if (lbl_8053A168 >= 0 && mActors[lbl_8053A168] != NULL && mActors[lbl_8053A168]->mAlive &&
            afterSerial == mActors[lbl_8053A168]->mSerial)
        {
            start = lbl_8053A168 + 1;
        } else {
            for (s32 i = 0; i < mActorEnd; i++) {
                if (mActors[i] != NULL && mActors[i]->mAlive && afterSerial == mActors[i]->mSerial) {
                    start = i + 1;
                    break;
                }
            }
        }
    }
    for (s32 i = start; i < mActorEnd; i++) {
        if (mActors[i] != NULL && mActors[i]->mAlive && id == mActors[i]->mActorId) {
            lbl_8053A168 = i;
            return mActors[i]->mSerial;
        }
    }
    lbl_8053A168 = -1;
    return -1;
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

// FIXME: 98.0 %, scheduling+regalloc: the target loads pos->x, actor x, pos->y,
// actor y and squares x first; this code loads and squares y first. The
// 1000000.0f literal is lbl_8053F824 once the pool before it is complete.
s32 ActorManager_804A5800::fn_801F778C(u32 id, const JGeometry::TVec3f* pos, f32* outDistSq) {
    s32 serial = -1;
    f32 best = 1000000.0f;
    for (s32 i = 0; i < mActorEnd; i++) {
        if (mActors[i] != NULL && mActors[i]->mAlive && id == mActors[i]->mActorId) {
            f32 dist = square(pos->x - mActors[i]->mPos.x) + square(pos->y - mActors[i]->mPos.y);
            if (dist < best) {
                serial = mActors[i]->mSerial;
                best = dist;
            }
        }
    }
    if (outDistSq != NULL) {
        *outDistSq = best;
    }
    return serial;
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

// FIXME: 98.0 %, scheduling+regalloc: the target loads pos->x, actor x, pos->y,
// actor y and squares x first; this code loads and squares y first. The
// 1000000.0f literal is lbl_8053F824 once the pool before it is complete.
s32 ActorManager_804A5800::fn_801F7664(const JGeometry::TVec3f* pos, f32* outDistSq) {
    s32 serial = -1;
    f32 best = 1000000.0f;
    for (s32 i = 0; i < mActorEnd; i++) {
        if (mActors[i] != NULL && mActors[i]->mAlive && mActors[i]->field_0x1a8 == true) {
            f32 dist = square(pos->x - mActors[i]->mPos.x) + square(pos->y - mActors[i]->mPos.y);
            if (dist < best) {
                serial = mActors[i]->mSerial;
                best = dist;
            }
        }
    }
    if (outDistSq != NULL) {
        *outDistSq = best;
    }
    return serial;
}

u8 ActorManager_804A5800::fn_801F75B4(s32 layer, const JGeometry::TBox2f* box) {
    BOOL clear = TRUE;
    for (s32 i = 0; i < mActorEnd; i++) {
        if (mActors[i] != NULL && mActors[i]->mAlive == true && layer == mActors[i]->field_0x4 &&
            mActors[i]->field_0x1a8 == true && contains(*box, mActors[i]->mPos) == true)
        {
            clear = FALSE;
            break;
        }
    }
    return clear;
}

s32 ActorManager_804A5800::fn_801F750C(s32 layer, s32 excludeSerial) const {
    s32 serial = -1;
    for (s32 i = 0; i < mActorEnd; i++) {
        if (mActors[i] != NULL && mActors[i]->mAlive && layer == mActors[i]->field_0x4 &&
            excludeSerial != mActors[i]->mSerial && fn_801CC910(mActors[i]))
        {
            serial = mActors[i]->mSerial;
            break;
        }
    }
    return serial;
}

s32 ActorManager_804A5800::fn_801F7464(s32 layer, s32 excludeSerial) const {
    s32 serial = -1;
    for (s32 i = 0; i < mActorEnd; i++) {
        if (mActors[i] != NULL && mActors[i]->mAlive && layer == mActors[i]->field_0x4 &&
            excludeSerial != mActors[i]->mSerial && fn_801CC960(mActors[i]))
        {
            serial = mActors[i]->mSerial;
            break;
        }
    }
    return serial;
}

// FIXME: 99.6 %, regalloc only: the two y/x differences sit in f3/f2 in the
// target, f2/f3 here; the 16.0f literal is lbl_8053F820 once the pool before
// it is complete.
s32 ActorManager_804A5800::fn_801F732C(s32 layer, const JGeometry::TVec3f* center,
                                       const JGeometry::TVec3f* from, s32 excludeSerial,
                                       f32 radius) const {
    f32 radiusSq = radius * radius;
    f32 best = FLT_MAX;
    s32 serial = -1;
    for (s32 i = 0; i < mActorEnd; i++) {
        if (mActors[i] != NULL && mActors[i]->mAlive && layer == mActors[i]->field_0x4 &&
            excludeSerial != mActors[i]->mSerial && mActors[i]->mPos.z <= 16.0f &&
            fn_801CC9B0(mActors[i]) &&
            square(mActors[i]->mPos.x - center->x) + square(mActors[i]->mPos.y - center->y) <= radiusSq)
        {
            f32 dist = square(mActors[i]->mPos.x - from->x) + square(mActors[i]->mPos.y - from->y);
            if (dist < best) {
                best = dist;
                serial = mActors[i]->mSerial;
            }
        }
    }
    return serial;
}

s32 ActorManager_804A5800::fn_801F7274(s32 layer, ActorBase_804A5770** out, s32 excludeSerial) const {
    s32 count = 0;
    for (s32 i = 0; i < mActorEnd; i++) {
        if (mActors[i] != NULL && mActors[i]->mAlive && layer == mActors[i]->field_0x4 &&
            excludeSerial != mActors[i]->mSerial && mActors[i]->mPos.z <= 16.0f &&
            fn_801CC9B0(mActors[i]))
        {
            *out++ = mActors[i];
            count++;
        }
    }
    return count;
}

// FIXME: 99.3 %, regalloc only (register numbering of the slot pointer and
// the loaded id).
bool ActorManager_804A5800::fn_801F71E8(u32 group, ActorBase_804A5770** out, s32* count, s32 max) {
    s32 end = mActorEnd;
    *count = 0;
    for (s32 i = 0; i < end; i++) {
        if (mActors[i] != NULL && mActors[i]->mAlive && mActors[i]->mActorId == ACTOR_RAIL &&
            group == ((mActors[i]->field_0x90 >> 8) & 0xFF))
        {
            out[*count] = mActors[i];
            (*count)++;
            if (*count == max) {
                return false;
            }
        }
    }
    return true;
}

void addCalc2_801F71A4(f32* value, f32 target, f32 rate, f32 maxStep) {
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

// FIXME: 95.2 %. In the last branch the target zero-extends delta (clrlwi 16)
// before adding it to *value; this code adds the s16 directly.
void addCalcAngle2_801F714C(u16* value, u16 target, u16 step, u16 maxStep) {
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

// FIXME(shotgun): 67.3 %, missing/extra + reloc-name
// The pairwise push pass over the actors fn_801F871C listed in field_0x201c: each pair on the
// same layer whose boxes overlap (and the second's 0xBC is not lower) records the other's
// serial in field_0x9c. Kind 2 (0xB8) only records it; the others are pushed out of the overlap
// in random steps away from the other actor (up to 8 tries through fn_801F23BC), 'BOYO'
// also knocking the actor back at 5 and flagging the BOYO's 0x94.
void ActorManager_804A5800::fn_801F6A4C() {
    for (s32 i = 0; i <= field_0x301c; i++) {
        if (field_0x201c[i] == -1) {
            return;
        }
        ActorBase_804A5770* a = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x201c[i]);
        if (a == NULL || !a->mAlive) {
            continue;
        }
        JGeometry::TBox2f boxA = a->mBox;
        boxA.addPos(a->mPos.x, a->mPos.y);
        a->field_0x9c = -1;
        for (s32 j = 0; j < field_0x301c; j++) {
            ActorBase_804A5770* b =
                getActorManager_801F856C()->findActorBySerial_801F666C(field_0x201c[j]);
            if (b == NULL || !b->mAlive) {
                continue;
            }
            if (a->field_0x4 != b->field_0x4 || a->mSerial == b->mSerial ||
                a->field_0xbc > b->field_0xbc)
            {
                continue;
            }
            JGeometry::TBox2f boxB = b->mBox;
            boxB.addPos(b->mPos.x, b->mPos.y);
            if ((u8)a->field_0xb8 == 2) {
                JGeometry::TBox2f overlap = boxA;
                if (overlap.intersect(boxB)) {
                    a->field_0x9c = b->mSerial;
                    j = 0x400;
                }
                continue;
            }
            s32 tries = 0;
            while (true) {
                JGeometry::TBox2f overlap = boxA;
                if (!overlap.intersect(boxB) || tries >= 8) {
                    break;
                }
                a->field_0x9c = b->mSerial;
                JGeometry::TVec3f pos = a->mPos;
                f32 dx = a->mPos.x - b->mPos.x;
                f32 dy = a->mPos.y - b->mPos.y;
                boxA = a->mBox;
                s32 angle = (s32)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f) +
                            (s32)(8192.0f - 16384.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
                pos.x += JMath::sincosTable_.sinShort(angle);
                pos.y += JMath::sincosTable_.cosShort(angle);
                if (b->mActorId == 'BOYO') {
                    f32 z = a->field_0x3c.z;
                    f32 c = JMath::sincosTable_.cosShort(
                        (s32)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f));
                    f32 s = JMath::sincosTable_.sinShort(
                        (s32)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f));
                    JGeometry::TVec3f v(5.0f * s, 5.0f * c, z);
                    a->field_0x3c = v;
                    b->field_0x94 = 1;
                }
                if (!a->fn_801F23BC(a->field_0x4, &pos, &a->mPos, &a->field_0x80, NULL, NULL, false)) {
                    tries++;
                    a->mPos = pos;
                    boxA.addPos(a->mPos.x, a->mPos.y);
                } else {
                    tries = 100;
                }
            }
        }
    }
}

s32 ActorManager_804A5800::fn_801F68E0() {
    cls_804A03A8* obj = gGameRoot2_8053AB10->field_0x30;
    if (!obj->field_0x284) {
        obj->field_0x284 = 1;
        obj->field_0x28c = 0.05f;
    }
    s32 busy = 0;
    u8 present = 0;
    for (s32 i = 0; i < 4; i++) {
        if (fn_8023F264(i) && fn_8023ED8C(i)) {
            fn_80230C6C(i);
            busy++;
        }
    }
    if (busy != 0) {
        return 0;
    }
    for (s32 i = 0; i < 4; i++) {
        if (fn_8023F264(i)) {
            present |= 1 << i;
        }
    }
    for (s32 i = 0; i < 4; i++) {
        if (fn_8023F264(i) && fn_802300EC(i) == true && !fn_8023E854(i)) {
            fn_80230AD4(i);
            field_0x3041 |= 1 << i;
        }
    }
    if (field_0x3041 == present) {
        lbl_8053AD70->fn_8037AE00();
        return 1;
    }
    return 0;
}

void ActorManager_804A5800::fn_801F6884() {
    for (s32 i = 0; i < 4; i++) {
        fn_80230224(i, false);
    }
    field_0x3041 = 0;
    lbl_8053AD70->fn_8037AE30();
}

u16 fn_801F6874(s32 dir) {
    return lbl_8053F800[dir & 3];
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

bool fn_801F6758(const JGeometry::TVec3f* pos, s32 layer, f32 margin) {
    GameView_804A9950* view = getGameView_802D6A9C();
    const JGeometry::TBox2f* box = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
    if (pos->x <= box->i.x - margin || pos->x >= box->f.x + margin ||
        pos->y <= box->i.y - margin || pos->y >= box->f.y + margin)
    {
        return true;
    }
    return false;
}

s32 ActorManager_804A5800::fn_801F6718(s32 a, s32 layer, const JGeometry::TVec3f* pos, s32 b, s32 c) {
    return spawnActor_801F9484(ACTOR_SCRE, layer, pos, (u16)b | ((u8)a << 16) | ((u8)c << 24), -1, -1);
}

ActorBase_804A5770* ActorManager_804A5800::findActorBySerial_801F666C(s32 serial) {
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

ActorSoundObj_804578F4* ActorManager_804A5800::fn_801F6644(s32 slot) {
    if (mActors[slot]->field_0x4 == 0) {
        return mSounds[slot];
    }
    return field_0x200c;
}

// FIXME(shotgun): 96 %, missing/extra (one extra move of the resource pointer)
void ActorManager_804A5800::fn_801F62AC(JKRHeap* heap) {
    char* data = (char*)gGameRoot_8053AAF8->field_0xa4->getResource('ENM ', "EnemyParam.csv");
    s32 size = gGameRoot_8053AAF8->field_0xa4->getResSize(
        gGameRoot_8053AAF8->field_0xa4->getResource('ENM ', "EnemyParam.csv"));

    // The header row gives the column count, the newlines the row count.
    mParamColumnNum = 0;
    for (char* p = data;; p++) {
        if (*p == ',') {
            mParamColumnNum++;
        } else if (*p == '\n') {
            break;
        }
    }
    mParamColumnNum--;
    mParamRowNum = 0;
    for (s32 i = 0; i <= size; i++) {
        if ((u8)data[i] == '\n') {
            mParamRowNum++;
        }
    }
    mParamRowNum--;

    mParamRowIds = new (heap, 0) u32[(mParamRowNum + 0x1f) & ~0x1f];
    mParamRowNames = new (heap, 0) char*[(mParamRowNum + 0x1f) & ~0x1f];
    mParamColumnIds = new (heap, 0) u32[(mParamColumnNum + 0x1f) & ~0x1f];
    mParamValues = new (heap, 0) s32[(mParamRowNum * mParamColumnNum + 0x1f) & ~0x1f];

    // Header: the first two fields are the id and name columns, the rest are 4-character keys.
    s32 fields = 0;
    s32 start = 0;
    for (s32 i = 0;; i++) {
        char c = data[i];
        if (c == ',' || c == '\n') {
            if (fields > 1) {
                mParamColumnIds[fields - 2] = ((u8)data[start] << 24) | ((u8)data[start + 1] << 16) |
                                              ((u8)data[start + 2] << 8) | (u8)data[start + 3];
            }
            if (c != ',') {
                break;
            }
            fields++;
            start = i + 1;
        }
    }

    // Rows: column 0 is the ActorId, column 1 the name, the others decimal values.
    s32 col = 0;
    s32 row = 0;
    start = 0;
    char* p = data;
    for (s32 i = 0; i <= size; i++, p++) {
        char c = *p;
        if (c == ',' || c == '\n' || c == 0) {
            if (row > 0 && (c != 0 || col >= 2)) {
                if (col == 0) {
                    mParamRowIds[row - 1] = ((u8)data[start] << 24) | ((u8)data[start + 1] << 16) |
                                            ((u8)data[start + 2] << 8) | (u8)data[start + 3];
                } else if (col == 1) {
                    mParamRowNames[row - 1] = data + start;
                    *p = 0;
                } else {
                    s32 value = 0;
                    for (s32 j = start; j < i; j++) {
                        char ch = data[j];
                        if (ch >= '0' && ch <= '9') {
                            value = value * 10 + (ch - '0');
                        }
                    }
                    mParamValues[(row - 1) * mParamColumnNum + col - 2] = value;
                }
            }
            if (c == ',' || c == 0) {
                col++;
                start = i + 1;
            } else if (c == '\n') {
                start = i + 1;
                col = 0;
                row++;
            }
        }
    }
}

// FIXME: 96.4 %, scheduling+regalloc: the target keeps the column count in r7,
// value in r8, col in r9 and the key pointer in r6 (ours: r6, r7, r8, r9),
// so the two increments come out in the other order.
s32 ActorManager_804A5800::fn_801F620C(u32 id, u32 column) {
    s32 value = 0;
    s32 col = 0;
    u32* key = mParamColumnIds;
    for (; col < mParamColumnNum; col++, key++) {
        if (column == *key) {
            break;
        }
    }
    if (col < mParamColumnNum) {
        s32 row = 0;
        for (u32* rowId = mParamRowIds; row < mParamRowNum; row++, rowId++) {
            if (id == *rowId) {
                value = mParamValues[row * mParamColumnNum + col];
                break;
            }
        }
        // No row for `id`: row 0's default.
        if (row == mParamRowNum) {
            value = mParamValues[col];
        }
    }
    return value;
}

void ActorManager_804A5800::fn_801F6204(u8 value) {
    field_0x305c = value;
}

// FIXME(shotgun): 99.2 %, regalloc-only
void ActorManager_804A5800::fn_801F6068() {
    s32 count = 0;
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerBitSet(gGameRoot_8053AAF8, i)) {
            field_0x3020[i] = fn_80236228(i);
            field_0x3030[i] = i;
            count++;
        } else {
            field_0x3020[i] = -9999;
            field_0x3030[i] = 3;
        }
    }
    // Selection sort, highest value first.
    for (s32 i = 0; i < count - 1; i++) {
        s32 best = i;
        s32 v = field_0x3020[i];
        for (s32 j = i + 1; j < count; j++) {
            if (v < field_0x3020[j]) {
                field_0x3020[best] = field_0x3020[j];
                field_0x3020[j] = v;
                v = field_0x3020[best];
                s32 p = field_0x3030[j];
                field_0x3030[j] = field_0x3030[best];
                field_0x3030[best] = p;
                best = j;
            }
        }
    }
    // The entries past the last player repeat it.
    if (count < 4) {
        s32 lastValue = field_0x3020[count - 1];
        s32 lastPlayer = field_0x3030[count - 1];
        for (s32 i = count; i < 4; i++) {
            field_0x3020[i] = lastValue;
            field_0x3030[i] = lastPlayer;
        }
    }
}

s32 ActorManager_804A5800::fn_801F6058(s32 i) {
    return field_0x3030[i];
}
