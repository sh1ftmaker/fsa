#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Effect.h"
#include "game/GameView.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/cls_804AE9B0.h"
#include "game/EnemyBase.h"
#include "game/Map.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/StageFlag.h"
#include "game/cls_8049D83C.h"
#include "game/GameRootSub20.h"
#include "game/RotMatrix.h"
#include "game/SpriteParams.h"
#include "game/ActorId.h"
#include "game/cls_804A8638.h"
#include "game/act/act_environment_control_2.h"
#include "JSystem/JUtility/JUTVideo.h"

static const s32 lbl_80464830[15] = {
    0x199, 0x1E7, 0x17F, 0x201, 0x1CD, 0x1B3, 0x21B, 0x21B,
    0x14C, 0x173, 0x20E, 0x21A, 0x29B, 0x2B1, 0x2CD,
};

// Frame tables of fn_8011395C (indexed by field_0x240): where the counter restarts, where the
// stage ends, and the crystal slot of each field_0x238.
static const s32 lbl_804648B4[3] = {0, 0xC9, 0x191};
static const s32 lbl_804648C0[3] = {0xC8, 0x190, 0x258};
static const s32 lbl_804648CC[7] = {1, 4, 0, 5, 3, 2, 6};

// 0x80115590.
// FIXME(shotgun): 99.9 %, reloc-name
cls_8049D83C::cls_8049D83C()
    : field_0x230(1), field_0x231(1), field_0x232(0), field_0x233(0), field_0x234(1), field_0x238(0),
      field_0x23c(0), field_0x240(0), field_0x244(0), field_0x248(0), field_0x24c(0), field_0x250(0.0f),
      field_0x254(0.0f), field_0x258(0.0f), field_0x25c(0.0f), field_0x260(NULL) {
    field_0x29c = NULL;
    field_0x2a0 = NULL;
    field_0x2a4 = NULL;
    field_0x2e4 = NULL;
    for (s32 i = 0; i < 7; i++) {
        field_0x2a8[i] = NULL;
        if (i < 2) {
            field_0x2e8[i] = NULL;
        }
        if (i < 3) {
            field_0x2c4[i] = NULL;
        }
        if (i < 5) {
            field_0x2d0[i] = NULL;
        }
    }
    gAudioMgr_8053AF80->fn_80458F9C(0x3C);
}

// 0x80115430.
// FIXME(shotgun): 99.0 %, regalloc-only
cls_8049D83C::~cls_8049D83C() {
    if (field_0x2a0 != NULL) {
        releaseEmitter_801EE434(field_0x2a0);
        field_0x2a0 = NULL;
    }
    if (field_0x29c != NULL) {
        releaseEmitter_801EE434(field_0x29c);
        field_0x29c = NULL;
    }
    for (s32 i = 0; i < 7; i++) {
        releaseEmitter_801EE434(field_0x2a8[i]);
        field_0x2a8[i] = NULL;
        if (i < 2) {
            releaseEmitter_801EE434(field_0x2e8[i]);
            field_0x2e8[i] = NULL;
        }
        if (i < 3) {
            releaseEmitter_801EE434(field_0x2c4[i]);
            field_0x2c4[i] = NULL;
        }
        if (i < 5) {
            releaseEmitter_801EE434(field_0x2d0[i]);
            field_0x2d0[i] = NULL;
        }
    }
    if (field_0x2a4 != NULL) {
        releaseEmitter_801EE434(field_0x2a4);
        field_0x2a4 = NULL;
    }
    if (field_0x2e4 != NULL) {
        releaseEmitter_801EE434(field_0x2e4);
        field_0x2e4 = NULL;
    }
}

// FIXME(shotgun): the next three tables and lbl_80539F00 are declared, not defined: this file does not
// carve their .rodata / .data / .sdata yet.
extern const s16* lbl_8049D820[7];
extern const u16 lbl_804648E8[5];
extern const s32 lbl_804648F4[5];
extern const char* lbl_80539F00;
extern const s32 lbl_8046486C[18];
// FIXME(shotgun): vfunc_0x60's effect id tables (u16, read with lhz). They sit inside the
// 0x74-byte lbl_804648F4 of symbols.txt (after its five words), which this file does not carve
// yet; declared by address until it does.
extern const u16 lbl_80464908[7];
extern const u16 lbl_80464918[];
extern const u16 lbl_80464928[];
extern const u16 lbl_80464938[];
extern const u16 lbl_80464948[];
extern const u16 lbl_80464958[];

