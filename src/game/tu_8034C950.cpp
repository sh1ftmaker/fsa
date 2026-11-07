#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/SpriteParams.h"
#include "game/act/tu_8034C950.h"
#include "game/BgRewrite.h"
#include "dolphin/mtx.h"

// The point in front of a player, by its facing (fn_80230CFC): (x, y) offsets.
static const f32 lbl_80472190[4][2] = {{16.0f, 0.0f}, {-16.0f, 0.0f}, {0.0f, 16.0f}, {0.0f, -16.0f}};


// FIXME(shotgun): 99.49 %, different
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
        field_0x320[i].field_0x4.z = 0.0f;
        field_0x320[i].field_0x4.y = 0.0f;
        field_0x320[i].field_0x4.x = 0.0f;
        field_0x320[i].field_0x10 = 0;
    }
    field_0x254[0] = -1;
    field_0x254[1] = -1;
    for (int i = 0; i < 4; i++) {
        field_0x244[i] = 0;
    }
    for (int i = 0; i < 8; i++) {
        field_0x260[i].field_0x0 = 0;
        field_0x260[i].field_0x4.z = 0.0f;
        field_0x260[i].field_0x4.y = 0.0f;
        field_0x260[i].field_0x4.x = 0.0f;
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
            field_0x260[field_0x374].field_0x4.x = actor->mSpawnPos.x;
            field_0x260[field_0x374].field_0x4.y = actor->mSpawnPos.y;
            field_0x260[field_0x374].field_0x4.z = actor->mSpawnPos.z;
            field_0x260[field_0x374].field_0x10 = actor->field_0x90;
            field_0x260[field_0x374].field_0x14 = actor->field_0x16c;
            actor->mAlive = false;
            field_0x374++;
        }
    }
}

