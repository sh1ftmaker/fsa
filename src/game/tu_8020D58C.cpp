#include "game/Map.h"

#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"

// The actor collision boxes: a function-local static pair of box lists, one
// written this frame and one read (see Map.h).
CollisionBoxes_8020D58C* fn_8020D58C(void) {
    static CollisionBoxes_8020D58C boxes;
    return &boxes;
}

CollisionBoxList_8020D60C::CollisionBoxList_8020D60C() {
    mCount = 0;
}

CollisionBox_8020D65C::CollisionBox_8020D65C() {}

// Swaps the read and write lists and empties the new write list.
void CollisionBoxes_8020D58C::fn_8020D660() {
    field_0x0 ^= 1;
    field_0x1 ^= 1;
    CollisionBoxList_8020D60C& list = mLists[field_0x0];
    list.mCount = 0;
}

void CollisionBoxes_8020D58C::fn_8020D690() {
    field_0x0 = 0;
    field_0x1 = 1;
    mLists[0].mCount = 0;
    mLists[1].mCount = 0;
}

static inline bool contains(const JGeometry::TBox2f& box, const JGeometry::TVec3f& p) {
    return p.x >= box.i.x && p.y >= box.i.y && p.x < box.f.x && p.y < box.f.y;
}

// Whether (x, y) lies in a box on `layer` registered by an actor other than
// `serial`. A box whose actor has one of the ids in `ignoreIds` (a list ending
// in 'NULL') doesn't count, and neither does a box whose actor is gone.
bool CollisionBoxList_8020D60C::fn_8020D6AC(s32 layer, const JGeometry::TVec3f* pos, s32 serial, const u32* ignoreIds) {
    bool found = false;
    s32 count = mCount;
    for (s32 i = 0; i < count; i++) {
        CollisionBox_8020D65C& box = mBoxes[i];
        if (box.mLayer == layer && box.mSerial != serial && contains(box.mBox, *pos)) {
            if (ignoreIds == NULL) {
                found = true;
                break;
            }
            ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(box.mSerial);
            if (actor != NULL) {
                bool ignored = false;
                for (const u32* id = ignoreIds; *id != 'NULL'; id++) {
                    if (actor->mActorId == *id) {
                        ignored = true;
                        break;
                    }
                }
                if (!ignored) {
                    found = true;
                    break;
                }
            }
        }
    }
    return found;
}

// Adds the box of the actor `serial` (moved by its position) if the actor is
// alive, the box is not empty and fewer than 0x50 boxes are listed.
void CollisionBoxList_8020D60C::fn_8020D7D0(s32 serial) {
    ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(serial);
    if (actor == NULL) {
        return;
    }
    if (!actor->mAlive) {
        return;
    }
    f32 x0 = actor->field_0x80.i.x + actor->mPos.x;
    f32 y0 = actor->field_0x80.i.y + actor->mPos.y;
    f32 x1 = actor->field_0x80.f.x + actor->mPos.x;
    f32 y1 = actor->field_0x80.f.y + actor->mPos.y;
    if (mCount >= 0x50) {
        return;
    }
    if (x1 - x0 > 0.0f && y1 - y0 > 0.0f) {
        CollisionBox_8020D65C& entry = mBoxes[mCount];
        entry.mBox.i.x = x0;
        entry.mBox.i.y = y0;
        entry.mBox.f.x = x1;
        entry.mBox.f.y = y1;
        entry.mLayer = actor->field_0x4;
        entry.mSerial = serial;
        mCount++;
    }
}

// Adds `box` on `layer` for `serial`, with the same checks as fn_8020D7D0.
void CollisionBoxList_8020D60C::fn_8020D8C0(const JGeometry::TBox2f* box, s32 layer, s32 serial) {
    if (mCount < 0x50) {
        if (box->f.x - box->i.x > 0.0f && box->f.y - box->i.y > 0.0f) {
            CollisionBox_8020D65C& entry = mBoxes[mCount];
            entry.mBox.i.x = box->i.x;
            entry.mBox.i.y = box->i.y;
            entry.mBox.f.x = box->f.x;
            entry.mBox.f.y = box->f.y;
            entry.mLayer = layer;
            entry.mSerial = serial;
            mCount++;
        }
    }
}
