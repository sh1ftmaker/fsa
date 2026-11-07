#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/RotMatrix.h"
#include "game/SpriteParams.h"
#include "game/tu_803B8C74.h"
#include "game/ActorId.h"
#include "game/GameState.h"
#include "game/Message.h"
#include "game/Map.h"
#include "JSystem/JMath/JMath.h"
#include "game/act/tu_801F9484.h"
#include "std/math.h"

// The 'TLWP' actor, cls_804A38D4.

// FIXME(shotgun): tables not written out (gate.py literal tripwire); extern-declared. dtk has one
// 0xF4-byte object; the code indexes these parts (probably separate arrays in the original).
struct Tables_804A37E0 {
    /* 0x00 */ u16 mEmitterIds[20]; // init spawns one emitter per id
    /* 0x28 */ s32 field_0x28[8];    // fn_801B56A0 state 7: fn_801365E8's first argument by field_0x254
    /* 0x48 */ s32 field_0x48[8];    // its second
    /* 0x68 */ s32 mMsgs[8];         // fn_801B56A0 state 1: the message by field_0x254
    /* 0x88 */ s32 mFrames[3][4];    // fn_801B5228: field_0x24c by field_0x98 and field_0x240
    /* 0xB8 */ s32 mDurations[3][4]; // fn_801B5228: mTimers[0] likewise
    /* 0xE8 */ s32 mRows[3];         // fn_801B5228: field_0x98 by field_0x250 - 1
}; // Size: 0xF4
extern Tables_804A37E0 lbl_804A37E0;

extern u8 lbl_8053ABF8;
s32 lbl_8053A118 = 3;

