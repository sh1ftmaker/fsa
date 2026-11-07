#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameRootSub20.h"
#include "game/ActorManager.h"
#include "game/GameMath.h"
#include "game/act/act_quicksand.h"
#include "game/act/tu_80212E3C.h"
#include "game/tu_80212AC0.h"

// Ends the active state: state 1 hands the actor to fn_80212ECC and the view
// bit on, state 6 clears it; then the actor gets mFlags 0x4000 and the state
// becomes 6.
static inline void leaveSand(cls_804A6270* sand) {
    switch (sand->mState) {
    case 1: {
        s32 serial = sand->field_0x388;
        if (serial >= 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != NULL) {
                static_cast<cls_804A6340*>(actor)->fn_80212ECC();
            }
        }
        s32 viewSerial = sand->field_0x2b8;
        if (viewSerial != 0) {
            getGameView_802D6A9C()->fn_802D8050(viewSerial, true, 0xff);
        }
        break;
    }
    case 6: {
        s32 viewSerial = sand->field_0x2b8;
        if (viewSerial != 0) {
            getGameView_802D6A9C()->fn_802D8050(viewSerial, false, 0xff);
        }
        break;
    }
    }
    s32 serial = sand->field_0x388;
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            static_cast<EnemyBase_804A4808*>(actor)->mFlags |= 0x4000;
        }
    }
    sand->mStateTimer = 0;
    sand->mState = 6;
}

cls_804A6270::cls_804A6270() {
    field_0x2a8 = 0;
    field_0x2ac = 0;
    field_0x2b0 = 0;
    field_0x2b4 = 0;
    field_0x2b8 = 0;
    field_0x2bc = 0;
    field_0x388 = -1;
    field_0x38c = -1;
    field_0x390 = 0.0f;
    field_0x394 = 0.0f;
    field_0x398 = 0.0f;
    field_0x39c = 0.0f;
    field_0x3a0 = 0.0f;
    field_0x3a4 = 0.0f;
    field_0x3a8 = 1.0f;
    field_0x3ac = -1;
    field_0x3b0 = 0;
    field_0x2c0.clear();
}

cls_804A6270::~cls_804A6270() {
    fn_80212AC0()->fn_80212BA0(mSerial);
}

// FIXME(shotgun): not measurable (the original symbol is the plain fn_802121C8)
void cls_804A6270::fn_802121C8() {
    if (!(mFlags & 0x2000)) {
        field_0x38c = getActorManager_801F856C()->fn_801F76F0('WARP', field_0x4, &mPos, -1);
        s32 serial = field_0x38c;
        if (serial >= 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor == NULL) {
                field_0x38c = -1;
            } else {
                f32 dx = actor->mPos.x - mPos.x;
                f32 dy = actor->mPos.y - mPos.y;
                if (square(dx) + square(dy) > 256.0f) {
                    field_0x38c = -1;
                }
            }
        }
    }
}

void cls_804A6270::vfunc_0x54() {
    if (mFlags & 0x8000 && field_0x2bc != 0) {
        field_0x2c0.fn_800FE3EC(field_0x2bc, mPos, field_0x4);
    }
}

// FIXME(shotgun): not measurable (the original symbol is the plain fn_80211EB0)
void cls_804A6270::fn_80211EB0() {
    if (mState != 0) {
        for (s32 i = 0; i < 4; i++) {
            if (fn_801D0B44(i)) {
                if (fn_80211000(getPlayerPos_8023E724(i))) {
                    if (!(fn_802373EC(i) == 1 && fn_8023725C(i) == 2 && fn_8023513C(i))) {
                        JGeometry::TVec3f push;
                        fn_80210E90(getPlayerPos_8023E724(i), &push);
                        fn_8023DFF4(i, &push);
                        fn_8022CD48(i);
                        fn_80230FD0(i, 0x20045);
                    }
                }
            }
        }
    }
}

void cls_804A6270::fn_80211C94() {
    // Only in states 2 to 5 (a switch: `mState >= 2 && mState < 6` orders the
    // branches differently).
    switch (mState) {
    case 2:
    case 3:
    case 4:
    case 5:
        if (field_0x2ac == 3) {
            s32 viewBit = field_0x2b0;
            if (viewBit != 0 && !getGameView_802D6A9C()->fn_802D800C(viewBit)) {
                leaveSand(this);
                return;
            }
        }
        if (field_0x2ac == 4 && fn_801D05F8(&mPos, field_0x3a0) < 0) {
            leaveSand(this);
        }
        break;
    }
}