// 0x80115248.
// FIXME(shotgun): 95.5 %, scheduling+regalloc + reloc-name
void cls_8049D83C::init() {
    cls_8049D140::init();
    field_0x238 = field_0x90 & 0xFF;
    field_0x244 = (field_0x90 >> 22) & 0x1F;
    field_0x260 = gGameRoot_8053AAF8->field_0xa4->getResource('????', lbl_80539F00);
    mAnim.fn_801D2608(lbl_8049D820, 7);
    switch (field_0x238) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        mAnim.startAnim_801D22DC(4, 2, false);
        break;
    case 6:
    default:
        mAnim.startAnim_801D22DC(6, 2, false);
        break;
    }
    mAnim.fn_801D1F14(0.0f);
    mPos.y -= 32.0f;
    JGeometry::TVec3f pos[5];
    pos[0] = mPos;
    pos[1] = JGeometry::TVec3f(mSpawnPos.x - 80.0f, mSpawnPos.y - 48.0f, 0.0f);
    pos[2] = JGeometry::TVec3f(mSpawnPos.x + 80.0f, mSpawnPos.y - 48.0f, 0.0f);
    pos[3] = JGeometry::TVec3f(mSpawnPos.x - 80.0f, mSpawnPos.y + 48.0f, 0.0f);
    pos[4] = JGeometry::TVec3f(mSpawnPos.x + 80.0f, mSpawnPos.y + 48.0f, 0.0f);
    for (s32 i = 0; i < 5; i++) {
        field_0x2d0[i] = spawnEmitter_8013CC50(field_0x4, &pos[i], lbl_804648E8[i], 0, lbl_804648F4[i], NULL, NULL, true, 1.0f);
    }
    getGameView_802D6A9C()->fn_802D7440(0);
}

// 0x80114C98.
// FIXME(shotgun): 96.7 %, missing/extra (case 5: the original keeps the flag test and the input test in a bool temporary)
void cls_8049D83C::update() {
    cls_8049D140::update();
    switch (field_0x23c) {
    case 0:
    case 1:
    case 10:
    case 17:
        break;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16: {
        JGeometry::TVec3f focus = mPos;
        focus.y += 32.0f;
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&focus);
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
        if (camera->field_0x7f5 == 0) {
            camera->field_0x7f4 = 1;
            camera->field_0x7f8 = 4;
            camera->field_0x7fc.x = focus.x;
            camera->field_0x7fc.y = focus.y;
            camera->field_0x7fc.z = focus.z;
            camera->field_0x7f6 = 0;
        }
        break;
    }
    default: {
        JGeometry::TVec3f focus = mPos;
        focus.y += 16.0f + (mSpawnPos.y - mPos.y - 32.0f);
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&focus);
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
        if (camera->field_0x7f5 == 0) {
            camera->field_0x7f4 = 1;
            camera->field_0x7f8 = 4;
            camera->field_0x7fc.x = focus.x;
            camera->field_0x7fc.y = focus.y;
            camera->field_0x7fc.z = focus.z;
            camera->field_0x7f6 = 0;
        }
        break;
    }
    }
    switch (field_0x23c) {
    case 0:
        gAudioMgr_8053AF80->fn_80458FF0(0x100001B, false, 0);
        vfunc_0x60(1);
        break;
    case 3:
        for (s32 i = 0; i < 4; i++) {
            s32 level = fn_8023BCD4(i) - 2;
            if (level <= 0x1E) {
                level = 0;
            }
            JGeometry::TVec3f playerPos = *getPlayerPos_8023E724(i);
            fn_80230498(i, &playerPos, fn_8023CBC0(i), level, -0x80, -0x80, true);
        }
        if (fn_8011921C()->field_0x8 == 0x1B8) {
            gAudioMgr_8053AF80->fn_80458F9C(0x3C);
        }
        if (fn_8016A3AC()->field_0x1ef == 0 && field_0x2e4 == NULL) {
            field_0x2e4 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x54C, 0, 0, NULL, NULL, true, 1.0f);
        }
        break;
    case 4: {
        fn_801CD950(&field_0x25c, 0.02f, 0.0004f);
        fn_801CD950(&field_0x254, 0.15f, 0.01f);
        mPos.y -= field_0x254;
        f32 limit = mSpawnPos.y - 48.0f;
        if (mPos.y <= limit) {
            mPos.y = limit;
        }
        field_0x248 += 2;
        if (field_0x248 >= 0xE1) {
            field_0x248 = 0xE1;
        }
        break;
    }
    case 5:
        if (isStageFlagSet(field_0x238, 3)) {
            gGameRoot_8053AAF8->field_0x85 = 2;
        }
        field_0x248 = 0xE1;
        if (fn_8011921C()->field_0x1 == 0) {
            fn_801CD950(&field_0x25c, 0.0f, -0.0002f);
        }
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_803865B8(0)) {
            fn_80388EEC(0);
            fn_8011921C()->fn_801183BC();
            break;
        }
        if (isStageFlagSet(field_0x238, 3) && lbl_8053AD30->fn_80309C4C(0x1000, -1) != 0) {
            fn_80387494(0);
            fn_8011921C()->fn_801183BC();
        }
        break;
    case 6:
        switch (fn_8011921C()->field_0x8) {
        case 0x43D:
            startSound_801F0E34(0x30082, 0);
            break;
        case 0x46A:
            startSound_801F0E34(0x30082, 0);
            startSound_801F0E34(0x30083, 0);
            break;
        }
        break;
    case 11:
        fn_801CD950(&mPos.y, mSpawnPos.y, 0.3f);
        break;
    case 13:
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_803865B8(0)) {
            fn_80388EEC(0);
            fn_8011921C()->fn_801183BC();
        }
        break;
    case 16:
        for (s32 i = 0; i < 2; i++) {
            if (field_0x2e8[i] != NULL) {
                setEmitterPos_8013CB44(field_0x2e8[i], &mPos, 0, 0, true);
                field_0x2e8[i]->mGlobalEnvClr.r = 0x2D;
                field_0x2e8[i]->mGlobalEnvClr.g = 0x78;
                field_0x2e8[i]->mGlobalEnvClr.b = 0xFF;
            }
        }
        break;
    }
    if (field_0x233 != 0) {
        field_0x258 += field_0x25c;
    }
    mAnim.fn_801D1C18();
    if (field_0x230 != 0) {
        fn_8011395C();
    }
}

