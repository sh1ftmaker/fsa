#include "game/GameAudio.h"
#include "game/cls_804A7BE8.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/ViewLayer.h"
#include "game/Camera.h"
#include "game/SpriteParams.h"

// FIXME: 96.58 %. The loop: the original keeps the index in r30 and a byte
// offset (mr r28, r30 after the end load, stepped by 4) for mActors[i]; ours
// keeps the index in r28 and the offset in r30 whatever the declaration order
// (tried: index before/after the end load, actor hoisted). The 30.0f literal is
// lbl_8054075C in the original and an anonymous literal here (data split).
void UnknownFORM_804A7BE8::init() {
    if (fn_8022F514(field_0x90 & 3)) {
        mAlive = false;
        return;
    }
    fn_8022CF08(field_0x90 & 3);
    mPos = *getPlayerPos_8023E724(field_0x90 & 3);
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
    s32 actorEnd = getActorManager_801F856C()->mActorEnd;
    for (s32 i = 0; i < actorEnd; i++) {
        actor = getActorManager_801F856C()->mActors[i];
        if (actor != NULL && actor->mActorId == 'FGAL' && actor->mAlive == true) {
            field_0x254 = actor;
        }
    }
}

UnknownFORM_804A7BE8::~UnknownFORM_804A7BE8() {
    if (field_0x244 == 0 && field_0x240 != field_0x23c) {
        fn_8022CA6C(field_0x90 & 3, field_0x23c, true);
    }
    if (gGameRoot_8053AAF8->field_0x7e == 0) {
        gGameRoot_8053AAF8->fn_80134FE0(0);
    }
    fn_8022CDD4(field_0x90 & 3);
}

// Keeps x inside [lo, hi): below lo it becomes lo, from hi up it becomes hi.
static inline void clampX(f32& x, f32 lo, f32 hi) {
    if (x < lo) {
        x = lo;
    } else if (x >= hi) {
        x = hi;
    }
}

// FIXME(shotgun): 88 %, different + reloc-name
void UnknownFORM_804A7BE8::draw() {
    JGeometry::TBox2f box;
    JGeometry::TVec3f pos(mPos);
    if (field_0x4 == 0) {
        box = gGameRoot2_8053AB10->field_0x24->field_0x164;
        if (pos.y - pos.z < 28.0f + box.i.y) {
            pos.y += 52.0f;
        }
        if (field_0x24c) {
            clampX(pos.x, 68.0f + box.i.x, box.f.x - 60.0f);
        } else {
            clampX(pos.x, 52.0f + box.i.x, box.f.x - 44.0f);
        }
    } else {
        fn_8022B8EC(field_0x248, &box);
        if (pos.y - pos.z < 28.0f + box.i.y) {
            pos.y += 52.0f;
        }
        if (field_0x24c) {
            if (fn_8022C8EC(field_0x90 & 3)) {
                clampX(pos.x, 52.0f + box.i.x, box.f.x - 52.0f);
            } else {
                clampX(pos.x, 80.0f + box.i.x, box.f.x - 80.0f);
            }
        } else {
            if (fn_8022C8EC(field_0x90 & 3)) {
                clampX(pos.x, 36.0f + box.i.x, box.f.x - 36.0f);
            } else {
                clampX(pos.x, 66.0f + box.i.x, box.f.x - 66.0f);
            }
        }
    }
    SpriteParams_801F5AE0 params;
    params.field_0x1c = 2;
    params.field_0x2d = 1;
    s32 id = field_0x230;
    if (field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, id, &params, 1.0f, 1.0f);
    }
    if (fn_8022C8EC(field_0x90 & 3)) {
        fn_80307AC8(mSerial, field_0x4, &pos, field_0x230, 0, 0, -1);
    } else {
        fn_80307AC8(mSerial, field_0x4, &pos, field_0x230, 1, 0, -1);
    }
}