void cls_804A6270::vfunc_0x9C(u8 mapHit) {
    EnemyBase_804A4808::vfunc_0x9C(mapHit);
    bool active = false;
    if (mState != 0 && field_0x54.x != 0.0f && field_0x398 != 0.0f) {
        active = true;
    }
    if (active) {
        // Read before the fn_801902C0 call (kept in f29-f31 across it); the
        // declaration order gives the original's register assignment (as the
        // stack-slot rule in RULES_CARD; read in the natural order: 99.74 %).
        f32 scaleX;
        f32 field398;
        f32 field3a8;
        field3a8 = field_0x3a8;
        field398 = field_0x398;
        scaleX = field_0x54.x;
        GameRootSub20_801902C0* shared = fn_801902C0();
        if (shared->mQuicksandCount < 10) {
            GameRootSub20_801902C0::QuicksandEntry& entry = shared->mQuicksandEntries[shared->mQuicksandCount];
            entry.mPos.x = mPos.x;
            entry.mPos.y = mPos.y;
            entry.mPos.z = mPos.z;
            entry.field_0xc = scaleX;
            entry.field_0x10 = field398;
            entry.field_0x14 = field3a8;
            shared->mQuicksandCount++;
        }
    }
    active = false;
    if (mState != 0 && field_0x54.x != 0.0f && field_0x398 != 0.0f) {
        active = true;
    }
    if (active) {
        startSound_801F0E34(0x30073, 0);
    }
}

// FIXME(shotgun): 90.16 %, missing/extra
void cls_804A6270::fn_802110A8() {
    if ((mFlags & 0x8000) && mState != 0) {
        if (field_0x2bc != 0) {
            JGeometry::TVec3f pos(mPos);
            field_0x2c0.fn_800FDEA0(&pos, 1.0f, &field_0x3c, NULL, NULL, 0x200);
        } else {
            bool inside = false;
            for (s32 i = 0; i < 4; i++) {
                if (fn_801D0B44(i) && fn_80211000(getPlayerPos_8023E724(i))) {
                    inside = true;
                    break;
                }
            }
            if (inside) {
                field_0x3ac = -1;
                field_0x3b0 = 0;
                return;
            }
            if (field_0x3b0 != 0) {
                field_0x3b0--;
            }
            s32 player = fn_801D05F8(&mPos, 0.0f);
            if (player != field_0x3ac) {
                if (player < 0) {
                    field_0x3ac = -1;
                    field_0x3b0 = 0;
                } else if (field_0x3ac < 0) {
                    field_0x3ac = player;
                    field_0x3b0 = 45;
                } else if (field_0x3b0 == 0) {
                    field_0x3ac = player;
                    field_0x3b0 = 45;
                }
            }
            if (field_0x3ac >= 0) {
                JGeometry::TVec2f step;
                fn_801D00B4(field_0x3ac, &step, 1.0f);
                field_0x3c.x = step.x;
                field_0x3c.y = step.y;
            }
        }
        if (field_0x388 >= 0) {
            if (!(field_0x3c.squared() <= 32.0f * JGeometry::TUtilf::epsilon())) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x388);
                if (actor != NULL) {
                    actor->mPos += field_0x3c;
                }
            }
        }
    }
}

void cls_804A6270::vfunc_0x88() {
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    fn_802110A8();
}

// Whether a position at or below the ground lies inside the quicksand's area:
// within 0.9 * (16 * field_0x54.x) of mPos in x and y, once it is active.
bool cls_804A6270::fn_80211000(const JGeometry::TVec3f* pos) const {
    bool inside = false;
    if (pos->z <= 0.0f && mState != 0) {
        f32 radius = 0.9f * (16.0f * field_0x54.x);
        f32 dx = pos->x - mPos.x;
        f32 dy = pos->y - mPos.y;
        if (square(dx) + square(dy) <= square(radius)) {
            inside = true;
        }
    }
    return inside;
}

// The push-out vector for a position in the quicksand: towards the middle
// (in x and y), as long as the distance less 2 but at most field_0x3a4; zero
// outside the area or above the ground (pos->z > 0).
// FIXME: 91.1 %. JMAFastSqrt, normalize (isZero, TUtilf::inv_sqrt) and
// scale match. The original picks the length with one fsel
// (push - field_0x3a4 >= 0) where ours compares and branches (an explicit
// `push - field_0x3a4 >= 0.0f` gave 88.9 %).
void cls_804A6270::fn_80210E90(const JGeometry::TVec3f* pos, JGeometry::TVec3f* out) const {
    out->set(0.0f, 0.0f, 0.0f);
    if (pos->z <= 0.0f && mState != 0) {
        f32 radius = 0.9f * (16.0f * field_0x54.x);
        if (radius > 2.0f) {
            f32 dist = JMAFastSqrt(square(pos->x - mPos.x) + square(pos->y - mPos.y));
            if (2.0f < dist && dist <= radius) {
                out->sub(mPos, *pos);
                out->z = 0.0f;
                out->normalize();
                f32 push = dist - 2.0f;
                out->scale(push >= field_0x3a4 ? field_0x3a4 : push);
            }
        }
    }
}

void cls_804A6270::fn_80210D90() {
    mFlags |= 0x20000;
    fn_801F3604();
    leaveSand(this);
}