// 0x801B6170.
// FIXME(shotgun): 78.11 %, missing/extra + reloc-name
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
// FIXME(shotgun): 92.68 %, different
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
// FIXME(shotgun): 78.06 %, missing/extra + reloc-name
void cls_804A38D4::fn_801B56A0() {
    JGeometry::TBox2f box = mBox;
    JGeometry::TVec3f pos = mPos;
    box.i.x += mPos.x;
    box.f.x += mPos.x;
    box.i.y += mPos.y;
    box.f.y += mPos.y;
    if (!(lbl_8050EC80.getCurrentFile().field_0x5a[0] & (u8)(1 << field_0x254))) {
        return;
    }
    if (gGameRoot_8053AAF8->field_0x7e == 0) {
        return;
    }
    if (field_0x230 == 5 || field_0x230 == 6 || field_0x230 == 7) {
        if (lbl_8053AD30->fn_80309C4C(0x1000, -1)) {
            lbl_8053A118 = 0x309;
            field_0x230 = 7;
        }
    }
    s32 dir;
    f32 dist;
    s32 player;
    fn_801EEC98(mSerial, NULL, &dir, &dist, &player, 0, 0);
    switch (field_0x230) {
    case 0:
        mEmitters294[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x48B, 0, 0, NULL, NULL, true, 1.0f);
        mEmitters294[1] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x48C, 0, 0, NULL, NULL, true, 1.0f);
        field_0x230 = 1;
    case 1:
        fn_8022FEB8(&box, field_0x4, mSerial, false, false);
        field_0x244 = fn_8022FD8C(mSerial);
        if (field_0x244 < 0 || !fn_802300EC(field_0x244)) {
            break;
        }
        pos.y -= 64.0f;
        if (!fn_8038AD24(field_0x244, lbl_804A37E0.mMsgs[field_0x254], mSerial, field_0x4, pos, 0x100)) {
            break;
        }
        field_0x230 = 2;
    case 2:
        fn_8022FE80(field_0x244, field_0x4, mSerial, false, false);
        if (fn_803865B8(field_0x244)) {
            fn_80384C58(field_0x244, mSerial);
            switch (fn_8037EFB4(field_0x244)) {
            case 0:
                field_0x230 = 4;
                break;
            case 1:
                fn_80230534(false);
                field_0x230 = 3;
                break;
            }
        }
        break;
    case 3: {
        s32 far = 0;
        for (s32 i = 0; i < gGameRoot_8053AAF8->fn_80138A30(); i++) {
            f32 dx = getPlayerPos_8023E724(i)->x - mPos.x;
            f32 dy = getPlayerPos_8023E724(i)->y - mPos.y;
            if (std::sqrtf(dx * dx + dy * dy) > 33.0f) {
                far++;
            }
        }
        if (far >= gGameRoot_8053AAF8->fn_80138A30()) {
            field_0x230 = 1;
        }
        break;
    }
    case 4:
        if (getActorManager_801F856C()->fn_801F68E0() == 0) {
            break;
        }
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i) && field_0x4 == getPlayerLayer_8023DE58(i)) {
                JGeometry::TVec3f* ppos = getPlayerPos_8023E724(i);
                field_0x264[i].x = ppos->x;
                field_0x264[i].y = ppos->y;
                field_0x264[i].z = ppos->z;
                field_0x264[i].z = 0.0f;
                if (fn_8022F3DC(i) < 0) {
                    fn_8022CA6C(i, 5, false);
                }
                if (fn_80236B50(i)) {
                    fn_8023764C(i, 1);
                }
            }
        }
        lbl_80529DEC.field_0x24e = 1;
        field_0x230 = 5;
    case 5: {
        JGeometry::TVec3f target(mPos.x, mPos.y, mPos.z);
        s32 done = 0;
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i)) {
                if (i != 0) {
                    target.y -= 0.2f;
                }
                addCalc2_801F71A4(&field_0x264[i].x, target.x, 0.3f, 1.0f);
                addCalc2_801F71A4(&field_0x264[i].y, target.y, 0.3f, 1.0f);
                f32 dy = field_0x264[i].y - target.y;
                f32 dx = field_0x264[i].x - target.x;
                if (std::sqrtf(dx * dx + dy * dy) < 1.0f) {
                    done++;
                    fn_80230498(i, &field_0x264[i], 0x3F, 0xFF, -0x80, -0x80, true);
                } else {
                    fn_80230BF4(i, fn_801F681C((s32)(JMath::atanTable_.atan2_(-dx, -dy) * 10430.378f)));
                    fn_802303FC(i, &field_0x264[i], true);
                }
            } else {
                done++;
            }
        }
        if (done >= 4) {
            field_0x230 = 6;
        }
        break;
    }
    case 6:
        if (mTimers[0] == 0) {
            JGeometry::TVec3f target(mPos.x, mPos.y, mPos.z);
            for (s32 i = 0; i < 4; i++) {
                if (fn_8023F264(i) && field_0x4 == getPlayerLayer_8023DE58(i)) {
                    if (i != 0) {
                        target.y -= 0.2f;
                    }
                    addCalc2_801F71A4(&field_0x264[i].x, target.x, 0.3f, 1.0f);
                    addCalc2_801F71A4(&field_0x264[i].y, target.y, 0.3f, 1.0f);
                    // FIXME(shotgun): the original also computes this distance (fabs form) and
                    // compares it with 2.0f without using the result.
                    if (field_0x240 == 0) {
                        fn_80230498(i, &field_0x264[i], 0x3C, 0xFF, -0x80, -0x80, true);
                    } else {
                        fn_80230498(i, &field_0x264[i], 0x3E, 0xFF, -0x80, -0x80, true);
                    }
                }
            }
            mTimers[0] = 8;
            field_0x240++;
            if (field_0x240 > 1) {
                f32 offset = -48.0f;
                for (s32 i = 0; i < 3; i++) {
                    JGeometry::TVec3f spawnPos = mPos;
                    spawnPos.x = mPos.x + offset;
                    offset += 48.0f;
                    getActorManager_801F856C()->spawnActor_801F9484(ACTOR_TLWP, field_0x4, &spawnPos, (i + 1) << 4, -1, -1);
                }
                field_0x230 = 7;
            }
        }
        break;
    case 7:
        if (lbl_8053A118 == 0x309) {
            gGameRoot_8053AAF8->fn_801365E8(lbl_804A37E0.field_0x28[field_0x254], lbl_804A37E0.field_0x48[field_0x254], 0);
        }
        break;
    }
}

// 0x801B5228.
// FIXME(shotgun): 87.96 %, missing/extra
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
// FIXME(shotgun): 100 %, matches
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
// FIXME(shotgun): 95.52 %, different
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
// FIXME(shotgun): 100 %, matches
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
