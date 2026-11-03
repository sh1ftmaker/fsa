#include "game/GameAudio.h"

#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/act/tu_80207548.h"

// Frames before the actor starts, indexed by the number of players - 1.
static const u8 lbl_8053F8C8[4] = {2, 30, 30, 30};

static inline void normalize(JGeometry::TVec2f& v) {
    f32 sq = v.x * v.x + v.y * v.y;
    if (sq <= 32.0f * FLT_EPSILON) {
        return;
    }
    f32 norm = JGeometry::TUtil<f32>::inv_sqrt(sq);
    v.x *= norm;
    v.y *= norm;
}

// Marks `slot` done; once its player's slot is active and picked in mMask,
// aims the actor found by the slot's serial at that player when the player is
// on the actor's layer and vfunc_0x4C accepts the position. Reports the actor
// (fn_801F3668) when every other slot was already done.
// FIXME: 99.60%. Only the registers differ: the original keeps the slot
// offset (mulli) and the layer in r28, the player in r27 (ours: offset r29,
// player r28, layer r27); f-registers and the code are identical.
void cls_804A5B28::fn_802075D8(u16 slot) {
    mSlots[slot].mDone = true;
    bool all = true;
    for (s32 i = 0; i < 4; i++) {
        if (i != slot && !mSlots[i].mDone) {
            all = false;
        }
    }
    if (mActive && (mMask & (1 << slot))) {
        JGeometry::TVec3f v;
        JGeometry::TVec3f pos(mPos);
        ActorBase_804A5770* a = fn_801F856C()->fn_801F666C(mSlots[slot].mSerial);
        if (a) {
            pos.set(a->mPos);
        }
        s32 handle = fn_801CB434(field_0x4, &pos, true);
        s32 player = mSlots[slot].mPlayer;
        if (player >= 0 && player < 4) {
            s32 layer = field_0x4;
            if (layer == fn_8023DE58(player)) {
                if (vfunc_0x4C(field_0x4, &pos)) {
                    ActorBase_804A5770* b = fn_801F856C()->fn_801F666C(handle);
                    if (b) {
                        JGeometry::TVec2f d(fn_8023E724(player)->x - pos.x, fn_8023E724(player)->y - pos.y);
                        normalize(d);
                        d.scale(2.0f);
                        v.set(d.x, d.y, b->field_0x3c.z);
                        b->field_0x3c = v;
                    }
                }
            }
        }
    }
    if (all) {
        fn_801F3668();
    }
}

// Registers a request in `slot` (once). The first request sets mTimer from the
// player count; the request that completes the set (every other slot used)
// picks 1-3 random players into mMask and calls vfunc_0xA8.
// FIXME: 93.46%. The original guards the removal loop and the `if (found)`
// with `remaining > 0` (cmpwi once, two ble) and stores mActive from the same
// register as count (li r29, 1; stb r29); with those and registers r28 = this,
// r29 = count, r30 = remaining, r31 = i, r27 = &order (ours: r27 this,
// r28 i, r29 remaining, r30 count, r31 &order) the loop schedules the pointer
// copy (mr r6, r27) earlier.
void cls_804A5B28::fn_80207830(u16 slot, s32 player, s32 arg3, const JGeometry::TVec3f* target, s32 arg5) {
    if (mSlots[slot].mUsed) {
        return;
    }
    mSlots[slot].mUsed = true;
    mSlots[slot].mPlayer = player;
    mSlots[slot].field_0xc = arg3;
    mSlots[slot].mTarget.set(*target);
    mSlots[slot].field_0x1c = arg5;
    bool noneOthers = true;
    bool allOthers = true;
    for (s32 i = 0; i < 4; i++) {
        if (i != slot) {
            if (mSlots[i].mUsed) {
                noneOthers = false;
            } else {
                allOthers = false;
            }
        }
    }
    if (noneOthers) {
        mTimer = lbl_8053F8C8[lbl_8053AAF8->fn_80138A30() - 1];
    } else if (allOthers && mTimer) {
        s32 count = 1;
        mActive = true;
        switch (lbl_8053AAF8->fn_80138A30()) {
        case 3:
            count = 2;
            break;
        case 4:
            count = 3;
            break;
        }
        s32 order[4];
        for (s32 k = 0; k < 4; k++) {
            order[k] = k;
        }
        s32 remaining = 4;
        for (s32 i = 0; i < count; i++) {
            s32 pick = order[fn_801CD664(remaining)];
            mMask |= 1 << pick;
            bool found = false;
            for (s32 j = 0; j < remaining; j++) {
                if (!found) {
                    if (pick == order[j]) {
                        found = true;
                    }
                } else {
                    order[j - 1] = order[j];
                }
            }
            if (found) {
                remaining--;
            }
        }
        vfunc_0xA8();
    }
}

void cls_804A5B28::fn_80207A60() {
    if (mTimer != 0) {
        mTimer--;
        if (mTimer == 0 && !mActive) {
            vfunc_0xA4();
        }
    }
}

void cls_804A5B28::vfunc_0xA8() {}

void cls_804A5B28::vfunc_0xA4() {}

cls_804A5B28::cls_804A5B28() : mActive(false), mMask(0), mTimer(0), field_0x2ac(0) {}

cls_804A5B28::Slot::Slot() {
    mSerial = -1;
    mUsed = false;
    mPlayer = -1;
    field_0xc = 0;
    mTarget.set(0.0f, 0.0f, 0.0f);
    field_0x1c = 0;
    mDone = false;
}
