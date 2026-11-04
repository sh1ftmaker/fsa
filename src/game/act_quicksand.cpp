#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/ActorManager.h"
#include "game/GameMath.h"
#include "game/act/act_quicksand.h"
#include "game/tu_80212AC0.h"

void cls_804A6270::fn_80210D90() {
    mFlags |= 0x20000;
    fn_801F3604();
    switch (mState) {
    case 1: {
        s32 serial = field_0x388;
        if (serial >= 0) {
            ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(serial);
            if (actor != NULL) {
                fn_80212ECC(static_cast<EnemyBase_804A4808*>(actor));
            }
        }
        s32 viewSerial = field_0x2b8;
        if (viewSerial != 0) {
            fn_802D6A9C()->fn_802D8050(viewSerial, true, 0xff);
        }
        break;
    }
    case 6: {
        s32 viewSerial = field_0x2b8;
        if (viewSerial != 0) {
            fn_802D6A9C()->fn_802D8050(viewSerial, false, 0xff);
        }
        break;
    }
    }
    s32 serial = field_0x388;
    if (serial >= 0) {
        ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(serial);
        if (actor != NULL) {
            static_cast<EnemyBase_804A4808*>(actor)->mFlags |= 0x4000;
        }
    }
    mStateTimer = 0;
    mState = 6;
}

// FIXME: 91.1 %. JMAFastSqrt, normalize (isZero, TUtilf::inv_sqrt) and
// scale match. The original picks the length with one fsel
// (push - field_0x3a4 >= 0) where ours compares and branches (an explicit
// `push - field_0x3a4 >= 0.0f` gave 88.9 %).
// Whether a position at or below the ground lies inside the quicksand's area:
// within 0.9 * (16 * field_0x54.x) of mPos in x and y, once it is active.
bool fn_80211000(ActorBase_804A5770* actor, const JGeometry::TVec3f* pos) {
    const cls_804A6270* sand = static_cast<const cls_804A6270*>(actor);
    bool inside = false;
    if (pos->z <= 0.0f && sand->mState != 0) {
        f32 radius = 0.9f * (16.0f * sand->field_0x54.x);
        f32 dx = pos->x - sand->mPos.x;
        f32 dy = pos->y - sand->mPos.y;
        if (square(dx) + square(dy) <= square(radius)) {
            inside = true;
        }
    }
    return inside;
}

// The push-out vector for a position in the quicksand: towards the middle
// (in x and y), as long as the distance less 2 but at most field_0x3a4; zero
// outside the area or above the ground (pos->z > 0).
void fn_80210E90(const ActorBase_804A5770* actor, const JGeometry::TVec3f* pos, JGeometry::TVec3f* out) {
    const cls_804A6270* sand = static_cast<const cls_804A6270*>(actor);
    out->set(0.0f, 0.0f, 0.0f);
    if (pos->z <= 0.0f && sand->mState != 0) {
        f32 radius = 0.9f * (16.0f * sand->field_0x54.x);
        if (radius > 2.0f) {
            f32 dist = JMAFastSqrt(square(pos->x - sand->mPos.x) + square(pos->y - sand->mPos.y));
            if (2.0f < dist && dist <= radius) {
                out->sub(sand->mPos, *pos);
                out->z = 0.0f;
                out->normalize();
                f32 push = dist - 2.0f;
                out->scale(push >= sand->field_0x3a4 ? sand->field_0x3a4 : push);
            }
        }
    }
}

void cls_804A6270::vfunc_0x88() {
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    fn_802110A8();
}

void cls_804A6270::vfunc_0x54() {
    if (mFlags & 0x8000 && field_0x2bc != 0) {
        field_0x2c0.fn_800FE3EC(field_0x2bc, mPos, field_0x4);
    }
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
