#include "game/GameAudio.h"
#include "game/cls_804A7BE8.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/ViewLayer.h"

cls_804A7BE8::~cls_804A7BE8() {
    if (field_0x244 == 0 && field_0x240 != field_0x23c) {
        fn_8022CA6C(field_0x90 & 3, field_0x23c, true);
    }
    if (lbl_8053AAF8->field_0x7e == 0) {
        lbl_8053AAF8->fn_80134FE0(0);
    }
    fn_8022CDD4(field_0x90 & 3);
}

// FIXME: 96.58 %. The loop: the original keeps the index in r30 and a byte
// offset (mr r28, r30 after the end load, stepped by 4) for mActors[i]; ours
// keeps the index in r28 and the offset in r30 whatever the declaration order
// (tried: index before/after the end load, actor hoisted). The 30.0f literal is
// lbl_8054075C in the original and an anonymous literal here (data split).
void cls_804A7BE8::init() {
    if (fn_8022F514(field_0x90 & 3)) {
        mAlive = false;
        return;
    }
    fn_8022CF08(field_0x90 & 3);
    mPos = *fn_8023E724(field_0x90 & 3);
    mPos.z += 30.0f;
    mLife = 0;
    field_0x234 = 1;
    field_0x238 = 0;
    field_0x240 = field_0x23c = fn_8022CAF8(field_0x90 & 3);
    field_0x244 = true;
    field_0x11f = 1;
    fn_801F0D20(0x45);
    field_0x248 = field_0x90 & 3;

    s32 count = 3;
    for (s32 i = 0; i < 4; i++) {
        if (fn_8022F514(i)) {
            count--;
        }
    }
    if (count == 0) {
        field_0x24c = true;
        if (viewLayerFlag(field_0x4)) {
            field_0x230 = 0x3F;
        } else {
            field_0x230 = 0x3D;
        }
        if (field_0x23c == 5) {
            field_0x23c = 3;
        }
    } else {
        field_0x24c = false;
        if (viewLayerFlag(field_0x4)) {
            field_0x230 = 0x44;
        } else {
            field_0x230 = 0x43;
        }
        if (field_0x23c == 5) {
            field_0x23c = 0;
        }
    }

    field_0x250 = 0;
    field_0x254 = NULL;
    ActorBase_804A5770* actor;
    s32 actorEnd = fn_801F856C()->mActorEnd;
    for (s32 i = 0; i < actorEnd; i++) {
        actor = fn_801F856C()->mActors[i];
        if (actor != NULL && actor->mActorId == 'FGAL' && actor->mAlive == true) {
            field_0x254 = actor;
        }
    }
}
