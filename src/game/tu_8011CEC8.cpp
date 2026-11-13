#include "game/ActorId.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Camera.h"
#include "game/Player.h"
#include "game/Effect.h"
#include "game/EnemyBase.h"
#include "game/act/tu_8011CEC8.h"
#include "game/Unk_8045A080.h"
#include <float.h>

// 'DMGA' (cls_8049DD68): the demo camera. States 1-9 are set by fn_8011D1B8
// from vfunc_0x54, which compares the manager's counter with lbl_80464BBC.

// The SpriteAnim frame tables (lbl_80464B98: anim 24-31, then the terminator).
static const s16 lbl_80464B98[18] = {24, 6, 25, 6, 26, 6, 27, 6, 28, 6, 29, 6, 30, 6, 31, 6, -1, -1};
const s16* lbl_80539F20[2] = {lbl_80464B98, NULL};

// Counter values that vfunc_0x54 matches, entry n selecting state n.
static const s32 lbl_80464BBC[11] = {0, 0, 360, 800, 830, 950, 1040, 1160, 1325, 1550, 0};

s32 fn_801365A8();

// FIXME(shotgun): 99.69 %, reloc-name; behaviour clean.
// only (the two float literals are @NNN, not lbl_)
void cls_8049DD68::init() {
    cls_8049D140::init();
    field_0x274.fn_801D2608(lbl_80539F20, 1);
    field_0x274.startAnim_801D22DC(0, 2, false);
    field_0x274.fn_801D1F14(0.0f);
    field_0x240 = mPos;
    field_0x258 = 284.0f;
    getGameView_802D6A9C()->fn_802D7440(0);
}

// FIXME(shotgun): 78.60 %, missing/extra + reloc-name; behaviour drift.
// the guarded static (lbl_8053AA98 / lbl_8053AA9C) update() zero-inits is not written
void cls_8049DD68::update() {
    cls_8049D140::update();
    fn_8011CF48();
    switch (field_0x234) {
    case 0:
        fn_8011D1B8(1);
        break;
    case 3:
        fn_801CD950(&field_0x238, 255.0f, 10.0f);
        break;
    case 5:
        fn_801CD950(&field_0x238, 0.0f, -10.0f);
        break;
    }
    if (field_0x230) {
        Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
        JGeometry::TVec3f in(-cam->field_0x4[0][3], -cam->field_0x4[1][3], 0.0f);
        JGeometry::TVec3f out;
        cam->fn_8030C5F0((Vec*)&in, false, false, (Vec*)&out);
        f32 size = fn_801365A8() == 2 ? 373.0f : 500.0f;
        out.x -= size * 0.5f;
        out.y -= 20.0f;
        lbl_8053AED0->fn_8043A208((s32)field_0x238, out.x, out.y, size, 40.0f);
    }
    field_0x274.fn_801D1C18();
}

void cls_8049DD68::draw() {
    cls_8049D140::draw();
}

void cls_8049DD68::vfunc_0x54() {
    if (fn_8011921C()->field_0x4 == 0x11) {
        s32 v = fn_8011921C()->field_0x8;
        if (v == lbl_80464BBC[2]) {
            fn_8011D1B8(2);
        } else if (v == lbl_80464BBC[3]) {
            fn_8011D1B8(3);
        } else if (v == lbl_80464BBC[4]) {
            fn_8011D1B8(4);
        } else if (v == lbl_80464BBC[5]) {
            fn_8011D1B8(5);
        } else if (v == lbl_80464BBC[6]) {
            fn_8011D1B8(6);
        } else if (v == lbl_80464BBC[7]) {
            fn_8011D1B8(7);
        } else if (v == lbl_80464BBC[8]) {
            fn_8011D1B8(8);
        } else if (v == lbl_80464BBC[9]) {
            fn_8011D1B8(9);
        }
    }
}

s32 cls_8049DD68::vfunc_0x58() {
    return field_0x234;
}

s32 cls_8049DD68::vfunc_0x5C() {
    return field_0x234 == 9;
}