// FIXME(shotgun): 93.6 %, missing/extra
void cls_804AC950::update() {
    JGeometry::TBox2f box(field_0x80);
    f32 x0 = -16.0f + mPos.x;
    f32 y0 = -16.0f + mPos.y;
    f32 x1 = 16.0f + mPos.x;
    f32 y1 = 16.0f + mPos.y;
    box.i.x += mPos.x;
    box.f.x += mPos.x;
    box.i.y += mPos.y;
    box.f.y += mPos.y;
    switch (field_0x230) {
    case 0: {
        fn_80239914(&box, field_0x4, mSerial);
        CollisionBoxes_8020D58C* boxes = fn_8020D58C();
        boxes->mLists[boxes->field_0x0].fn_8020D8C0(&box, field_0x4, mSerial);
        field_0x234[0] = -1;
        field_0x234[1] = -1;
        field_0x234[2] = -1;
        field_0x234[3] = -1;
        field_0x244[0] = 0;
        field_0x244[1] = 0;
        field_0x244[2] = 0;
        field_0x244[3] = 0;
        field_0x388 = -1;
        for (s32 i = 0; i < 4; i++) {
            if (!fn_8023F264(i)) {
                continue;
            }
            f32 px = getPlayerPos_8023E724(i)->x;
            f32 py = getPlayerPos_8023E724(i)->y;
            s32 dir = fn_80230CFC(i);
            px += lbl_80472190[dir][0];
            py += lbl_80472190[dir][1];
            field_0x25c[i] = fn_8022C004(i);
            if (field_0x25c[i] == 1 && fn_8023E8F0(i) == 1 && fn_8023188C(i) == 1 && fn_802319B4(i, 0x11) == 1 &&
                !fn_80234D94(i)) {
                bool inside = false;
                if (px >= x0 && py >= y0 && px < x1 && py < y1) {
                    inside = true;
                }
                if (inside == 1) {
                    field_0x234[i] = dir;
                    field_0x244[field_0x234[i]]++;
                } else {
                    field_0x25c[i] = 0;
                }
            } else {
                field_0x25c[i] = 0;
            }
            if (field_0x25c[i] == 1 && field_0x388 == -1 && field_0x244[field_0x234[i]] >= 2) {
                field_0x388 = field_0x234[i];
            }
        }
        if (field_0x388 == -1) {
            break;
        }
        s32 count = 0;
        if (field_0x244[field_0x388] >= 2) {
            f32 dist[4];
            for (s32 i = 0; i < 4; i++) {
                if (field_0x234[i] == field_0x388) {
                    if (fn_8023E8F0(i) == 1) {
                        dist[i] = PSVECDistance(&mPos, getPlayerPos_8023E724(i));
                    } else {
                        field_0x234[i] = -1;
                        dist[i] = 0.0f;
                        field_0x244[field_0x388]--;
                    }
                } else {
                    field_0x234[i] = -1;
                    dist[i] = 0.0f;
                }
            }
            for (s32 j = 0; j < field_0x244[field_0x388] - 2; j++) {
                f32 best = 0.0f;
                s32 far;
                if (best < dist[0]) {
                    best = dist[0];
                    far = 0;
                }
                if (best < dist[1]) {
                    best = dist[1];
                    far = 1;
                }
                if (best < dist[2]) {
                    best = dist[2];
                    far = 2;
                }
                if (best < dist[3]) {
                    best = dist[3];
                    far = 3;
                }
                if (best > 0.0f) {
                    field_0x234[far] = -1;
                    dist[far] = 0.0f;
                    field_0x244[field_0x388]--;
                }
            }
        }
        if (field_0x244[field_0x388] >= 2) {
            for (s32 i = 0; i < 4; i++) {
                field_0x320[i].field_0x4 = *getPlayerPos_8023E724(i);
                field_0x320[i].field_0x10 = fn_8022CAF8(i);
                if (field_0x234[i] == field_0x388 && count < 2) {
                    if (fn_8022F9E4(i, mSerial, true, false)) {
                        field_0x254[count++] = i;
                        if (count >= 2) {
                            if (vfunc_0x3C() > 0) {
                                vfunc_0x30(0xFF, true);
                            }
                            field_0x230 = 1;
                            if (field_0x380[5] == 1) {
                                fn_801F3604();
                            }
                            field_0xb8 = 0;
                            field_0x37c = 3;
                        }
                    }
                } else {
                    field_0x234[i] = -1;
                }
            }
        }
        if (field_0x230 == 1) {
            for (s32 i = 0; i < 4; i++) {
                if (fn_8022E438(field_0x254[0], i) == 1 || fn_8022E438(field_0x254[1], i) == 1) {
                    field_0x320[i].field_0x0 = 0;
                }
            }
        }
        break;
    }
    case 1:
        if (field_0xc4) {
            JGeometry::TVec3f held[2];
            JGeometry::TVec3f* next = held;
            for (s32 i = 0; i < 4; i++) {
                if (field_0x234[i] == field_0x388 && field_0x320[i].field_0x0 == 0) {
                    if (field_0x320[i].field_0x10 != fn_8022CAF8(i) ||
                        (fn_8022CAF8(i) == 4 && fn_8022F478(i) == 1)) {
                        fn_80230C6C(i);
                        return;
                    }
                    if (fn_802373EC(i) == 7 && fn_8022C310(i) == 1) {
                        fn_80230C6C(i);
                        return;
                    }
                }
                JGeometry::TVec3f pos(*getPlayerPos_8023E724(i));
                JGeometry::TVec3f prev(field_0x320[i].field_0x4);
                if (field_0x320[i].field_0x0 == 0) {
                    f32 dx = pos.x - prev.x;
                    f32 dy = pos.y - prev.y;
                    if (JGeometry::TUtil<f32>::sqrt(dx * dx + dy * dy) > 2.0f) {
                        JGeometry::TVec3f back = prev - pos;
                        fn_8023DFF4(i, &back);
                    }
                }
                if (field_0x234[i] == field_0x388) {
                    *next = *fn_8023E7B0(i);
                    next++;
                }
            }
            held[0].add(held[1]);
            held[0].scale(0.5f);
            if (held[0].z > 0.0f) {
                mPos.x = held[0].x;
                mPos.y = held[0].y;
                mPos.z = held[0].z;
                field_0x4 = getPlayerLayer_8023DE58(field_0xc8);
            }
            if (mPos.z <= 0.0f) {
                fn_8020D58C()->addBox(mSerial);
            } else if (field_0x380[2] == 0) {
                startSound_801F0E34(0x3008C, 0);
                field_0x380[2] = 1;
            }
        } else {
            field_0x3c.z += -0.15f;
            mPos.add(field_0x3c);
            if (mPos.z < 0.0f || fn_801F2618(NULL, false)) {
                if (mPos.z < 0.0f) {
                    if (fn_80226850(&mPos, field_0x4) == 0x90) {
                        gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x91);
                    } else if (fn_80226850(&mPos, field_0x4) == 0x91) {
                        gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x92);
                    }
                }
                field_0x230 = 2;
                field_0x37c = 0;
            } else {
                u8 mask = fn_801F2740(-1);
                bool hit = false;
                if (mask > 0) {
                    for (s32 i = 0; i < 4; i++) {
                        if (field_0x234[i] != field_0x388 && (mask & (1 << i)) > 0 &&
                            !fn_8022E438(field_0x254[0], i) && !fn_8022E438(field_0x254[1], i)) {
                            fn_80238548(i, 0, (const JGeometry::TVec2f*)&mPos, 5);
                            hit = true;
                        }
                    }
                }
                if (hit == 1 || fn_801F2B20(0)) {
                    field_0x230 = 2;
                    field_0x37c = 0;
                }
            }
        }
        break;
    case 2:
        vfunc_0x24();
        break;
    }
    if ((field_0x230 == 1 && mPos.z > 0.0f) || field_0x230 == 2) {
        field_0x37c--;
        // FIXME(shotgun): the original loads field_0x370 as a word (lwz) here; init stores a byte.
        if (field_0x37c <= 0 && field_0x380[4] == 1 && field_0x370 == 1) {
            for (s32 i = 0; i < field_0x374; i++) {
                getActorManager_801F856C()->spawnActor_801F9484(field_0x260[i].field_0x0, field_0x4,
                                                                &field_0x260[i].field_0x4, field_0x260[i].field_0x10,
                                                                field_0x260[i].field_0x14, -1);
            }
            field_0x380[4] = 0;
        }
        if (field_0x380[0] == 1) {
            if (field_0x380[1] != 1) {
                fn_801F0D20(3);
            }
            field_0x380[0] = 0;
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
