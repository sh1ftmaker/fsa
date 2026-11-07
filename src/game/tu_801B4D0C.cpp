#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/RotMatrix.h"
#include "game/SpriteParams.h"
#include "game/tu_803B8C74.h"
#include "game/act/tu_801F9484.h"
#include "std/math.h"

// The 'TLWP' actor, cls_804A38D4.

// FIXME(shotgun): tables not written out (gate.py literal tripwire); extern-declared. dtk has one
// 0xF4-byte object; the code indexes these parts (probably separate arrays in the original).
struct Tables_804A37E0 {
    /* 0x00 */ u16 mEmitterIds[20]; // init spawns one emitter per id
    /* 0x28 */ s32 field_0x28[24];
    /* 0x88 */ s32 mFrames[3][4];    // fn_801B5228: field_0x24c by field_0x98 and field_0x240
    /* 0xB8 */ s32 mDurations[3][4]; // fn_801B5228: mTimers[0] likewise
    /* 0xE8 */ s32 mRows[3];         // fn_801B5228: field_0x98 by field_0x250 - 1
}; // Size: 0xF4
extern Tables_804A37E0 lbl_804A37E0;

extern u8 lbl_8053ABF8;
s32 lbl_8053A118 = 3;

// 0x801B6170.
// FIXME(shotgun): 0 %, new
void cls_804A38D4::init() {
    mBox.set(-12.0f, -12.0f, 12.0f, 12.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    for (s32 i = 0; i < 3; i++) {
        mTimers[i] = 0;
    }
    for (s32 i = 0; i < 3; i++) {
        mEmitters294[i] = NULL;
    }
    for (s32 i = 0; i < 20; i++) {
        mEmitters2A0[i] = NULL;
    }
    lbl_8053ABF8 = 0;
    field_0x25c = 1.0f;
    field_0x250 = (field_0x90 >> 4) & 0xF;
    field_0x254 = field_0x90 & 0xF;
    if (field_0x250 == 0 && field_0x254 == 0) {
        JGeometry::TBox2f box = getGameView_802D6A9C()->mVisibleBox;
        JGeometry::TVec3f pos = mPos;
        pos.x = box.i.x + 0.5f * (box.f.x - box.i.x);
        pos.y = box.i.y + 0.5f * (box.f.y - box.i.y);
        pos = mPos;
        pos.x += 96.0f;
        pos.y += -32.0f;
        for (s32 i = 0; i < 20; i++) {
            mEmitters2A0[i] = spawnEmitter_8013CC50(field_0x4, &pos, lbl_804A37E0.mEmitterIds[i], 0, 0, NULL, NULL, true, 1.0f);
        }
    }
    field_0x24c = 0;
    for (s32 i = 0; i < 4; i++) {
        field_0x264[i].x = mPos.x;
        field_0x264[i].y = mPos.y;
        field_0x264[i].z = mPos.z;
    }
    field_0x258 = 0.0f;
    field_0x260 = 0;
    if (field_0x250 == 0) {
        lbl_8053A118 = 3;
        field_0x230 = 0;
    } else {
        mPos.z = 200.0f;
        field_0x230 = 8;
    }
}

// 0x801B609C.
// FIXME(shotgun): 0 %, new
cls_804A38D4::~cls_804A38D4() {
    for (s32 i = 0; i < 3; i++) {
        if (mEmitters294[i] != NULL) {
            getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(mEmitters294[i]);
            mEmitters294[i] = NULL;
        }
    }
    for (s32 i = 0; i < 20; i++) {
        if (mEmitters2A0[i] != NULL) {
            getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(mEmitters2A0[i]);
            mEmitters2A0[i] = NULL;
        }
    }
}

// 0x801B56A0.
// FIXME(shotgun): not written yet

// 0x801B5228.
// FIXME(shotgun): 0 %, new
void cls_804A38D4::fn_801B5228() {
    addCalc2_801F71A4(&mPos.z, field_0x258, 0.3f, field_0x25c);
    switch (field_0x230) {
    case 8:
        field_0x98 = lbl_804A37E0.mRows[field_0x250 - 1];
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i) && field_0x4 == getPlayerLayer_8023DE58(i)) {
                JGeometry::TVec3f* pos = getPlayerPos_8023E724(i);
                field_0x264[i].x = pos->x;
                field_0x264[i].y = pos->y;
                field_0x264[i].z = pos->z;
                if (i == 0) {
                    field_0x264[i].y += 0.1f;
                }
            }
        }
        field_0x25c = 3.0f;
        field_0x230 = 9;
    case 9: {
        JGeometry::TVec3f* pos = getPlayerPos_8023E724(0);
        f32 x = pos->x;
        f32 y = pos->y;
        field_0x258 = 14.0f;
        if (field_0x250 == 1) {
            field_0x260 = 3000;
            x -= 10.0f;
            field_0x258 = 8.0f;
        }
        if (field_0x250 == 3) {
            field_0x260 = -3000;
            x += 10.0f;
            field_0x258 = 8.0f;
        }
        addCalc2_801F71A4(&mPos.x, x, 0.3f, field_0x25c);
        mPos.y = y;
        f32 dz = fabs(mPos.z - field_0x258);
        f32 dx = fabs(mPos.x - x);
        if (std::sqrtf(dx * dx + dz * dz) < 1.0f) {
            if (--lbl_8053A118 == 0) {
                for (s32 i = 0; i < 4; i++) {
                    if (fn_8023F264(i) && field_0x4 == getPlayerLayer_8023DE58(i)) {
                        fn_80230498(i, &field_0x264[i], 0x3E, 0xFF, -0x80, -0x80, true);
                    }
                }
                startSound_801F0E34(0x50052, 0);
            }
            field_0x230 = 10;
        }
        break;
    }
    case 10:
        if (lbl_8053A118 <= 0) {
            if (field_0x258 != 200.0f) {
                field_0x258 = 200.0f;
                field_0x25c = 2.25f;
            } else if (field_0x250 == 2) {
                f32 z = mPos.z;
                for (s32 i = 0; i < 4; i++) {
                    if (fn_8023F264(i) && field_0x4 == getPlayerLayer_8023DE58(i)) {
                        addCalc2_801F71A4(&field_0x264[i].z, z, 0.3f, field_0x25c);
                        fn_80230498(i, &field_0x264[i], 0xB0, 0xFF, -0x80, -0x80, true);
                    }
                }
                if (fabs(mPos.z - field_0x258) < 2.0f) {
                    lbl_8053A118 = 0x309;
                }
            }
        }
        break;
    }
    if (mTimers[0] == 0) {
        field_0x24c = lbl_804A37E0.mFrames[field_0x98][field_0x240];
        mTimers[0] = lbl_804A37E0.mDurations[field_0x98][field_0x240];
        field_0x240++;
        field_0x240 &= 3;
    }
}