// Not auto-inlined: update and vfunc_0x54 call fn_8011D1B8 / fn_8011CF48 out of line in the
// original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// FIXME(shotgun): 11.60 %, missing/extra + reloc-name; behaviour drift.
// jump table is not yet a named jumptable_8049DDF0; case 6 pos copy and stack layout still differ
void cls_8049DD68::fn_8011D1B8(s32 state) {
    switch (state) {
    case 1:
        fn_8011921C()->fn_801186B0(0x11);
        for (s32 i = 0; i < 4; i++) {
            fn_80230BF4(i, 3);
            fn_80230498(i, getPlayerPos_8023E724(i), 0x3F, 0, -0x80, -0x80, true);
        }
        fn_8011921C()->field_0x2 = 1;
        gAudioMgr_8053AF80->fn_80458FF0(0x100001B, false, 0);
        break;
    case 2: {
        field_0x23c = 0.0f;
        field_0x238 = 0.0f;
        field_0x230 = 0;
        s32 dir[2];
        dir[0] = 0;
        dir[1] = -1;
        getGameView_802D6A9C()->fn_802DCD1C(dir);
        break;
    }
    case 3:
        field_0x238 = 0.0f;
        field_0x230 = 1;
        break;
    case 4:
        field_0x238 = 255.0f;
        break;
    case 6: {
        for (s32 i = 0; i < 4; i++) {
            cls_801192AC* area = fn_8011921C();
            ActorBase_804A5770** it = area->fn_8011871C(area->mBegin, ACTOR_DMLK);
            JGeometry::TVec3f zero(0.0f, 0.0f, 0.0f);
            const JGeometry::TVec3f* pos = it == fn_8011921C()->mEnd ? &zero : &(*it)->mSpawnPos;
            fn_80230498(i, pos, 0x3F, 0, -0x80, -0x80, true);
        }
        cls_801192AC* area = fn_8011921C();
        ActorBase_804A5770** it = area->fn_8011871C(area->mBegin, ACTOR_DMLK);
        JGeometry::TVec3f zero(0.0f, 0.0f, 0.0f);
        JGeometry::TVec3f pos = it == fn_8011921C()->mEnd ? zero : (*it)->mSpawnPos;
        spawnEmitter_8013CC50(field_0x4, &pos, 0x33F, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &pos, 0x340, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &pos, 0x341, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &pos, 0x342, 0, 0, NULL, NULL, true, 1.0f);
        startSound_801F0E34(0x30079, 0);
        break;
    }
    case 7:
        for (s32 i = 0; i < 4; i++) {
            fn_80230BF4(i, 3);
            fn_80230498(i, getPlayerPos_8023E724(i), 0x3F, 0xFF, -0x80, -0x80, true);
        }
        break;
    case 9:
        getGameView_802D6A9C()->fn_802D9CB0();
        fn_8011921C()->fn_80118384();
        fn_801F3668();
        break;
    }
    field_0x234 = state;
}

// FIXME(shotgun): 90.35 %, missing/extra + reloc-name; behaviour drift.
void cls_8049DD68::fn_8011CF48() {
    switch (field_0x234) {
    case 0:
    case 9:
        break;
    case 2:
        if (field_0x240.y - (mSpawnPos.y - 440.0f) >= 116.0f) {
            fn_801CD950(&field_0x23c, -1.4f, -0.01f);
        } else {
            fn_801CD950(&field_0x23c, -0.1f, 0.006f);
        }
        fn_801CD950(&field_0x240.y, mSpawnPos.y - 440.0f, field_0x23c);
        {
            JGeometry::TVec2f c(field_0x240.x, field_0x240.y);
            gGameRoot2_8053AB10->field_0x24->fn_8030C7C4(&c, field_0x258);
        }
        break;
    case 1:
    case 3:
    case 4:
    case 5: {
        JGeometry::TVec2f c(field_0x240.x, field_0x240.y);
        gGameRoot2_8053AB10->field_0x24->fn_8030C7C4(&c, field_0x258);
        break;
    }
    case 6: {
        if (fn_8011921C()->field_0x8 >= lbl_80464BBC[6] + 20) {
            f32 d = (f32)(lbl_80464BBC[7] - 75 - fn_8011921C()->field_0x8);
            f32 spawnY = mSpawnPos.y;
            if (d < FLT_EPSILON) {
                field_0x240.y = spawnY;
            } else {
                field_0x240.y += (1.0f / d) * (spawnY - field_0x240.y);
            }
        }
        JGeometry::TVec2f c(field_0x240.x, field_0x240.y);
        gGameRoot2_8053AB10->field_0x24->fn_8030C7C4(&c, field_0x258);
        break;
    }
    case 7: {
        JGeometry::TVec2f c(field_0x240.x, field_0x240.y);
        gGameRoot2_8053AB10->field_0x24->fn_8030C7C4(&c, field_0x258);
        break;
    }
    case 8: {
        cls_801192AC* list = fn_8011921C();
        ActorBase_804A5770** it = list->fn_8011871C(list->mBegin, 'DMSW');
        JGeometry::TVec3f zero(0.0f, 0.0f, 0.0f);
        const JGeometry::TVec3f* pos = it == fn_8011921C()->mEnd ? &zero : &(*it)->mPos;
        fn_801CD950(&field_0x240.y, 40.0f + pos->y, 1.8f);
        JGeometry::TVec2f c(field_0x240.x, field_0x240.y);
        gGameRoot2_8053AB10->field_0x24->fn_8030C7C4(&c, field_0x258);
        break;
    }
    }
}
#pragma auto_inline reset

cls_8049DD68::~cls_8049DD68() {}
