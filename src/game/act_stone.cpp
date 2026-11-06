#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/ViewLayer.h"
#include "game/act/act_stone.h"

void Stone_804A93D0::init() {
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    field_0x80.i.x = -4.0f;
    field_0x80.i.y = -4.0f;
    field_0x80.f.x = 4.0f;
    field_0x80.f.y = 4.0f;
    field_0x230 = 0;
    field_0x234 = 0xFF;
    field_0x235 = false;
}

void Stone_804A93D0::update() {
    field_0x234 = 0xFF;
    if (field_0xc4) {
        mPos.set(*fn_8023E7B0(field_0xc8));
        field_0x4 = getPlayerLayer_8023DE58(field_0xc8);
        field_0x234 = fn_8023BCD4(field_0xc8);
    } else if (!viewLayerFlag(field_0x4)) {
        field_0x3c.z += -0.15f;
        mPos += field_0x3c;
        s32 damage = field_0x114;
        if (gGameRoot_8053AAF8->field_0x7f) {
            damage = 0;
        }
        if (mPos.z < 0.0f || fn_801F2618(NULL, false)) {
            if (mPos.z < 0.0f) {
                if (fn_80226850(&mPos, field_0x4) == 0x90) {
                    gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x91);
                } else if (fn_80226850(&mPos, field_0x4) == 0x91) {
                    gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x92);
                }
            }
            vfunc_0x24();
        } else if (fn_801F2B7C(damage, field_0xc8, 4) || fn_801F2B20(0)) {
            vfunc_0x24();
        }
    } else {
        if (!field_0x235) {
            mPos.y -= mPos.z;
            mPos.z = 0.0f;
            field_0x3c.z = 0.0f;
            field_0x235 = true;
        }
        field_0x3c.y += 0.15f;
        mPos += field_0x3c;
        s32 damage = field_0x114;
        if (gGameRoot_8053AAF8->field_0x7f) {
            damage = 0;
        }
        if (fn_801F2618(NULL, false)) {
            vfunc_0x24();
        } else if (fn_801F2B7C(damage, field_0xc8, 4) || fn_801F2B20(0)) {
            vfunc_0x24();
        }
    }
}

void Stone_804A93D0::draw() {
    s32 flags = 7;
    SpriteParams_801F5AE0 params;
    params.field_0x10.a = field_0x234;
    if (field_0xc4) {
        params.field_0x20 = 0;
        flags = -1;
        params.field_0x1d = fn_8023CB24(field_0xc8) + 1;
        params.field_0x1c = fn_8023CA88(field_0xc8);
    }
    fn_801F02BC("enemy_both2_sob.bin", 0x29, &params, flags, 0, true, 1.0f, 1.0f);
}

void Stone_804A93D0::vfunc_0x24() {
    JGeometry::TVec3f pos(mPos);
    pos.z = 0.0f;
    s32 layer = field_0x4;
    if (layer >= 8) {
        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_KMRI, layer, &pos, 0, -1, -1);
    }
    if (fn_8022461C(&mPos, field_0x4) == 0xD || fn_8022461C(&mPos, field_0x4) == 0x39) {
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x3D0, 0, 1, NULL, NULL, true, 1.0f);
        startSound_801F0E34(0x30022, 0);
    } else if (fn_8022461C(&mPos, field_0x4) == 0xE) {
        if (fn_802DE9A0()) {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x2A9, -1, 1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x2AA, 0, 0, NULL, NULL, true, 1.0f);
            startSound_801F0E34(0x5005F, 0);
        } else if (fn_802DEB58()) {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x45E, -1, 1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x45F, 0, 0, NULL, NULL, true, 1.0f);
            startSound_801F0E34(0x5003F, 0);
        }
    } else {
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x8C, 1, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x8D, 1, 0, NULL, NULL, true, 1.0f);
        startSound_801F0E34(0x30003, 0);
    }
    mAlive = false;
}

bool Stone_804A93D0::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    return fn_801F132C(layer, pos, &field_0x230);
}

void Stone_804A93D0::vfunc_0x48() {
    vfunc_0x24();
}

Stone_804A93D0::~Stone_804A93D0() {}
