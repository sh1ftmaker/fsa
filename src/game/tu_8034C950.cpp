#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/SpriteParams.h"
#include "game/act/tu_8034C950.h"

// FIXME(shotgun): not written (size, 0xAEC): update (0x8034CEB0)

// FIXME(shotgun): 99.3 %, different
void cls_804AC950::init() {
    mBox.i.x = -14.0f;
    mBox.i.y = -14.0f;
    mBox.f.x = 14.0f;
    mBox.f.y = 14.0f;
    field_0x1a4 = 2;
    field_0x80.i.x = -16.0f;
    field_0x80.i.y = -16.0f;
    field_0x80.f.x = 16.0f;
    field_0x80.f.y = 16.0f;
    for (int i = 0; i < 4; i++) {
        field_0x234[i] = -1;
        field_0x25c[i] = 0;
        field_0x320[i].field_0x0 = 1;
        field_0x320[i].field_0xc = 0.0f;
        field_0x320[i].field_0x8 = 0.0f;
        field_0x320[i].field_0x4 = 0.0f;
        field_0x320[i].field_0x10 = 0;
    }
    field_0x254[0] = -1;
    field_0x254[1] = -1;
    for (int i = 0; i < 4; i++) {
        field_0x244[i] = 0;
    }
    for (int i = 0; i < 8; i++) {
        field_0x260[i].field_0x0 = 0;
        field_0x260[i].field_0xc = 0.0f;
        field_0x260[i].field_0x8 = 0.0f;
        field_0x260[i].field_0x4 = 0.0f;
        field_0x260[i].field_0x10 = 0;
        field_0x260[i].field_0x14 = -1;
    }
    field_0xb8 = 2;
    field_0x230 = 0;
    field_0x378 = 0;
    field_0x37c = 0;
    field_0x388 = -1;
    field_0x374 = 0;
    field_0x370 = 1;
    field_0x380[0] = (field_0x90 >> 4) & 1;
    field_0x380[3] = (field_0x90 >> 5) & 1;
    field_0x380[1] = 0;
    field_0x380[4] = 0;
    field_0x380[5] = 0;
    field_0x380[2] = 0;
}

// FIXME(shotgun): 89.9 %, missing/extra
void cls_804AC950::vfunc_0x10() {
    u16 tile = fn_80226850(&mPos, field_0x4);
    if ((u16)(tile - 0x64) <= 3 || (u16)(tile - 0x2C2) <= 1 || tile == 0x2D2 || tile == 0x2D3) {
        field_0x380[0] = 1;
        field_0x380[5] = 1;
    }
    if (field_0x370 == 1) {
        f32 x0 = field_0x80.i.x + mPos.x;
        f32 y0 = field_0x80.i.y + mPos.y;
        f32 x1 = field_0x80.f.x + mPos.x;
        f32 y1 = field_0x80.f.y + mPos.y;
        s32 count = getActorManager_801F856C()->mActorEnd;
        for (s32 i = 0; i < count; i++) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
            if (actor == NULL || mSerial == actor->mSerial || actor->mAlive != 1 || field_0x4 != actor->field_0x4) {
                continue;
            }
            bool inside = false;
            if (actor->mSpawnPos.x >= x0 && actor->mSpawnPos.y >= y0 && actor->mSpawnPos.x < x1 &&
                actor->mSpawnPos.y < y1) {
                inside = true;
            }
            if (inside != true) {
                continue;
            }
            if (field_0x374 >= 8) {
                break;
            }
            if (actor->field_0x1a8 == 1) {
                field_0x380[0] = 1;
                field_0x380[1] = 1;
            } else if (actor->mActorId == 'FTGL') {
                field_0x380[0] = 1;
            }
            field_0x380[4] = 1;
            field_0x260[field_0x374].field_0x0 = actor->mActorId;
            field_0x260[field_0x374].field_0x4 = actor->mSpawnPos.x;
            field_0x260[field_0x374].field_0x8 = actor->mSpawnPos.y;
            field_0x260[field_0x374].field_0xc = actor->mSpawnPos.z;
            field_0x260[field_0x374].field_0x10 = actor->field_0x90;
            field_0x260[field_0x374].field_0x14 = actor->field_0x16c;
            actor->mAlive = false;
            field_0x374++;
        }
    }
}

// FIXME(shotgun): 99.9 %, reloc-name
void cls_804AC950::draw() {
    SpriteParams_801F5AE0 params;
    params.field_0x1d = -1;
    params.field_0x1c = 0;
    if (field_0xc4 == 1 && mPos.z != 0.0f) {
        params.field_0x20 = 0;
        params.field_0x1d = fn_8023CB24(field_0xc8) + 1;
        params.field_0x1c = fn_8023CA88(field_0xc8);
    }
    JGeometry::TVec3f saved(mPos);
    mPos.y += 8.0f;
    s32 flags = 7;
    if (field_0x230 == 0) {
        flags = -1;
        fn_80307AC8(mSerial, field_0x4, &mPos, 0xC, 0, 0, -1);
    } else {
        fn_803075AC(mSerial, field_0x4, &mPos, 0x2B, false, 2, -1, true, 0, 0);
    }
    drawSprite_801F06F0("enemy_both2_sob.bin", 0x2B, &params, flags, 1.0f, 1.0f);
    mPos = saved;
}

// FIXME(shotgun): 98.6 %, different
void cls_804AC950::vfunc_0x24() {
    u16 tile = fn_8022461C(&mPos, field_0x4);
    if (tile == 0xD || tile == 0x3B || tile == 0x39) {
        mPos.z = 0.0f;
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        startSound_801F0E34(0x30047, 0);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x4F3, -1, 1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x4F4, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x4F5, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x4F6, 0, 0, NULL, NULL, true, 1.0f);
        mAlive = false;
    } else if (tile == 0xE && fn_802DEB58() == true) {
        mPos.z = 0.0f;
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        startSound_801F0E34(0x30047, 0);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x4FB, -1, 1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x4FC, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x4FD, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x4FE, 0, 0, NULL, NULL, true, 1.0f);
        mAlive = false;
    } else {
        if (field_0x4 >= 8) {
            JGeometry::TVec3f pos(mPos);
            pos.z = 0.0f;
            getActorManager_801F856C()->spawnActor_801F9484('KMRI', field_0x4, &pos, 0, -1, -1);
        } else {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x8A, 1, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x8B, 1, 0, NULL, NULL, true, 1.0f);
        }
        startSound_801F0E34(0x30003, 0);
        mAlive = false;
    }
}

bool cls_804AC950::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    return fn_801F132C(layer, pos, &field_0x378);
}

cls_804AC950::~cls_804AC950() {}
