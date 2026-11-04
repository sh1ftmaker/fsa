#include "game/GameAudio.h"
#include "game/act/act_magic_circle_generator.h"
#include "game/GameView.h"
#include "game/act/act_small_key.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"

void cls_804A9FB0::draw() {}

// FIXME: 93.51 %. The -20.0f / 20.0f literals are @508 / @509, not
// lbl_80541260 / lbl_80541264 (data split), and the search keeps its loop
// value in r29 where the original has li r28, -1 (r28 = previous serial).
bool cls_804A9FB0::fn_802E7DB8() {
    mBox.set(-20.0f, -20.0f, 20.0f, 20.0f);
    f32 left = mBox.i.x + mPos.x;
    f32 top = mBox.i.y + mPos.y;
    f32 right = mBox.f.x + mPos.x;
    f32 bottom = mBox.f.y + mPos.y;

    s32 serial = -1;
    for (;;) {
        s32 found = fn_801F856C()->fn_801F78F4(ACTOR_KEY0, serial);
        serial = found;
        if (found < 0) {
            return false;
        }
        SmallKey_804A8CD8* key = static_cast<SmallKey_804A8CD8*>(fn_801F856C()->fn_801F666C(found));
        if (key == NULL) {
            continue;
        }
        if (key->field_0x280 != 6) {
            continue;
        }
        JGeometry::TVec3f pos = key->mPos;
        if (pos.z != 0.0f) {
            continue;
        }
        f32 keyLeft = key->mBox.i.x + pos.x;
        f32 keyTop = key->mBox.i.y + pos.y;
        f32 keyRight = key->mBox.f.x + pos.x;
        f32 keyBottom = key->mBox.f.y + pos.y;
        if (keyLeft <= left) {
            keyLeft = left;
        }
        if (keyTop <= top) {
            keyTop = top;
        }
        if (keyRight >= right) {
            keyRight = right;
        }
        if (keyBottom >= bottom) {
            keyBottom = bottom;
        }
        bool overlap = false;
        if (keyRight >= keyLeft && keyBottom >= keyTop) {
            overlap = true;
        }
        if (!overlap) {
            continue;
        }

        field_0x244 = 1;
        field_0x24c = 0x14;
        JGeometry::TVec3f effectPos = pos;
        if (mEmitter230 != NULL) {
            fn_801EE434(mEmitter230);
        }
        if (mEmitter234 != NULL) {
            fn_801EE434(mEmitter234);
        }
        if (mEmitter238 != NULL) {
            fn_801EE434(mEmitter238);
        }
        if (mEmitter23c != NULL) {
            fn_801EE434(mEmitter23c);
        }
        if (mEmitter240 != NULL) {
            fn_801EE434(mEmitter240);
        }
        mEmitter230 = NULL;
        mEmitter234 = NULL;
        mEmitter238 = NULL;
        mEmitter23c = NULL;
        mEmitter240 = NULL;
        fn_8013CC50(field_0x4, &effectPos, 0x85, 0, 0, NULL, NULL, true, 1.0f);
        fn_8013CC50(field_0x4, &effectPos, 0x86, 0, 0, NULL, NULL, true, 1.0f);
        fn_8013CC50(field_0x4, &effectPos, 0x87, 0, 0, NULL, NULL, true, 1.0f);
        fn_801F0E34(0x30069, 0);
        key->fn_802C5284();
        key->mAlive = false;
        fn_801F34DC();
        return true;
    }
}

cls_804A9FB0::~cls_804A9FB0() {
    if (mEmitter230 != NULL) {
        fn_801EE434(mEmitter230);
    }
    if (mEmitter234 != NULL) {
        fn_801EE434(mEmitter234);
    }
    if (mEmitter238 != NULL) {
        fn_801EE434(mEmitter238);
    }
    if (mEmitter23c != NULL) {
        fn_801EE434(mEmitter23c);
    }
    if (mEmitter240 != NULL) {
        fn_801EE434(mEmitter240);
    }
}

void cls_804A9FB0::init() {
    field_0x248 = field_0x90 & 7;
    field_0x24c = 0;
    field_0x264 = 0;
    field_0x254 = 0;
    field_0x268 = 0;
    field_0x258 = 0;
    field_0x26c = 0;
    field_0x25c = 0;
    field_0x270 = 0;
    field_0x260 = 0;
    mEmitter230 = NULL;
    mEmitter234 = NULL;
    mEmitter238 = NULL;
    mEmitter23c = NULL;
    mEmitter240 = NULL;
    if (!fn_802D6A9C()->fn_802D75BC(field_0x16c)) {
        field_0x244 = 0;
    } else if (field_0x248 == 0 || field_0x248 == 3) {
        field_0x244 = 1;
        field_0x24c = 0x14;
    } else {
        mAlive = false;
    }
}