// 0x801B5198.
// FIXME(shotgun): 0 %, new
void cls_804A38D4::update() {
    switch (field_0x250) {
    case 0:
        fn_801B56A0();
        break;
    case 1:
    case 2:
    case 3:
        fn_801B5228();
        break;
    }
    for (s32 i = 0; i < 3; i++) {
        if (mTimers[i] > 0) {
            mTimers[i]--;
        }
    }
}

// 0x801B4EC8.
// FIXME(shotgun): 0 %, new
void cls_804A38D4::fn_801B4EC8() {
    Mtx mtx;
    SpriteParams_801F5AE0 params;
    switch (field_0x250) {
    case 1:
        params.field_0x1d = 1;
        params.field_0x0[2] = 3;
        break;
    case 2:
        params.field_0x1d = 1;
        params.field_0x0[2] = 9;
        break;
    case 3:
        params.field_0x1d = 1;
        params.field_0x0[2] = 4;
        break;
    }
    JGeometry::TVec3f rot(0.0f, (f32)field_0x260 / -10430.228f, 0.0f);
    setRotScaleMtx(mtx, rot, 1.0f);
    params.mMtx = mtx;
    drawSprite_801F06F0("enemy_both2_sob.bin", field_0x24c + 0xC0, &params, 7, 1.0f, 1.0f);
}

// 0x801B4D0C.
// FIXME(shotgun): 0 %, new
void cls_804A38D4::draw() {
    switch (field_0x250) {
    case 0:
        if (lbl_8050EC80.getCurrentFile().field_0x5a[0] & (u8)(1 << field_0x254)) {
            SpriteParams_801F5AE0 params;
            params.field_0x1d = -1;
            drawSprite_801F06F0("enemy_gc_sob.bin", 0x431, &params, -1, 1.0f, 1.0f);
            fn_801F02BC("enemy_gc_sob.bin", 0x431, &params, -1, 0, false, 1.0f, 1.0f);
        }
        break;
    case 1:
    case 2:
    case 3:
        fn_801B4EC8();
        break;
    }
}
