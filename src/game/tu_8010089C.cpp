#include "game/Camera.h"
#include "game/Effect.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/act/tu_8010089C.h"

// FIXME(shotgun): 99.7 %, reloc-name
void WhackAMoleShop_8049C8EC::init() {
    if (field_0x90 & 0x10000) {
        fn_80100A90();
        return;
    }
    mBox.i.x = -10.0f;
    mBox.i.y = -10.0f;
    mBox.f.x = 10.0f;
    mBox.f.y = 10.0f;
    field_0x247 = 1;
    field_0x246 = 0;
    field_0x245 = 0;
    field_0x238 = -1;
    mPos.x += -5.0f;
    mPos.y += 7.0f;
    field_0x24c = mPos.x;
    field_0x250 = mPos.y;
    field_0x254 = mPos.z;
    field_0x258 = 0.0f;
    field_0x25c[0] = 0;
    field_0x260[0] = 0;
    field_0x268[0] = 0;
    field_0x25c[1] = 0;
    field_0x260[1] = 0;
    field_0x268[1] = 0;
    field_0x25c[2] = 0;
    field_0x260[2] = 0;
    field_0x268[2] = 0;
    field_0x25c[3] = 0;
    field_0x260[3] = 0;
    field_0x268[3] = 0;
    field_0x244 = gGameRoot_8053AAF8->fn_80138A30();
    field_0x248 = (field_0x90 >> 22) & 0x1F;
    if (vfunc_0x34()) {
        field_0x230 = 9;
        field_0x234 = 0;
    } else {
        field_0x230 = 0;
        field_0x234 = 0;
    }
}

void WhackAMoleShop_8049C8EC::update() {
    if (field_0x90 & 0x10000) {
        field_0x245++;
        if (field_0x245 == 8) {
            field_0x246++;
            if (field_0x246 == 2) {
                field_0x246 = 0;
            }
            field_0x245 = 0;
        }
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        field_0x234++;
        if (field_0x234 < 40) {
            if ((field_0x234 & 3) == 0) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x1DA, 0, 1, NULL, NULL, true, 1.0f);
            }
        } else {
            field_0x240 = 1;
            field_0xb0 = 0x1100;
            if (field_0x234 == 160) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x1DA, 0, 1, NULL, NULL, true, 1.0f);
                fn_801F3668();
            }
            fn_801F3D94(1);
        }
    } else {
        fn_80100D28();
    }
}

// FIXME(shotgun): 96.4 %, missing/extra
void WhackAMoleShop_8049C8EC::vfunc_0x44() {
    s32 player = field_0x10c;
    if (player < 0) {
        return;
    }
    startSound_801F0E34(0x40099, 0);
    if (fn_8022F514(player)) {
        player = fn_8022F3DC(player);
    }
    if (field_0x244 == 1) {
        JGeometry::TVec3f pos = mPos;
        pos.x += -15.0f;
        spawnEmitter_8013CC50(field_0x4, &pos, 0x59, 1, 1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &pos, 0x5A, 1, 1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &pos, 0x5B, 1, 1, NULL, NULL, true, 1.0f);
        startSound_801F0E34(0x30001, 0);
        JGeometry::TVec2f pos2(pos.x, pos.y);
        fn_80238548(player, 0, &pos2, 0xb);
    } else {
        s32 hits = 1;
        s32 total = 0;
        if (player == field_0x244 - 2) {
            hits = 3;
        } else if (field_0x244 != 0) {
            total = 1;
        }
        field_0x290->field_0x260[player] += hits;
        field_0x290->field_0x268[player] += total;
    }
}

WhackAMoleShop_8049C8EC::~WhackAMoleShop_8049C8EC() {}
