#include "game/GameAudio.h"
#include "game/act/tu_801DE714.h"
#include "game/Camera.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/GameView.h"

// Bits of cls_804A51E4::mFlags above EnemyBase's (see fn_8021835C): the
// sounds for coming into and leaving the view play once each.
enum {
    TINK_FLAG_ENTER_SOUND_PLAYED = 0x200000,
    TINK_FLAG_LEAVE_SOUND_PLAYED = 0x400000,
};

// Widens *box by the extent of each axis of rel that is non-empty.
static inline void growBox(JGeometry::TBox2f& box, const JGeometry::TBox2f& rel) {
    if (rel.getWidth() > 0.0f) {
        box.i.x += rel.i.x;
        box.f.x += rel.f.x;
    }
    if (rel.getHeight() > 0.0f) {
        box.i.y += rel.i.y;
        box.f.y += rel.f.y;
    }
}

// FIXME: 93.3 %. Register numbers and slots only: the original keeps `result`
// in r29 (li after the first stores) and `this` in r30; ours puts li result
// first and this in r29, so every later register is shifted; the stack slots
// of the two TVec3f also differ (the first is at 0x8 in the original, 0x14 in
// ours).
// Whether the actor's box around mPos is in view (the camera's screen, or
// fn_8022B978 on the upper layers).
bool cls_804A51E4::fn_80218538() const {
    JGeometry::TVec2<f32> p(mPos.x, mPos.y);
    JGeometry::TBox2f box(p, p);
    bool result = false;
    growBox(box, mBox);
    if (fn_801CD2F8(field_0x4)) {
        if (fn_8022B978(field_0x4, &box, false)) {
            result = true;
        }
    } else {
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
        bool inView = false;
        JGeometry::TVec3f a(box.i.x, box.i.y, 0.0f);
        if (camera->fn_8030C2A8(&a)) {
            JGeometry::TVec3f b(box.f.x, box.f.y, 0.0f);
            if (camera->fn_8030C2A8(&b)) {
                inView = true;
            }
        }
        if (inView) {
            result = true;
        }
    }
    return result;
}

// Plays a sound once when the actor's position (fn_801CE010) comes into the
// view while mState is 1, and another when it leaves.
void cls_804A51E4::fn_8021835C() {
    if (mState != 1) {
        return;
    }
    bool visible = false;
    JGeometry::TVec3f screen;
    fn_801CE010(&screen);
    JGeometry::TVec3f a(screen);
    JGeometry::TVec3f b(screen);
    if (mBox.getWidth() > 0.0f) {
        a.x += mBox.i.x;
        b.x += mBox.f.x;
    }
    if (fn_801CD2F8(field_0x4)) {
        if (fn_8022BA58(field_0x4, &a) && fn_8022BA58(field_0x4, &b)) {
            visible = true;
        }
    } else {
        if (gGameRoot2_8053AB10->field_0x24->fn_8030C2A8(&a) && gGameRoot2_8053AB10->field_0x24->fn_8030C2A8(&b)) {
            visible = true;
        }
    }
    if (visible) {
        if (!(mFlags & TINK_FLAG_ENTER_SOUND_PLAYED) && !(mFlags & 0x4000)) {
            if (!fn_801E1DE8()) {
                if (mFlags & 0x20000) {
                    startSound_801F0E34(0x5001C, 0);
                } else {
                    startSound_801F0E34(0x50018, 0);
                }
            }
            mFlags |= TINK_FLAG_ENTER_SOUND_PLAYED;
        }
    } else if ((mFlags & TINK_FLAG_ENTER_SOUND_PLAYED) && !(mFlags & TINK_FLAG_LEAVE_SOUND_PLAYED) &&
               (mFlags & 0x4000) && !(mFlags & 0x80000) &&
               mTargetSerial >= 0) {
        if (!fn_801E1DE8()) {
            startSound_801F0E34(0x5001B, 0);
        }
        mFlags |= TINK_FLAG_LEAVE_SOUND_PLAYED;
    }
}

// vfunc_0xA0 (the last slot of the vtable at 0x804A51E4): while mState is
// nonzero, hands the camera the screen position (fn_801CE010) of the actor
// once its x extent lies inside the view's box for its layer and mPos.y lies
// between that box's top and bottom.
void cls_804A51E4::vfunc_0xA0() {
    if (mState == 0) {
        return;
    }
    s32 layer = field_0x4;
    GameView_804A9950* view = getGameView_802D6A9C();
    JGeometry::TBox2f* box = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
    if (box->i.y < mPos.y && mPos.y < box->f.y) {
        f32 left = mPos.x;
        f32 right = mPos.x;
        if (mBox.getWidth() > 0.0f) {
            left = mPos.x + mBox.i.x;
            right = mPos.x + mBox.f.x;
        }
        bool inside = false;
        if (box->i.x < left && right < box->f.x) {
            inside = true;
        }
        if (inside) {
            JGeometry::TVec3f screen;
            fn_801CE010(&screen);
            gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&screen);
        }
    }
}
