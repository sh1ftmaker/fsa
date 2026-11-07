#include "game/GameAudio.h"
#include "game/act/act_falling_floor.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "std/math.h"

void FallingFloor_804A9078::init() {
    mBox.i.x = -5.0f;
    mBox.i.y = -5.0f;
    mBox.f.x = 5.0f;
    mBox.f.y = 5.0f;
    field_0x23c = 0;
    field_0x238 = field_0x90 & 0x3F;
    if (field_0x238 != 2) {
        mPos.x += 8.0f;
        mPos.y += 8.0f;
    } else {
        mPos.x += 8.0f;
    }
    if (field_0x238 == 0 || field_0x238 == 3) {
        field_0x230 = 0;
    } else if (field_0x238 == 1) {
        field_0x230 = 5;
    } else {
        field_0x230 = 8;
    }
}

// 0x802CCF14. FLYK: a floor tile that crumbles once a player stands on it (state 0 / 5), then
// falls away and is replaced by the pit tile; state 8 only restores the tile under a player.
// FIXME(shotgun): 88.61 %, missing/extra + reloc-name
void FallingFloor_804A9078::update() {
    JGeometry::TVec2f pos2d;
    pos2d.set(mPos.x, mPos.y);
    JGeometry::TBox2f box;
    box.i.x = mBox.i.x + mPos.x;
    box.i.y = mBox.i.y + mPos.y;
    box.f.x = mBox.f.x + mPos.x;
    box.f.y = mBox.f.y + mPos.y;
    s32 standing;
    if (field_0x238 == 3) {
        standing = fn_8023AF14(&box, field_0x4, false, 100.0f);
    } else {
        standing = fn_8023AF14(&box, field_0x4, false, 0.0f);
    }
    s32 under = fn_8023B8E4(&box, field_0x4, &pos2d, true);
    mLife = 200;
    field_0xd8 = 0;
    s32 on[4];
    for (s32 i = 0; i < 4; i++) {
        on[i] = -1;
        if (fn_8023A51C(i)) {
            on[i] = i;
        }
    }
    s32 near[4];
    JGeometry::TVec2f out;
    for (s32 i = 0; i < 4; i++) {
        near[i] = -1;
        if (fn_8023AAF8(i, &box, field_0x4, &out)) {
            near[i] = i;
        }
    }
    switch (field_0x230) {
    case 0:
        if (standing >= 0) {
            if (field_0x238 == 3) {
                gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x3F);
                startSound_801F0E34(0x30015, 0);
                field_0x23c = 60;
                field_0x230 = 4;
            } else {
                gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x91);
                field_0x230 = 1;
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x28B, 0, 0, NULL, NULL, true, 0.3f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x28C, 0, 0, NULL, NULL, true, 0.3f);
                startSound_801F0E34(0x50013, 0);
            }
        } else {
            for (s32 i = 0; i < 4; i++) {
                if (on[i] >= 0) {
                    f32 dx = mPos.x - getPlayerPos_8023E724(i)->x;
                    f32 dy = mPos.y - getPlayerPos_8023E724(i)->y;
                    f32 dist = std::sqrtf(dx * dx + dy * dy);
                    if (dist < 100.0f) {
                        field_0x23c = (s32)dist >> 3;
                        field_0x230 = (field_0x238 == 3) ? 3 : 2;
                        break;
                    }
                } else if (near[i] >= 0) {
                    f32 dx = mPos.x - getPlayerPos_8023E724(i)->x;
                    f32 dy = mPos.y - getPlayerPos_8023E724(i)->y;
                    f32 dist = std::sqrtf(dx * dx + dy * dy);
                    if (dist < 40.0f) {
                        field_0x23c = (s32)dist >> 3;
                        field_0x230 = (field_0x238 == 3) ? 3 : 2;
                        break;
                    }
                }
            }
            if (fn_80226850(&mPos, field_0x4) != 0x90 && field_0x238 != 3) {
                field_0x230 = 5;
            }
        }
        break;
    case 1:
        if (standing != 0) {
            field_0x230 = 5;
        }
        break;
    case 2:
        if (field_0x23c == 0) {
            gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x91);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x28B, 0, 0, NULL, NULL, true, 0.3f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x28C, 0, 0, NULL, NULL, true, 0.3f);
            startSound_801F0E34(0x50013, 0);
            field_0x230 = 5;
        } else {
            field_0x23c--;
        }
        break;
    case 3:
        if (field_0x23c == 0) {
            gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x3F);
            startSound_801F0E34(0x30015, 0);
            field_0x230 = 4;
            field_0x23c = 60;
        } else {
            field_0x23c--;
        }
        break;
    case 4:
        if (field_0x23c == 0) {
            gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x3E);
            startSound_801F0E34(0x30016, 0);
            field_0x230 = 0;
        } else {
            field_0x23c--;
        }
        break;
    case 5:
        if (standing >= 0) {
            field_0x230 = 7;
        } else {
            for (s32 i = 0; i < 4; i++) {
                if (on[i] >= 0) {
                    field_0x230 = 2;
                    f32 dx = mPos.x - getPlayerPos_8023E724(i)->x;
                    f32 dy = mPos.y - getPlayerPos_8023E724(i)->y;
                    f32 dist = std::sqrtf(dx * dx + dy * dy);
                    field_0x23c = (s32)dist >> 3;
                    field_0x230 = 6;
                    break;
                }
            }
            if (fn_80226850(&mPos, field_0x4) != 0x91 && fn_80226850(&mPos, field_0x4) != 0x22E &&
                fn_80226850(&mPos, field_0x4) != 1) {
                mAlive = false;
            }
        }
        break;
    case 6:
        if (field_0x23c == 0) {
            gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x92);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x28B, 0, 0, NULL, NULL, true, 0.5f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x28C, 0, 0, NULL, NULL, true, 0.5f);
            startSound_801F0E34(0x50012, 0);
            mAlive = false;
        } else {
            field_0x23c--;
        }
        break;
    case 7:
        if (standing < 0) {
            gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x92);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x28B, 0, 0, NULL, NULL, true, 0.5f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x28C, 0, 0, NULL, NULL, true, 0.5f);
            startSound_801F0E34(0x50012, 0);
            mAlive = false;
        }
        break;
    case 8:
        if (under >= 0 && fn_80235448(under)) {
            JGeometry::TVec3f p;
            p.x = mPos.x;
            p.y = mPos.y;
            p.z = 0.0f;
            if (fn_80226998(&p, field_0x4) == 1) {
                gBgRewrite_8053AC98->fn_802A3948(&mPos, 0, field_0x4, 0x3E0);
                startSound_801F0E34(0x50045, 0);
                if (field_0x4 >= 8) {
                    getActorManager_801F856C()->spawnActor_801F9484('KMRI', field_0x4, &mPos, 0, -1, -1);
                } else {
                    startSound_801F0E34(0x30075, 0);
                    spawnEmitter_8013CC50(field_0x4, &mPos, 0x24A, 1, 0, NULL, NULL, true, 1.0f);
                    spawnEmitter_8013CC50(field_0x4, &mPos, 0x24B, 1, 0, NULL, NULL, true, 1.0f);
                }
                mAlive = false;
            }
        }
        break;
    }
}

void FallingFloor_804A9078::draw() {}

FallingFloor_804A9078::~FallingFloor_804A9078() {}