// 0x80114C1C.
void cls_8049D83C::draw() {
    cls_8049D140::draw();
    fn_80113790();
    switch (field_0x23c) {
    case 0x10:
        fn_80113B54(0x1D6);
        break;
    default:
        if (field_0x232 != 0) {
            fn_80113B54(lbl_80464830[field_0x238]);
        }
        break;
    }
}

// 0x80114860.
// FIXME(shotgun): 99.5 %, missing/extra
void cls_8049D83C::vfunc_0x54() {
    cls_801192AC* sub = fn_8011921C();
    s32 t = sub->field_0x8;
    if (field_0x238 == 0) {
        if (t == lbl_8046486C[2]) {
            vfunc_0x60(2);
        } else if (t == lbl_8046486C[3]) {
            vfunc_0x60(3);
        } else if (t == lbl_8046486C[4]) {
            vfunc_0x60(4);
        } else if (t == lbl_8046486C[5]) {
            vfunc_0x60(5);
        } else if (t == lbl_8046486C[6]) {
            vfunc_0x60(6);
        } else if (t == lbl_8046486C[7]) {
            vfunc_0x60(7);
        } else if (t == lbl_8046486C[9]) {
            vfunc_0x60(9);
        } else if (t == lbl_8046486C[11]) {
            vfunc_0x60(11);
        } else if (t == lbl_8046486C[12]) {
            vfunc_0x60(12);
        } else if (t == lbl_8046486C[13]) {
            vfunc_0x60(13);
        } else if (t == lbl_8046486C[14]) {
            vfunc_0x60(14);
        } else if (t == lbl_8046486C[15]) {
            vfunc_0x60(15);
        } else if (t == lbl_8046486C[16]) {
            vfunc_0x60(16);
        } else if (t == lbl_8046486C[17]) {
            vfunc_0x60(17);
        }
    } else {
        if (t == lbl_8046486C[2]) {
            vfunc_0x60(2);
        } else if (t == lbl_8046486C[3]) {
            vfunc_0x60(3);
        } else if (t == lbl_8046486C[4]) {
            vfunc_0x60(4);
        } else if (t == lbl_8046486C[5]) {
            vfunc_0x60(5);
        } else if (t == lbl_8046486C[6]) {
            vfunc_0x60(6);
        } else if (t == lbl_8046486C[7]) {
            vfunc_0x60(7);
        } else if (t == lbl_8046486C[9]) {
            vfunc_0x60(9);
        } else if (t == lbl_8046486C[10]) {
            vfunc_0x60(10);
        }
    }
}

