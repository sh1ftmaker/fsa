#include "game/Map.h"

#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"

// The actor collision boxes: a function-local static pair of box lists, one
// written this frame and one read (see Map.h).
CollisionBoxes_8020D58C* fn_8020D58C() {
    static CollisionBoxes_8020D58C lbl_804CB8B0;
    return &lbl_804CB8B0;
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
// FIXME: 97.7 %. Registers only: the original keeps the box pointer, `found`
// and the counter in r26/r24/r23, ours in other callee-saved registers. The
// method is const (the original loads mCount above the prologue moves).
bool CollisionBoxList_8020D60C::fn_8020D6AC(s32 layer, const JGeometry::TVec3f* pos, s32 serial, const u32* ignoreIds) const {
    bool found = false;
    for (s32 i = 0; i < mCount; i++) {
        if (mBoxes[i].mLayer == layer && mBoxes[i].mSerial != serial && contains(mBoxes[i].mBox, *pos)) {
            found = true;
            if (ignoreIds != NULL) {
                ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(mBoxes[i].mSerial);
                if (actor == NULL) {
                    found = false;
                } else {
                    for (const u32* id = ignoreIds; *id != 'NULL'; id++) {
                        if (*id == actor->mActorId) {
                            found = false;
                            break;
                        }
                    }
                }
            }
            if (found) {
                break;
            }
        }
    }
    return found;
}

// Adds the box of the actor `serial` (moved by its position) if the actor is
// alive, the box is not empty and fewer than 0x50 boxes are listed.
// FIXME: 87.1 %. The original loads the actor's layer (field_0x4) before the
// count check and adds the box corners in another order; the rest is the same.
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
// FIXME: 89.3 %. The original keeps only box->i.x from the size test and
// loads i.y, f.x and f.y again for the copy; ours keeps all four in registers.
void CollisionBoxList_8020D60C::fn_8020D8C0(const JGeometry::TBox2f* box, s32 layer, s32 serial) {
    if (mCount < 0x50) {
        if (box->getWidth() > 0.0f && box->getHeight() > 0.0f) {
            mBoxes[mCount].mBox.set(*box);
            mBoxes[mCount].mLayer = layer;
            mBoxes[mCount].mSerial = serial;
            mCount++;
        }
    }
}