// 0x80113EC4.
// FIXME(shotgun): 84.4 %, missing/extra + reloc-name
void cls_8049D83C::vfunc_0x60(s32 arg) {
    switch (arg) {
    case 2:
        field_0x234 = 0;
        for (s32 i = 0; i < 5; i++) {
            field_0x2d0[i]->becomeInvalidEmitter();
            field_0x2d0[i] = NULL;
        }
        fn_8011921C()->field_0x2 = 1;
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f playerPos = *getPlayerPos_8023E724(i);
            fn_80230498(i, &playerPos, fn_8023CBC0(i), 0xFF, 0, 0, true);
        }
        break;
    case 3: {
        field_0x234 = 1;
        field_0x240 = 1;
        field_0x24c = lbl_804648B4[1];
        field_0x2c4[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x2AE, 0, 2, NULL, NULL, true, 0.5f);
        field_0x2c4[1] = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80464938[field_0x238], 0, 2, NULL, NULL, true, 0.5f);
        field_0x2c4[2] = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80464948[field_0x238], 0, 2, NULL, NULL, true, 0.5f);
        field_0x2a4 = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80464958[field_0x238], 0, 2, NULL, NULL, true, 1.0f);
        s32 swevSerial = getActorManager_801F856C()->fn_801F7A08(ACTOR_SWEV);
        ActorBase_804A5770* swev = getActorManager_801F856C()->findActorBySerial_801F666C(swevSerial);
        if (swev != NULL) {
            static_cast<cls_804A8638*>(swev)->fn_802B3A3C();
        }
        s32 sev2Serial = getActorManager_801F856C()->fn_801F7A08(ACTOR_SEV2);
        ActorBase_804A5770* sev2 = getActorManager_801F856C()->findActorBySerial_801F666C(sev2Serial);
        if (sev2 != NULL) {
            static_cast<EnvironmentControl2_804A9CD0*>(sev2)->fn_802E43E0();
        }
        startSound_801F0E34(0x400D5, 0);
        break;
    }
    case 4:
        if (field_0x238 == 6) {
            gAudioMgr_8053AF80->fn_80458FF0(0x100000F, false, 0);
        } else {
            gAudioMgr_8053AF80->fn_80458FF0(0x100001C, false, 0);
        }
        field_0x233 = 1;
        break;
    case 5: {
        JGeometry::TVec3f pos;
        pos.z = 0.0f;
        pos.y = (s32)JUTVideo::getManager()->getRenderMode()->efbHeight * 0.5f + -16.0f;
        pos.x = (s32)JUTVideo::getManager()->getRenderMode()->fbWidth * 0.5f;
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        // The message ids per field_0x238 are the last seven words of lbl_80464830.
        if (fn_8038BD08(0, lbl_80464830[8 + field_0x238], field_0x4, pos, 0x62)) {
            fn_8011921C()->fn_80118454();
        }
        break;
    }
    case 6:
        field_0x233 = 0;
        for (s32 i = 0; i < 7; i++) {
            field_0x2a8[i] = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80464908[i], 0, 2, NULL, NULL, true, 0.5f);
        }
        field_0x29c = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80464918[field_0x238], 0, 2, NULL, NULL, true, 0.5f);
        field_0x2a0 = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80464928[field_0x238], 0, 2, NULL, NULL, true, 1.0f);
        startSound_801F0E34(0x30082, 0);
        fn_8011921C()->field_0x2 = 0;
        break;
    case 7:
        field_0x230 = 0;
        field_0x232 = 1;
        break;
    case 8:
        if (field_0x2e4 != NULL) {
            field_0x2e4->becomeInvalidEmitter();
            field_0x2e4 = NULL;
        }
        break;
    case 9: {
        fn_80119DD0()->fn_80119998(0xB4, 0, 0, 0, 1, 0, 0);
        field_0x248 = 1;
        JGeometry::TVec3f pos(mSpawnPos.x - 36.0f, mSpawnPos.y + 48.0f, mSpawnPos.z);
        for (s32 i = 0; i < 4; i++) {
            if (i > 0) {
                pos.x += 24.0f;
            }
            fn_80230498(i, &pos, 0x3F, 0xFF, -0x80, -0x80, true);
            fn_80230BF4(i, 3);
        }
        if (field_0x238 == 0) {
            fn_8011921C()->field_0x2 = 1;
        } else {
            field_0x232 = 0;
        }
        if (field_0x2a0 != NULL) {
            releaseEmitter_801EE434(field_0x2a0);
            field_0x2a0 = NULL;
        }
        if (field_0x29c != NULL) {
            releaseEmitter_801EE434(field_0x29c);
            field_0x29c = NULL;
        }
        for (s32 i = 0; i < 7; i++) {
            releaseEmitter_801EE434(field_0x2a8[i]);
            field_0x2a8[i] = NULL;
            if (i < 2) {
                releaseEmitter_801EE434(field_0x2e8[i]);
                field_0x2e8[i] = NULL;
            }
            if (i < 3) {
                releaseEmitter_801EE434(field_0x2c4[i]);
                field_0x2c4[i] = NULL;
            }
            if (i < 5) {
                releaseEmitter_801EE434(field_0x2d0[i]);
                field_0x2d0[i] = NULL;
            }
        }
        if (field_0x2a4 != NULL) {
            releaseEmitter_801EE434(field_0x2a4);
            field_0x2a4 = NULL;
        }
        if (field_0x2e4 != NULL) {
            releaseEmitter_801EE434(field_0x2e4);
            field_0x2e4 = NULL;
        }
        if (field_0x238 != 0 && field_0x238 < 6) {
            JGeometry::TVec3f spawnPos(mSpawnPos.x, mSpawnPos.y, 20.0f);
            getActorManager_801F856C()->spawnActor_801F9484('CRNV', field_0x4, &spawnPos, field_0x238, -1, -1);
        }
        break;
    }
    case 10:
        lbl_8053AD70->fn_8037AD00(field_0x238);
        field_0x248 = 0;
        fn_801F3668();
        if (field_0x244 > 0) {
            getGameView_802D6A9C()->fn_802D8050(field_0x244, true, 0xFF);
        }
        break;
    case 11:
        field_0x232 = 1;
        field_0x254 = 0.0f;
        mAnim.startAnim_801D22DC(4, 2, false);
        mAnim.fn_801D1F14(0.0f);
        break;
    case 12:
        mAnim.startAnim_801D22DC(5, 0, false);
        mAnim.fn_801D1F14(0.0f);
        break;
    case 13:
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        fn_8022FD8C(mSerial);
        fn_8038BD08(0, 0x14B, field_0x4, JGeometry::TVec3f(0.0f, -16.0f, 0.0f), 0x4022);
        fn_8011921C()->fn_80118454();
        break;
    case 15:
        mDrawEnabled = false;
        mPos.z = 20.0f;
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x20, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x21, 0, 0, NULL, NULL, true, 1.0f);
        startSound_801F0E34(0x400D9, 0);
        break;
    case 16:
        mDrawEnabled = true;
        field_0x2e8[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x32C, 0, 0, NULL, NULL, true, 1.0f);
        field_0x2e8[1] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x183, 0, 0, NULL, NULL, true, 1.0f);
        mAnim.startAnim_801D22DC(2, 2, false);
        mAnim.fn_801D1F14(0.0f);
        break;
    case 17:
        fn_8011921C()->field_0x2 = 0;
        lbl_8053AD70->fn_8037AD00(field_0x238);
        field_0x248 = 0;
        if (field_0x2e8[0] != NULL) {
            field_0x2e8[0]->becomeInvalidEmitter();
            field_0x2e8[0] = NULL;
        }
        if (field_0x2e8[1] != NULL) {
            field_0x2e8[1]->becomeInvalidEmitter();
            field_0x2e8[1] = NULL;
        }
        fn_801F3668();
        if (field_0x244 > 0) {
            getGameView_802D6A9C()->fn_802D8050(field_0x244, true, 0xFF);
            if (field_0x238 < 6) {
                JGeometry::TVec3f spawnPos = mSpawnPos;
                spawnPos.z = 20.0f;
                getActorManager_801F856C()->spawnActor_801F9484('CRNV', field_0x4, &spawnPos, field_0x238, -1, -1);
            }
        }
        break;
    }
    field_0x23c = arg;
}

// 0x80113E98.
s32 cls_8049D83C::vfunc_0x5C() {
    switch (field_0x23c) {
    case 0xA:
    case 0x11:
        return 1;
    default:
        return 0;
    }
}

// Not auto-inlined: draw calls fn_80113790 out of line in the original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// Darkens the screen: draws the image field_0x260 scaled by 96 with an alpha of field_0x248.
// 0x80113790.
// FIXME(shotgun): 74.0 %, missing/extra + reloc-name
void cls_8049D83C::fn_80113B54(s32 arg) {
    SpriteParams_801F5AE0 params;
    SpriteParams_801F5AE0 params2;
    JGeometry::TVec3f pos(mPos.x, mSpawnPos.y, mPos.z);
    s32 frame = arg + mAnim.mCel;
    const char* name;
    if (arg == 0x1D6) {
        name = "enemy_both2_sob.bin";
    } else {
        name = "enemy_gc_sob.bin";
    }
    fn_801EF830(field_0x4, &pos, &params2, 7, name, frame, NULL);
    if (field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', name);
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
    }
}

// FIXME(shotgun): 84.8 %, missing/extra + reloc-name
void cls_8049D83C::fn_8011395C() {
    Mtx mtx;
    PSMTXTrans(mtx, mPos.x, mPos.y, mPos.z);
    JGeometry::TVec3f axisX(1.0f, 0.0f, 0.0f);
    fn_8010F668(mtx, &axisX, 0.08726646f);
    Mtx rot;
    for (s32 i = 0; i < 3; i++) {
        for (s32 j = 0; j < 4; j++) {
            rot[i][j] = 0.0f;
        }
    }
    JGeometry::TVec3f axisZ(0.0f, 0.0f, 1.0f);
    fn_8010F668(rot, &axisZ, field_0x258);
    PSMTXConcat(mtx, rot, mtx);
    if (field_0x234 != 0) {
        field_0x24c++;
    }
    switch (field_0x240) {
    case 0:
    case 2:
        if (field_0x24c >= lbl_804648C0[field_0x240]) {
            field_0x24c = lbl_804648B4[field_0x240];
        }
        break;
    case 1:
        if (field_0x24c >= lbl_804648C0[field_0x240]) {
            field_0x240 = 2;
        }
        break;
    }
    s32 frame = field_0x24c;
    GameRootSub20_801902C0::Crystal& crystal = fn_801902C0()->mCrystals[lbl_804648CC[field_0x238]];
    if (crystal.mModel != NULL && crystal.mActive == 0) {
        crystal.mActive = 1;
        crystal.mPos.x = 0.5f;
        crystal.mPos.y = 0.5f;
        crystal.mPos.z = 0.5f;
        PSMTXCopy(mtx, crystal.mMtx);
        crystal.field_0x48 = -1.0f;
        crystal.field_0x4c = frame;
        crystal.field_0x54 = 0;
        crystal.field_0x58 = 2;
    }
}

// FIXME(shotgun): 95.13 %, different + reloc-name
void cls_8049D83C::fn_80113790() {
    if (field_0x248 > 0) {
        SpriteParams_801F5AE0 params;
        Mtx mtx;
        PSMTXTrans(mtx, 0.0f, 0.0f, 0.0f);
        mtx[0][0] = 96.0f;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 1.0f;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = 96.0f;
        params.field_0x10.r = 0;
        params.field_0x10.g = 0;
        params.field_0x10.b = 0;
        params.field_0x10.a = field_0x248;
        params.mMtx = mtx;
        params.field_0x1d = -1;
        params.field_0x2d = 1;
        JGeometry::TVec3f pos = mPos;
        pos.z = 0.0f;
        if (field_0x4 < 8) {
            gGameRoot2_8053AB10->field_0x20->fn_802F745C(&pos, field_0x260, &params);
        }
    }
}
#pragma auto_inline reset

// 0x80113788.
s32 cls_8049D83C::vfunc_0x58() {
    return field_0x23c;
}
