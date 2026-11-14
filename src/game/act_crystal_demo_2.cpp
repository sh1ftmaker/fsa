// Crystal (cutscene) actor: 'DCR2', spawned by tu_801F9484.cpp.
// Declared in include/game/act/tu_801F9484.h (cls_804AE088).
// Inherits from cls_8049D140 (tu_8010DF44).

#include "game/ActorBase.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "game/GameRootSub20.h"
#include "game/RotMatrix.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/tu_801F9484.h"
#include "game/act/tu_8010DF44.h"
#include "game/SpriteAnim.h"
#include "game/Unk_8045A080.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JMath/JMath.h"
#include "game/Camera.h"
#include "game/GameState.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/StageFlag.h"
#include "game/EnemyBase.h"
#include "game/act/act_navi_maiden_crystal.h"
#include "game/cls_804A8638.h"
#include "game/cls_804AE9B0.h"

// FIXME(shotgun): not written, .rodata tables lbl_80474C38 (string),
// lbl_80474C68 (0x1E0 table), lbl_80474E48 (two filenames) and the .sdata/.sdata2 anim tables are not
// declared: nothing compiled here reads them yet.
// FIXME(shotgun): not written: init, vfunc_0x60 (jumptable_804AE184),
// fn_8036AC58, fn_8036A65C, fn_80369DDC (jumptable_804AE13C),
// fn_80368660, fn_803683AC, fn_80368198.

// Functions are listed in descending address order (highest first, lowest last).

s32 cls_804AE088::vfunc_0x58() {
    return field_0x240;
}

// FIXME: 98.05 %, scheduling + reloc-name; behaviour clean.
cls_804AE088::cls_804AE088()
    : mNaviActor(NULL), field_0x234(1), field_0x235(1), field_0x236(0), field_0x237(0), field_0x238(1),
      field_0x23c(0), field_0x240(0), field_0x244(0), field_0x248(0), field_0x24c(0), field_0x250(0.0f),
      field_0x254(0), field_0x258(0), field_0x25c(-1), field_0x26c(0), field_0x270(0), field_0x274(0),
      field_0x278(0.0f), field_0x27c(0.0f), field_0x280(0.0f), field_0x284(0.0f), field_0x288(0.0f),
      field_0x28c(-1), field_0x290(0.0f), field_0x2f0(0), field_0x310(NULL), field_0x314(NULL) {
    field_0x350 = NULL;
    field_0x354 = NULL;
    field_0x358 = NULL;
    field_0x398 = NULL;
    for (s32 i = 0; i < 7; i++) {
        if (i < 2) {
            field_0x35c[i] = NULL;
            field_0x3b4[i] = NULL;
        }
        if (i < 3) {
            field_0x378[i] = NULL;
        }
        if (i < 5) {
            field_0x384[i] = NULL;
            field_0x3ec[i] = NULL;
            field_0x400[i] = NULL;
            field_0x294[i] = 0;
        }
        if (i < 6) {
            field_0x3bc[i] = NULL;
            field_0x3d4[i] = NULL;
            field_0x39c[i] = NULL;
            field_0x2f8[i] = 0;
        }
    }
    gAudioMgr_8053AF80->fn_80458F9C(60);
}

cls_804AE088::~cls_804AE088() {
    if (field_0x314 != NULL) {
        delete field_0x314;
    }
    field_0x314 = NULL;
    fn_8036A65C();
}

void (cls_804AE088::*lbl_804AE058[2])() = {&cls_804AE088::fn_80369DDC, &cls_804AE088::fn_80368CD8};
void (cls_804AE088::*lbl_804AE070[2])() = {lbl_804AE058[0], lbl_804AE058[1]};

static const s16 lbl_80542C48[2] = {9, -1};
static const s16 lbl_80542C50[2] = {9, -1};
static const s16* lbl_8053A1C0[2] = {lbl_80542C48, lbl_80542C50};

const char* lbl_8053A1C8 = "kk_black.bti";

static const s16 sEmitterAngles[5] = {-5461, 5461, 16384, 27307, -27307};
static const u16 sEmitterIds[5] = {0x105, 0x106, 0x107, 0x108, 0x109};
static const s32 sEmitterDepths[5] = {2, 0, 0, 0, 0};

// FIXME: 90.98 %, different + reloc-name; behaviour drift.
void cls_804AE088::init() {
    cls_8049D140::init();
    field_0x23c = field_0x90 & 0x1F;
    field_0x248 = (field_0x90 >> 22) & 0x1F;
    if (field_0x23c != 4 && field_0x23c != 5) {
        fn_801F3668();
        return;
    }
    if (field_0x23c == 5) {
        field_0x240 = 0x13;
        field_0x268 = 5;
    } else {
        field_0x268 = 0;
    }
    field_0x310 = gGameRoot_8053AAF8->field_0xa4->getResource('?\?\?\?', lbl_8053A1C8);
    field_0x318.fn_801D2608(lbl_8053A1C0, 2);
    switch (field_0x23c) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        field_0x318.startAnim_801D22DC(0, 2, false);
        break;
    case 6:
    default:
        field_0x318.startAnim_801D22DC(1, 2, false);
        break;
    }
    field_0x318.fn_801D1F14(0.0f);
    mPos.y -= 32.0f;
    JGeometry::TVec3f pts[5] = {
        mPos,
        JGeometry::TVec3f(mSpawnPos.x - 80.0f, mSpawnPos.y - 48.0f, 0.0f),
        JGeometry::TVec3f(mSpawnPos.x + 80.0f, mSpawnPos.y - 48.0f, 0.0f),
        JGeometry::TVec3f(mSpawnPos.x - 80.0f, mSpawnPos.y + 48.0f, 0.0f),
        JGeometry::TVec3f(mSpawnPos.x + 80.0f, mSpawnPos.y + 48.0f, 0.0f),
    };
    for (s32 i = 0; i < 5; i++) {
        field_0x384[i] = spawnEmitter_8013CC50(field_0x4, &pts[i], sEmitterIds[i], 0, sEmitterDepths[i], NULL,
                                               NULL, true, 1.0f);
    }
    getGameView_802D6A9C()->fn_802D7440(0);
    for (s32 i = 0; i < 5; i++) {
        s16 angle = sEmitterAngles[i];
        field_0x2a8[i].x = 55.0f * JMath::sincosTable_.cosShort(angle) + mPos.x;
        field_0x2a8[i].y = 32.0f + (35.0f * JMath::sincosTable_.sinShort(angle) + mPos.y);
        field_0x2a8[i].z = 0.0f;
    }
    field_0x2a8[5] = mPos;
    s32 count = getActorManager_801F856C()->mActorEnd;
    for (s32 i = 0; i < count; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
        if (actor != NULL && actor->mActorId == ACTOR_GOL2) {
            field_0x2f4 = (cls_804A9C20*)actor;
            break;
        }
    }
}

void cls_804AE088::update() {
    if (field_0x2f0 != 0) {
        field_0x2f0--;
        if (field_0x2f0 == 0xE1) {
            startSound_801F0E34(0x30082, 0);
        } else if (field_0x2f0 == 0xB4) {
            startSound_801F0E34(0x30082, 0);
            startSound_801F0E34(0x30083, 0);
        }
    }
    (this->*lbl_804AE070[field_0x23c - 4])();
}

const s32 lbl_80474C48[8] = {0x199, 0x1E7, 0x17F, 0x201, 0x1CD, 0x1B3, 0x21B, 0x21B};

void cls_804AE088::draw() {
    JGeometry::TVec3f savedPos = mPos;
    if (field_0x23c == 5 && field_0x240 >= 0x24 && field_0x240 != 0x26) {
        fn_803683AC();
        return;
    }
    cls_8049D140::draw();
    fn_8036A894();
    if (field_0x236 != 0) {
        fn_8036AC58(lbl_80474C48[field_0x23c]);
    }
    fn_80368660();
    mPos = savedPos;
}

void cls_804AE088::vfunc_0x54() {
    s32 v = fn_8011921C()->field_0x8;
    s32 arg;
    if (field_0x270 == 1) {
        return;
    }
    if (field_0x23c == 4) {
        if (v == 0) {
            arg = 2;
        } else if (v == 0x96) {
            arg = 3;
        } else if (v == 0x1FE) {
            arg = 4;
        } else if (v == 0x32A) {
            arg = 5;
        } else if (v == 0x3F2) {
            arg = 0xE;
        } else if (v == 0x51E) {
            arg = 0xF;
        } else if (v == 0x5B4) {
            arg = 0x10;
        } else if (v == 0x6E0) {
            arg = 0x11;
        } else {
            return;
        }
    } else {
        if (v == 0) {
            arg = 0x15;
        } else if (v == 0x96) {
            arg = 0x16;
        } else if (v == 0x1FE) {
            arg = 0x17;
        } else if (v == 0x32A) {
            arg = 0x18;
        } else if (v == 0x3F2) {
            arg = 0x1A;
        } else if (v == 0x51E) {
            arg = 0x1B;
        } else if (v == 0x5B4) {
            arg = 0x1C;
        } else if (v == 0x6E0) {
            arg = 0x26;
        } else {
            return;
        }
    }
    vfunc_0x60(arg);
}

// Table lbl_80474C68 (0x1E0 bytes) is read by the original; its definition is not written yet.
extern const s32 lbl_80474C68[120];

static const s32 lbl_80474C8C[3] = {0, 0xC9, 0x191};
static const s32 lbl_80474CB0[5] = {0x1E, 0x37, 0x0A, 0x2D, 0x3C};
static const u16 lbl_80474D00[7] = {0x2AB, 0x2AC, 0x2AD, 0x2C4, 0x2C5, 0x2C7, 0x2C6};
static const u16 lbl_80474D10[7] = {0x2C8, 0x2CD, 0x2CA, 0x2C9, 0x2CC, 0x2CB, 0x2CE};
static const u16 lbl_80474D20[7] = {0x30B, 0x310, 0x30D, 0x30C, 0x30F, 0x30E, 0x311};
static const u16 lbl_80474D30[7] = {0x2AF, 0x2BE, 0x2B5, 0x2B2, 0x2BB, 0x2B8, 0x2C1};
static const u16 lbl_80474D40[7] = {0x2B0, 0x2BF, 0x2B6, 0x2B3, 0x2BC, 0x2B9, 0x2C2};
static const u16 lbl_80474D50[7] = {0x2B1, 0x2C0, 0x2B7, 0x2B4, 0x2BD, 0x2BA, 0x2C3};
static const u16 lbl_80474D60[6] = {0x53A, 0x53B, 0x53C, 0x53D, 0x53E, 0x53F};

// differs: the original reads every table at an offset from one base (lbl_80474C38); the tables
// here are copies of parts of lbl_80474C68.
void cls_804AE088::vfunc_0x60(s32 arg) {
    JGeometry::TVec3f pos;
    switch (arg) {
    case 2:
        field_0x238 = 0;
        for (s32 i = 0; i < 5; i++) {
            field_0x384[i]->becomeInvalidEmitter();
            field_0x384[i] = NULL;
        }
        break;
    case 3: {
        field_0x238 = 1;
        field_0x244 = 1;
        field_0x254 = lbl_80474C8C[1];
        field_0x378[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x2AE, 0, 1, NULL, NULL, true, 0.5f);
        field_0x378[1] = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80474D30[field_0x23c], 0, 1, NULL, NULL, true, 0.5f);
        field_0x378[2] = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80474D40[field_0x23c], 0, 1, NULL, NULL, true, 0.5f);
        field_0x358 = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80474D50[field_0x23c], 0, 1, NULL, NULL, true, 1.0f);
        startSound_801F0E34(0x400D5, 0);
        s32 count = getActorManager_801F856C()->mActorEnd;
        for (s32 i = 0; i < count; i++) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
            if (actor != NULL && actor->mActorId == ACTOR_SWEV) {
                static_cast<cls_804A8638*>(actor)->fn_802B3A3C();
                break;
            }
        }
        break;
    }
    case 4:
        switch (field_0x23c) {
        case 6:
            gAudioMgr_8053AF80->fn_80458FF0(0x100000F, false, 0);
            break;
        default:
            gAudioMgr_8053AF80->fn_80458FF0(0x100001C, false, 0);
            break;
        }
        field_0x237 = 1;
        break;
    case 5: {
        JGeometry::TVec3f pos = mPos + JGeometry::TVec3f(0.0f, 160.0f, 20.0f);
        field_0x25c = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_DNAV, field_0x4, &pos, 0, -1, -1);
        mNaviActor = static_cast<Navi54MaidenCrystalCutscene_804ADFE8*>(
            getActorManager_801F856C()->findActorBySerial_801F666C(field_0x25c));
        field_0x258 = 0x3C;
        break;
    }
    case 7: {
        pos.set((s32)JUTVideo::getManager()->getFbWidth() * 0.5f,
                (s32)JUTVideo::getManager()->getEfbHeight() * 0.5f, 0.0f);
        pos.y += -16.0f;
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        fn_8038BD08(0, lbl_80474C68[field_0x268], field_0x4, pos, 0x62);
        field_0x268++;
        mNaviActor->field_0x230 = 1;
        break;
    }
    case 9: {
        pos.set((s32)JUTVideo::getManager()->getFbWidth() * 0.5f,
                (s32)JUTVideo::getManager()->getEfbHeight() * 0.5f, 0.0f);
        pos.y += -16.0f;
        fn_8022FE80(0, field_0x4, field_0x25c, true, false);
        fn_8038BD08(0, lbl_80474C68[field_0x268], field_0x4, pos, 0x62);
        field_0x268++;
        break;
    }
    case 10: {
        pos.set((s32)JUTVideo::getManager()->getFbWidth() * 0.5f,
                (s32)JUTVideo::getManager()->getEfbHeight() * 0.5f, 0.0f);
        pos.y += -16.0f;
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        fn_8038BD08(0, lbl_80474C68[field_0x268], field_0x4, pos, 0x62);
        field_0x268++;
        break;
    }
    case 13: {
        pos.set((s32)JUTVideo::getManager()->getFbWidth() * 0.5f,
                (s32)JUTVideo::getManager()->getEfbHeight() * 0.5f, 0.0f);
        pos.y += -16.0f;
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        fn_8038BD08(0, lbl_80474C68[field_0x268], field_0x4, pos, 0x62);
        field_0x268++;
        break;
    }
    case 12:
        field_0x258 = 0x96;
        break;
    case 14:
        field_0x237 = 0;
        for (s32 i = 0; i < 7; i++) {
            field_0x35c[i] = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80474D00[i], 0, 1, NULL, NULL, true, 0.5f);
        }
        field_0x2f0 = 300;
        startSound_801F0E34(0x30082, 0);
        field_0x350 = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80474D10[field_0x23c], 0, 1, NULL, NULL, true, 0.5f);
        field_0x354 = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80474D20[field_0x23c], 0, 1, NULL, NULL, true, 1.0f);
        field_0x26c = 400;
        break;
    case 15:
        field_0x234 = 0;
        field_0x236 = 1;
        break;
    case 16: {
        field_0x236 = 0;
        fn_80119DD0()->fn_80119998(0xB4, 0, 0, 0, 1, 0, 0);
        field_0x24c = 1;
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f playerPos = *getPlayerPos_8023E724(i);
            fn_80230498(i, &playerPos, fn_8023CBC0(i), 0xFF, -128, -128, true);
        }
        fn_8036A65C();
        JGeometry::TVec3f pos;
        pos.set(mSpawnPos);
        pos.z += 20.0f;
        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_CRNV, field_0x4, &pos, field_0x23c, -1, -1);
        break;
    }
    case 17: {
        field_0x24c = 0;
        fn_801F3668();
        s32 bit = field_0x248;
        if (bit > 0) {
            getGameView_802D6A9C()->fn_802D8050(bit, true, 0xFF);
            lbl_8053AD70->fn_8037AD00(4);
        }
        break;
    }
    case 21:
        field_0x238 = 0;
        for (s32 i = 0; i < 5; i++) {
            field_0x384[i]->becomeInvalidEmitter();
            field_0x384[i] = NULL;
        }
        break;
    case 22: {
        field_0x238 = 1;
        field_0x244 = 1;
        field_0x254 = lbl_80474C8C[1];
        field_0x378[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x2AE, 0, 1, NULL, NULL, true, 0.5f);
        field_0x378[1] = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80474D30[field_0x23c], 0, 1, NULL, NULL, true, 0.5f);
        field_0x378[2] = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80474D40[field_0x23c], 0, 1, NULL, NULL, true, 0.5f);
        field_0x358 = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80474D50[field_0x23c], 0, 1, NULL, NULL, true, 1.0f);
        startSound_801F0E34(0x400D5, 0);
        s32 count = getActorManager_801F856C()->mActorEnd;
        for (s32 i = 0; i < count; i++) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
            if (actor != NULL && actor->mActorId == ACTOR_SWEV) {
                static_cast<cls_804A8638*>(actor)->fn_802B3A3C();
                break;
            }
        }
        break;
    }
    case 23:
        switch (field_0x23c) {
        case 6:
            gAudioMgr_8053AF80->fn_80458FF0(0x100000F, false, 0);
            break;
        default:
            gAudioMgr_8053AF80->fn_80458FF0(0x100001C, false, 0);
            break;
        }
        field_0x237 = 1;
        break;
    case 24: {
        if (field_0x398 == NULL) {
            field_0x398 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x54C, 0, 1, NULL, NULL, true, 1.0f);
        }
        JGeometry::TVec3f pos;
        pos.set((s32)JUTVideo::getManager()->getFbWidth() * 0.5f,
                (s32)JUTVideo::getManager()->getEfbHeight() * 0.5f, 0.0f);
        pos.y += -16.0f;
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_8038BD08(0, lbl_80474C68[field_0x268], field_0x4, pos, 0x62)) {
            field_0x268++;
            fn_8011921C()->fn_80118454();
        }
        break;
    }
    case 25:
        for (s32 i = 0; i < 5; i++) {
            field_0x294[i] = lbl_80474CB0[i] + 0x46;
        }
        break;
    case 26:
        field_0x237 = 0;
        for (s32 i = 0; i < 7; i++) {
            field_0x35c[i] = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80474D00[i], 0, 1, NULL, NULL, true, 0.5f);
        }
        field_0x2f0 = 300;
        startSound_801F0E34(0x30082, 0);
        field_0x350 = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80474D10[field_0x23c], 0, 1, NULL, NULL, true, 0.5f);
        field_0x354 = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80474D20[field_0x23c], 0, 1, NULL, NULL, true, 1.0f);
        field_0x26c = 400;
        break;
    case 27:
        field_0x234 = 0;
        field_0x236 = 1;
        break;
    case 28:
        field_0x24c = 1;
        fn_80119DD0()->fn_80119998(0xB4, 0, 0, 0, 1, 0, 0);
        fn_8036A65C();
        mPos += JGeometry::TVec3f(0.0f, 16.0f, 0.0f);
        field_0x258 = 0xB4;
        for (s32 i = 0; i < 5; i++) {
            deleteEmitter(&field_0x3bc[i]);
            deleteEmitter(&field_0x3d4[i]);
        }
        break;
    case 29:
        field_0x258 = 0xAA;
        break;
    case 31: {
        field_0x264 = 0;
        JGeometry::TVec3f pos = mPos;
        pos.y += 16.0f;
        pos.z = 0.0f;
        field_0x288 = 0.0f;
        field_0x28c = 0;
        for (s32 i = 0; i < 6; i++) {
            field_0x39c[i] = spawnEmitter_8013CC50(field_0x4, &pos, lbl_80474D60[i], 0, 1, NULL, NULL, true, 1.0f);
            field_0x39c[i]->setGlobalAlpha(field_0x288);
        }
        field_0x3b4[0] = spawnEmitter_8013CC50(field_0x4, &pos, 0x540, 0, 1, NULL, NULL, true, 1.0f);
        field_0x3b4[1] = spawnEmitter_8013CC50(field_0x4, &pos, 0x541, 0, 1, NULL, NULL, true, 1.0f);
        field_0x258 = 0x96;
        gAudioMgr_8053AF80->fn_8045873C(1);
        break;
    }
    case 32:
        fn_80119DD0()->fn_80119998(0x96, 0, 1, 0, 0, 0x32, 0);
        field_0x258 = 0xB9;
        startSound_801F0E34(0x40041, 0);
        break;
    case 33:
        fn_80119DD0()->fn_80119998(0x96, 0, 0, 0, 0, 0x19, 0);
        field_0x24c = 1;
        field_0x2f4->fn_802E2990(0xFF);
        for (s32 i = 0; i < 6; i++) {
            deleteEmitterKeep(&field_0x39c[i]);
            field_0x39c[i] = NULL;
        }
        field_0x264 = 10;
        gAudioMgr_8053AF80->fn_8045873C(0);
        break;
    case 34: {
        pos.set((s32)JUTVideo::getManager()->getFbWidth() * 0.5f,
                (s32)JUTVideo::getManager()->getEfbHeight() * 0.5f, 0.0f);
        pos.y += -16.0f;
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        fn_8038BD08(0, lbl_80474C68[field_0x268], field_0x4, pos, 0x62);
        field_0x268++;
        break;
    }
    case 35:
        field_0x258 = 0x78;
        break;
    case 36:
        field_0x258 = 0xF0;
        field_0x2a8[5] = mPos;
        break;
    case 37:
        field_0x258 = 0xF0;
        break;
    case 38: {
        field_0x24c = 0;
        fn_801F3668();
        s32 bit = field_0x248;
        if (bit > 0) {
            getGameView_802D6A9C()->fn_802D8050(bit, true, 0xFF);
            lbl_8053AD70->fn_8037AD00(5);
        }
        break;
    }
    }
    field_0x240 = arg;
}

s32 cls_804AE088::vfunc_0x5C() {
    s32 s = field_0x240;
    if (s == 0x11 || s == 0x26)
        return 1;
    return 0;
}

// differs: register allocation (original keeps &mPos in a register across the draw calls) and the alpha store order.
// FIXME: 93.90 %, missing/extra + reloc-name; behaviour clean.
void cls_804AE088::fn_8036AC58(s32 arg) {
    if (field_0x270 == 1 && field_0x274 < 0x80)
        return;
    SpriteParams_801F5AE0 params;
    s32 frame;
    if (field_0x23c != 5 || field_0x240 < 0x1E || field_0x240 >= 0x21) {
        if (field_0x240 == 0x1C || field_0x240 == 0x1D || field_0x240 == 0x1E || field_0x240 == 0x21 || field_0x240 == 0x22 || field_0x240 == 0x23) {
            frame = field_0x264 + 0x1B3;
            if (mSpriteParams.mMtx == NULL) {
                fn_801EF830(field_0x4, &mPos, &params, 7, "enemy_gc_sob.bin", frame, NULL);
            }
        } else {
            frame = arg + field_0x318.mCel;
            if (mSpriteParams.mMtx == NULL) {
                fn_801EF830(field_0x4, &mPos, &params, 7, "enemy_gc_sob.bin", frame, NULL);
            }
        }
    } else {
        if (field_0x24c < 0xC0) {
            frame = arg + field_0x318.mCel;
            if (mSpriteParams.mMtx == NULL) {
                fn_801EF830(field_0x4, &mPos, &params, 7, "enemy_gc_sob.bin", frame, NULL);
            }
        }
        params.field_0x10.r = 0xFF;
        params.field_0x10.g = 0xFF;
        params.field_0x10.b = 0xFF;
        params.field_0x10.a = 0xFF - field_0x24c;
    }
    if (field_0x240 == 0x1C || field_0x240 == 0x1D || field_0x240 == 0x1E || field_0x240 == 0x21 || field_0x240 == 0x22 || field_0x240 == 0x23) {
        frame = field_0x264 + 0x1B3;
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
        }
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x264 + 0x1B3, 0, 1, -1, false, 0, 0);
    } else {
        frame = arg + field_0x318.mCel;
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
        }
        fn_803075AC(mSerial, field_0x4, &mPos, arg + field_0x318.mCel, 0, 1, -1, false, 0, 0);
    }
}

// FIXME: 75.80 %, missing/extra + reloc-name; behaviour drift.
// the original keeps lbl_80474C38 in a register as table base (we index lbl_80474C68).
void cls_804AE088::fn_8036AA60() {
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
    fn_8010F668(rot, &axisZ, field_0x280);
    PSMTXConcat(mtx, rot, mtx);
    if (field_0x238 != 0) {
        field_0x254++;
    }
    switch (field_0x244) {
    case 0:
    case 2:
        if (field_0x254 >= lbl_80474C68[12 + field_0x244]) {
            field_0x254 = lbl_80474C68[9 + field_0x244];
        }
        break;
    case 1:
        if (field_0x254 >= lbl_80474C68[13]) {
            field_0x244 = 2;
        }
    }
    s32 frame = field_0x254;
    GameRootSub20_801902C0::Crystal& crystal = fn_801902C0()->mCrystals[lbl_80474C68[23 + field_0x23c]];
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

// Not auto-inlined: draw calls fn_8036A894 out of line in the original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// FIXME: 93.25 %, scheduling+regalloc + reloc-name; behaviour clean.
void cls_804AE088::fn_8036A894() {
    if (field_0x24c > 0) {
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
        params.field_0x14.r = 0;
        params.field_0x14.g = 0;
        params.field_0x14.b = 0;
        params.field_0x14.a = field_0x24c;
        params.field_0x1d = -1;
        params.field_0x2d = 1;
        params.mMtx = mtx;
        JGeometry::TVec3f pos = mPos;
        pos.z = 0.0f;
        if (field_0x4 < 8) {
            gGameRoot2_8053AB10->field_0x20->fn_802F745C(&pos, field_0x310, &params);
        }
    }
}
#pragma auto_inline reset

void cls_804AE088::fn_8036A65C() {
    deleteEmitter(&field_0x350);
    deleteEmitter(&field_0x354);
    deleteEmitter(&field_0x358);
    deleteEmitter(&field_0x398);
    for (s32 i = 0; i < 7; i++) {
        deleteEmitter(&field_0x35c[i]);
        if (i < 2) {
            deleteEmitter(&field_0x3b4[i]);
        }
        if (i < 3) {
            deleteEmitter(&field_0x378[i]);
        }
        if (i < 5) {
            deleteEmitter(&field_0x384[i]);
            deleteEmitter(&field_0x3ec[i]);
            deleteEmitter(&field_0x400[i]);
        }
        if (i < 6) {
            deleteEmitter(&field_0x3bc[i]);
            deleteEmitter(&field_0x3d4[i]);
            deleteEmitter(&field_0x39c[i]);
        }
    }
}

// Keeps the camera on the box (x, y0)-(x, y1) unless a scripted move is running.
static inline void setCameraBox(f32 x, f32 y0, f32 y1) {
    gGameRoot2_8053AB10->field_0x24->field_0x179 = 1;
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    if (camera->field_0x7f5 == 0) {
        camera->field_0x7f4 = 1;
        camera->field_0x7f8 = 5;
        camera->field_0x808.set(x, y0, x, y1);
        camera->field_0x7f6 = 0;
    }
}

// differs: register choice for the camera box and the message position.
void cls_804AE088::fn_80369DDC() {
    if (field_0x258 != 0) {
        field_0x258--;
    }
    if (field_0x26c != 0) {
        field_0x26c--;
        if (field_0x26c == 0 && field_0x398 != NULL) {
            field_0x398->becomeInvalidEmitter();
            field_0x398 = NULL;
        }
    }
    cls_8049D140::update();
    if (isStageFlagSet(lbl_80529DEC.field_0x118, lbl_80529DEC.field_0x11c) && field_0x240 >= 5 &&
        field_0x240 <= 13) {
        gGameRoot_8053AAF8->field_0x85 = 2;
        if (lbl_8053AD30->fn_80309C4C(0x1000, -1) != 0) {
            field_0x240 = 13;
            fn_80387494(0);
            fn_8011921C()->fn_801183BC();
            if (mNaviActor != NULL) {
                mNaviActor->fn_803676CC();
                mNaviActor->fn_801F3668();
                mNaviActor = NULL;
            }
        }
    }
    if (field_0x240 <= 5 && fn_8016A3AC()->field_0x1ef == 0 && field_0x398 == NULL) {
        field_0x398 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x54C, 0, 1, NULL, NULL, true, 1.0f);
    }
    JGeometry::TVec3f pos;
    JGeometry::TVec3f camPos;
    if (field_0x240 >= 2 && field_0x240 != 0x11) {
        if (field_0x240 >= 3 && field_0x240 < 0xE) {
            cls_804A03A8* obj = gGameRoot2_8053AB10->field_0x30;
            if (obj->field_0x284 == 0) {
                obj->field_0x284 = 1;
                obj->field_0x28c = 0.05f;
            }
        }
        pos.set(mPos);
        pos.y += 16.0f + ((mSpawnPos.y - mPos.y) - 32.0f);
        if (field_0x240 == 5) {
            pos.y += 28.0f;
        }
        if (field_0x240 >= 8 && field_0x240 < 10) {
            pos.y += 28.0f;
        }
        if (field_0x240 >= 3) {
            setCameraBox(pos.x, pos.y - 105.0f, 105.0f + pos.y);
        } else {
            gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&pos);
            Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
            if (camera->field_0x7f5 == 0) {
                camera->field_0x7f4 = 1;
                camera->field_0x7f8 = 4;
                camera->field_0x7fc.set(pos);
                camera->field_0x7f6 = 0;
            }
        }
    }
    switch (field_0x240) {
    case 0:
        gAudioMgr_8053AF80->fn_80458FF0(0x100001B, false, 0);
        vfunc_0x60(1);
        break;
    case 3:
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f playerPos = *getPlayerPos_8023E724(i);
            fn_80230498(i, &playerPos, fn_8023CBC0(i), 0, -128, -128, true);
        }
        if (fn_8011921C()->field_0x8 == 0x1B8) {
            gAudioMgr_8053AF80->fn_80458F9C(60);
        }
        break;
    case 4:
        fn_801CD950(&field_0x284, 0.02f, 0.0004f);
        fn_801CD950(&field_0x27c, 0.15f, 0.01f);
        mPos.y -= field_0x27c;
        if (mPos.y <= mSpawnPos.y - 48.0f) {
            mPos.y = mSpawnPos.y - 48.0f;
        }
        field_0x24c += 2;
        if (field_0x24c >= 0xE1) {
            field_0x24c = 0xE1;
        }
        break;
    case 5:
        field_0x24c = 0xE1;
        if (field_0x258 != 0) {
            if (field_0x258 == 1) {
                pos.set((s32)JUTVideo::getManager()->getFbWidth() * 0.5f,
                        (s32)JUTVideo::getManager()->getEfbHeight() * 0.5f, 0.0f);
                pos.y += -16.0f;
                fn_8022FE80(0, field_0x4, mSerial, true, false);
                if (fn_8038BD08(0, lbl_80474C68[field_0x268], field_0x4, pos, 0x62)) {
                    field_0x268++;
                    fn_8011921C()->fn_80118454();
                }
            }
        } else {
            fn_8022FE80(0, field_0x4, mSerial, true, false);
            if (fn_803865B8(0)) {
                fn_80388EEC(0);
                vfunc_0x60(6);
            }
        }
        break;
    case 6: {
        field_0x24c = 0xE1;
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
        camPos.x = -camera->field_0x4[0][3];
        camPos.y = -camera->field_0x4[1][3];
        camPos.z = 0.0f;
        if (PSVECDistance(&pos, &camPos) < 0.1f) {
            vfunc_0x60(7);
        }
        break;
    }
    case 7:
        field_0x24c = 0xE1;
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_803865B8(0)) {
            fn_80388EEC(0);
            vfunc_0x60(8);
        }
        break;
    case 8: {
        field_0x24c = 0xE1;
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
        camPos.x = -camera->field_0x4[0][3];
        camPos.y = -camera->field_0x4[1][3];
        camPos.z = 0.0f;
        if (PSVECDistance(&pos, &camPos) < 0.1f) {
            vfunc_0x60(9);
            mNaviActor->mState = 2;
        }
        break;
    }
    case 9:
        field_0x24c = 0xE1;
        break;
    case 10: {
        field_0x24c = 0xE1;
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
        camPos.x = -camera->field_0x4[0][3];
        camPos.y = -camera->field_0x4[1][3];
        camPos.z = 0.0f;
        if (PSVECDistance(&pos, &camPos) >= 0.1f) {
            break;
        }
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_803865B8(0)) {
            fn_80388EEC(0);
            vfunc_0x60(0xB);
            mNaviActor->mState = 6;
        }
        break;
    }
    case 11:
        field_0x24c = 0xE1;
        break;
    case 12:
        field_0x24c = 0xE1;
        if (field_0x258 == 0) {
            vfunc_0x60(0xD);
        }
        break;
    case 13:
        field_0x24c = 0xE1;
        if (fn_8011921C()->field_0x1 == 0) {
            fn_801CD950(&field_0x284, 0.0f, -0.0002f);
        }
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_803865B8(0)) {
            fn_80388EEC(0);
            fn_8011921C()->fn_801183BC();
        }
        break;
    case 14:
    case 15:
    case 16:
    case 17:
        break;
    }
    if (field_0x237 != 0) {
        field_0x280 += field_0x284;
    }
    field_0x318.fn_801D1C18();
    if (field_0x234 != 0) {
        fn_8036AA60();
    }
}

static const u16 lbl_80474D6C[5] = {0x335, 0x32F, 0x331, 0x337, 0x32D};
static const u16 lbl_80474D78[5] = {0x336, 0x330, 0x332, 0x338, 0x32E};
static const u8 lbl_80474D84[5][3] = {
    {0xFF, 0x41, 0x37}, {0x1E, 0xA0, 0xBE}, {0x4B, 0xFF, 0x2D}, {0xC8, 0xA0, 0x28}, {0x2D, 0x78, 0xFF},
};
static const s32 lbl_80474D94[6] = {0x96, 0x64, 0x46, 0x78, 0x55, 0x3C};
static const s32 lbl_80474DAC[6] = {0x55, 0x78, 0x46, 0x64, 0x96, 0x3C};
static const u16 lbl_80474E1C[6] = {0x28, 0x22, 0x24, 0x2A, 0x20, 0x26};
static const u16 lbl_80474E28[6] = {0x29, 0x23, 0x25, 0x2B, 0x21, 0x27};
static const u8 lbl_80474E34[6][3] = {
    {0xFF, 0x41, 0x37}, {0x1E, 0xA0, 0xBE}, {0x4B, 0xFF, 0x2D},
    {0xC8, 0xA0, 0x28}, {0x2D, 0x78, 0xFF}, {0xDC, 0x1E, 0xDC},
};

// differs: the original reads every table at an offset from one base (lbl_80474C38).
void cls_804AE088::fn_80368CD8() {
    if (field_0x258 != 0) {
        field_0x258--;
    }
    if (field_0x26c != 0) {
        field_0x26c--;
        if (field_0x26c == 0 && field_0x398 != NULL) {
            field_0x398->becomeInvalidEmitter();
            field_0x398 = NULL;
        }
    }
    if (field_0x274 != 0) {
        field_0x274--;
    }
    if (field_0x28c >= 0) {
        field_0x28c++;
        if (field_0x28c >= 0x96 && field_0x288 < 255.0f) {
            field_0x288 += 1.0f;
            if (field_0x288 >= 255.0f) {
                field_0x288 = 255.0f;
            }
            for (s32 i = 0; i < 6; i++) {
                if (field_0x39c[i] != NULL) {
                    field_0x39c[i]->setGlobalAlpha(field_0x288);
                }
            }
        }
    }
    cls_8049D140::update();
    if (isStageFlagSet(lbl_80529DEC.field_0x118, lbl_80529DEC.field_0x11c) && field_0x270 == 0 &&
        field_0x240 >= 0x18 && field_0x240 <= 0x23) {
        gGameRoot_8053AAF8->field_0x85 = 2;
        if (lbl_8053AD30->fn_80309C4C(0x1000, -1) != 0) {
            field_0x270 = 1;
            field_0x290 = 0.0f;
            fn_80387494(0);
            fn_80119DD0()->fn_80119958();
            field_0x274 = 0x100;
        }
    }
    if (field_0x240 <= 0x18 && fn_8016A3AC()->field_0x1ef == 0 && field_0x398 == NULL) {
        field_0x398 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x54C, 0, 1, NULL, NULL, true, 1.0f);
    }
    JGeometry::TVec3f pos;
    if (field_0x240 >= 0x15 && field_0x240 != 0x26) {
        if ((field_0x240 >= 0x16 && field_0x240 < 0x1A) || (field_0x240 >= 0x1C && field_0x240 < 0x24)) {
            if (field_0x270 == 0 || field_0x274 >= 0x80) {
                cls_804A03A8* obj = gGameRoot2_8053AB10->field_0x30;
                if (obj->field_0x284 == 0) {
                    obj->field_0x284 = 1;
                    obj->field_0x28c = 0.05f;
                }
            }
        }
        pos.set(mPos);
        pos.y += 16.0f + ((mSpawnPos.y - mPos.y) - 32.0f);
        if (field_0x240 >= 0x1C) {
            if (field_0x240 <= 0x20) {
                pos.y += 16.0f;
            } else {
                pos.y += 32.0f;
            }
        }
        if (field_0x240 >= 0x16) {
            setCameraBox(pos.x, pos.y - 105.0f, 105.0f + pos.y);
        } else {
            gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&pos);
            Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
            if (camera->field_0x7f5 == 0) {
                camera->field_0x7f4 = 1;
                camera->field_0x7f8 = 4;
                camera->field_0x7fc.set(pos);
                camera->field_0x7f6 = 0;
            }
        }
    }
    if (field_0x270 != 1) {
        switch (field_0x240) {
        case 0x13:
            gAudioMgr_8053AF80->fn_80458FF0(0x100001B, false, 0);
            vfunc_0x60(0x14);
            break;
        case 0x16:
            for (s32 i = 0; i < 4; i++) {
                JGeometry::TVec3f playerPos = *getPlayerPos_8023E724(i);
                fn_80230498(i, &playerPos, fn_8023CBC0(i), 0, -128, -128, true);
            }
            if (fn_8011921C()->field_0x8 == 0x1B8) {
                gAudioMgr_8053AF80->fn_80458F9C(60);
            }
            break;
        case 0x17:
            fn_801CD950(&field_0x284, 0.02f, 0.0004f);
            fn_801CD950(&field_0x27c, 0.15f, 0.01f);
            mPos.y -= field_0x27c;
            if (mPos.y <= mSpawnPos.y - 48.0f) {
                mPos.y = mSpawnPos.y - 48.0f;
            }
            field_0x24c += 2;
            if (field_0x24c >= 0xE1) {
                field_0x24c = 0xE1;
            }
            break;
        case 0x18:
            field_0x24c = 0xE1;
            if (fn_8011921C()->field_0x1 == 0) {
                fn_801CD950(&field_0x284, 0.0f, -0.0002f);
            }
            fn_8022FE80(0, field_0x4, mSerial, true, false);
            if (fn_803865B8(0)) {
                fn_80388EEC(0);
                vfunc_0x60(0x19);
                field_0x25c = 0;
                field_0x260 = 0;
                field_0x264 = 0;
            }
            break;
        case 0x19:
            for (s32 i = 0; i < 5; i++) {
                if (field_0x294[i] != 0) {
                    field_0x294[i]--;
                    if (field_0x294[i] == 0) {
                        const JGeometry::TVec3f& p = field_0x2a8[i] + JGeometry::TVec3f(0.0f, 0.0f, 20.0f);
                        field_0x3bc[i] = spawnEmitter_8013CC50(field_0x4, &p, 0x32C, 0, -1, NULL, NULL, true, 1.0f);
                        const JGeometry::TVec3f& q = field_0x2a8[i] + JGeometry::TVec3f(0.0f, 0.0f, 20.0f);
                        field_0x3d4[i] = spawnEmitter_8013CC50(field_0x4, &q, 0x184, 0, -1, NULL, NULL, true, 1.0f);
                        if (field_0x3bc[i] != NULL) {
                            field_0x3bc[i]->setGlobalEnvColor(lbl_80474D84[i][0], lbl_80474D84[i][1],
                                                              lbl_80474D84[i][2]);
                        }
                    }
                }
                if (field_0x294[i] == 0x28) {
                    const JGeometry::TVec3f& p = field_0x2a8[i] + JGeometry::TVec3f(0.0f, 0.0f, 20.0f);
                    field_0x3ec[i] = spawnEmitter_8013CC50(field_0x4, &p, lbl_80474D6C[i], 1, 0, NULL, NULL, true, 1.0f);
                    const JGeometry::TVec3f& q = field_0x2a8[i] + JGeometry::TVec3f(0.0f, 0.0f, 20.0f);
                    field_0x400[i] = spawnEmitter_8013CC50(field_0x4, &q, lbl_80474D78[i], 1, 0, NULL, NULL, true, 1.0f);
                    startSound_801F0E34(0x400D9, 0);
                }
            }
            if ((field_0x294[0] | field_0x294[1] | field_0x294[2] | field_0x294[3] | field_0x294[4]) == 0) {
                fn_8011921C()->fn_801183BC();
            }
            break;
        case 0x1C:
            if (field_0x258 == 1) {
                fn_8011921C()->fn_80118454();
            } else if (field_0x258 == 0) {
                vfunc_0x60(0x1D);
            }
            break;
        case 0x1D:
            if (field_0x258 == 50) {
                pos.set((s32)JUTVideo::getManager()->getFbWidth() * 0.5f,
                        (s32)JUTVideo::getManager()->getEfbHeight() * 0.5f, 0.0f);
                pos.y += -16.0f;
                fn_8022FE80(0, field_0x4, mSerial, true, false);
                fn_8038BD08(0, 0x848, field_0x4, pos, 0x62);
            } else if (field_0x258 == 49) {
                fn_8022FE80(0, field_0x4, mSerial, true, false);
                if (!fn_803865B8(0)) {
                    field_0x258++;
                } else {
                    fn_80388EEC(0);
                }
            } else if (field_0x258 == 15) {
            } else if (field_0x258 == 0) {
                vfunc_0x60(0x1E);
            }
            break;
        case 0x1E:
            if (field_0x250 < 315.0f) {
                field_0x250 += 1.0f;
            } else {
                vfunc_0x60(0x1F);
            }
            field_0x24c = field_0x250;
            if (field_0x24c >= 0x100) {
                field_0x24c = 0xFF;
            }
            field_0x2f4->fn_802E2990(0xFF - field_0x24c);
            break;
        case 0x1F:
            if (field_0x258 != 0) {
                if (field_0x258 == 1) {
                    pos.set((s32)JUTVideo::getManager()->getFbWidth() * 0.5f,
                            (s32)JUTVideo::getManager()->getEfbHeight() * 0.5f, 0.0f);
                    pos.y += -16.0f;
                    fn_8022FE80(0, field_0x4, mSerial, true, false);
                    fn_8038BD08(0, lbl_80474C68[field_0x268], field_0x4, pos, 0x62);
                    field_0x268++;
                }
            } else {
                fn_8022FE80(0, field_0x4, mSerial, true, false);
                if (fn_803865B8(0)) {
                    fn_80388EEC(0);
                    vfunc_0x60(0x20);
                }
            }
            break;
        case 0x20:
            if (field_0x258 == 0) {
                vfunc_0x60(0x21);
            }
            break;
        case 0x21:
            if (fn_80119DD0()->field_0x0 == 0) {
                vfunc_0x60(0x22);
            }
            break;
        case 0x22:
            fn_8022FE80(0, field_0x4, mSerial, true, false);
            if (fn_803865B8(0)) {
                fn_80388EEC(0);
                vfunc_0x60(0x23);
            }
            break;
        case 0x23:
            if (field_0x258 != 0) {
                if (field_0x258 == 1) {
                    pos.set((s32)JUTVideo::getManager()->getFbWidth() * 0.5f,
                            (s32)JUTVideo::getManager()->getEfbHeight() * 0.5f, 0.0f);
                    pos.y += -16.0f;
                    fn_8022FE80(0, field_0x4, mSerial, true, false);
                    fn_8038BD08(0, lbl_80474C68[field_0x268], field_0x4, pos, 0x62);
                    field_0x268++;
                }
            } else {
                fn_8022FE80(0, field_0x4, mSerial, true, false);
                if (fn_803865B8(0)) {
                    fn_80388EEC(0);
                    vfunc_0x60(0x24);
                }
            }
            break;
        case 0x24:
            for (s32 i = 0; i < 6; i++) {
                if (field_0x258 == lbl_80474D94[i]) {
                    field_0x2f8[i] = 1;
                    JGeometry::TVec3f p = field_0x2a8[i];
                    p.z += 6.0f;
                    spawnEmitter_8013CC50(field_0x4, &p, lbl_80474E1C[i], 0, 0, NULL, NULL, true, 1.0f);
                    p.z += 7.0f;
                    spawnEmitter_8013CC50(field_0x4, &p, lbl_80474E28[i], 0, 0, NULL, NULL, true, 1.0f);
                    startSound_801F0E34(0x400D9, 0);
                    field_0x2a8[i].z = 16.0f;
                } else if (field_0x258 == lbl_80474D94[i] - 0x28) {
                    field_0x2f8[i] = 2;
                    field_0x3bc[i] = spawnEmitter_8013CC50(field_0x4, &field_0x2a8[i], 0x32C, 0, -1, NULL, NULL, true, 1.0f);
                    field_0x3d4[i] = spawnEmitter_8013CC50(field_0x4, &field_0x2a8[i], 0x184, 0, -1, NULL, NULL, true, 1.0f);
                    if (field_0x3bc[i] != NULL) {
                        field_0x3bc[i]->setGlobalEnvColor(lbl_80474E34[i][0], lbl_80474E34[i][1],
                                                          lbl_80474E34[i][2]);
                    }
                }
            }
            if (field_0x258 == 0) {
                vfunc_0x60(0x25);
            }
            break;
        case 0x25: {
            s32 count = 0;
            for (s32 i = 0; i < 6; i++) {
                if (field_0x258 == lbl_80474DAC[i]) {
                    field_0x2f8[i] = 3;
                }
                if (field_0x2f8[i] == 3) {
                    field_0x2a8[i].z += 1.5f;
                }
                if (field_0x3bc[i] != NULL) {
                    setEmitterPos_8013CB44(field_0x3bc[i], &field_0x2a8[i], 0, -1, true);
                }
                if (field_0x3d4[i] != NULL) {
                    setEmitterPos_8013CB44(field_0x3d4[i], &field_0x2a8[i], 0, -1, true);
                }
                if (field_0x2f8[i] != 4 && field_0x2a8[i].z >= 255.0f) {
                    field_0x2f8[i] = 4;
                    deleteEmitter(&field_0x3bc[i]);
                }
                if (field_0x2f8[i] == 4) {
                    count++;
                }
            }
            if (count >= 6) {
                fn_8011921C()->fn_801183BC();
            }
            break;
        }
        case 0x26:
            fn_80119DD0()->fn_80119998(0x78, 0, 0, 0, 1, 0, 0);
            break;
        }
    }
    if (field_0x270 == 1) {
        fn_80368198();
    }
    if (field_0x237 != 0) {
        field_0x280 += field_0x284;
    }
    field_0x318.fn_801D1C18();
    if (field_0x234 != 0) {
        fn_8036AA60();
    }
}

// FIXME: 84.13 %, missing/extra + reloc-name; behaviour drift.
// FIXME: original keeps lbl_80474C38 in a register as table base (we index lbl_80474C68).
void cls_804AE088::fn_80368660() {
    SpriteParams_801F5AE0 params;
    if (field_0x23c == 5 && (field_0x270 != 1 || field_0x274 >= 0x80) && field_0x25c >= 0) {
        if (field_0x260 != 0) {
            field_0x260--;
        }
        s32 state = field_0x240;
        if (state < 0x1c) {
            if (field_0x260 == 0) {
                field_0x260 = 1;
                field_0x264++;
                if (field_0x264 >= 6) {
                    field_0x264 = 0;
                }
            }
        } else if (state == 0x1c || state == 0x1d || state == 0x1e) {
            if (state == 0x1c) {
                field_0x264 = 8;
            } else if (state == 0x1d) {
                field_0x264 = 8;
                field_0x260 = 30;
            } else if (state == 0x1e) {
                if (field_0x260 == 0) {
                    if (field_0x264 == 8) {
                        field_0x264 = 10;
                        field_0x260 = 30;
                    } else if (field_0x264 == 10) {
                        field_0x264 = 11;
                        field_0x260 = 10;
                    } else {
                        field_0x260 = 9;
                        field_0x264++;
                        if (field_0x264 >= 6) {
                            field_0x264 = 0;
                        }
                    }
                }
            } else {
                if (field_0x260 == 0) {
                    field_0x260 = 9;
                    field_0x264++;
                    if (field_0x264 >= 6) {
                        field_0x264 = 0;
                    }
                }
            }
        } else if (state == 0x22 || state == 0x21) {
            if (field_0x260 == 0) {
                field_0x260 = 9;
                field_0x264++;
                if (field_0x264 >= 6) {
                    field_0x264 = 0;
                }
            }
        } else if (state == 0x23) {
            if (field_0x258 >= 0x50) {
                if (field_0x260 == 0) {
                    field_0x260 = 9;
                    field_0x264++;
                    if (field_0x264 >= 6) {
                        field_0x264 = 0;
                    }
                }
            } else if (field_0x258 >= 0x46) {
                field_0x264 = 11;
            } else if (field_0x258 >= 0x28) {
                field_0x264 = 10;
            } else {
                field_0x264 = 8;
            }
        }
        JGeometry::TVec3f savedPos = mPos;
        for (s32 i = 0; i < 5; i++) {
            if (field_0x240 < 0x1c) {
                if (field_0x294[i] == 0) {
                    mPos = field_0x2a8[i];
                    mPos.z = 20.0f;
                    drawSprite_801F06F0("enemy_gc_sob.bin", field_0x264 + lbl_80474C68[92 + i], &params, 7, 1.0f, 1.0f);
                    if (field_0x3bc[i] != NULL) {
                        JGeometry::TVec3f p = field_0x2a8[i] + JGeometry::TVec3f(0.0f, 0.0f, 20.0f);
                        setEmitterPos_8013CB44(field_0x3bc[i], &p, 0, -1, true);
                    }
                    if (field_0x3d4[i] != NULL) {
                        JGeometry::TVec3f p = field_0x2a8[i] + JGeometry::TVec3f(0.0f, 0.0f, 20.0f);
                        setEmitterPos_8013CB44(field_0x3d4[i], &p, 0, -1, true);
                    }
                }
            } else if (field_0x240 < 0x21) {
                mPos = field_0x2a8[i];
                if (field_0x240 >= 0x1e && field_0x240 < 0x21) {
                    if (field_0x24c < 0xc0 && mSpriteParams.mMtx == NULL) {
                        fn_801EF830(field_0x4, &mPos, &params, 7, "enemy_both2_sob.bin", field_0x264 + lbl_80474C68[87 + i], NULL);
                    }
                    params.field_0x10.r = 255;
                    params.field_0x10.g = 255;
                    params.field_0x10.b = 255;
                    params.field_0x10.a = 255 - field_0x24c;
                    if (field_0x4 < 8) {
                        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
                        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, field_0x264 + lbl_80474C68[87 + i], &params, 1.0f, 1.0f);
                    }
                    fn_803075AC(mSerial, field_0x4, &mPos, field_0x264 + lbl_80474C68[87 + i], 0, 1, -1, false, 0, 0);
                } else {
                    drawSprite_801F06F0("enemy_both2_sob.bin", field_0x264 + lbl_80474C68[87 + i], &params, 7, 1.0f, 1.0f);
                }
            } else {
                mPos.x = field_0x2a8[i].x;
                mPos.y = field_0x2a8[i].y;
                mPos.z = field_0x2a8[i].z;
                drawSprite_801F06F0("enemy_both2_sob.bin", field_0x264 + lbl_80474C68[87 + i], &params, 7, 1.0f, 1.0f);
            }
        }
        mPos = savedPos;
    }
}

// FIXME: 76.65 %, different + reloc-name; behaviour drift.
void cls_804AE088::fn_803683AC() {
    SpriteParams_801F5AE0 params;
    JGeometry::TVec3f savedPos = mPos;
    if (field_0x260 != 0) {
        field_0x260--;
    }
    if (field_0x260 == 0) {
        field_0x264++;
        if (field_0x264 >= 6) {
            field_0x264 = 0;
        }
        field_0x260 = 1;
    }
    for (s32 i = 0; i < 6; i++) {
        if (field_0x2f8[i] == 0) {
            mPos = field_0x2a8[i];
            params.field_0x10.a = 255;
            drawSprite_801F06F0("enemy_gc_sob.bin", lbl_80474C68[97 + i] + 8, &params, 7, 1.0f, 1.0f);
        } else if (field_0x2f8[i] != 1 && field_0x2f8[i] != 4) {
            mPos = field_0x2a8[i];
            params.field_0x10.a = 255;
            drawSprite_801F06F0("enemy_both2_sob.bin", field_0x264 + lbl_80474C68[103 + i], &params, -1, 1.0f, 1.0f);
            if (field_0x2a8[i].z < 255.0f) {
                params.field_0x10.a = 255 - (s32)field_0x2a8[i].z;
            } else {
                params.field_0x10.a = 0;
            }
            if (mSpriteParams.mMtx == NULL) {
                fn_801EF830(field_0x4, &mPos, &params, 0x407, "enemy_both2_sob.bin", field_0x264 + lbl_80474C68[103 + i], NULL);
            }
        }
    }
    mPos = savedPos;
}

// FIXME: 97.74 %, missing/extra + reloc-name; behaviour drift.
void cls_804AE088::fn_80368198() {
    if (field_0x270 != 0) {
        if (field_0x274 < 0x80) {
            field_0x290 -= 8.0f;
        } else {
            field_0x290 += 8.0f;
        }
        if (field_0x290 >= 255.0f) {
            field_0x290 = 255.0f;
        } else if (field_0x290 < 0.0f) {
            field_0x290 = 0.0f;
        }
        if (field_0x314 == NULL) {
            const ResTIMG* tex = (const ResTIMG*)gGameRoot_8053AAF8->field_0xa4->getResource('?\?\?\?', lbl_8053A1C8);
            field_0x314 = new J2DPicture(tex);
        }
        gGameRoot2_8053AB10->fn_8013B490();
        field_0x314->setBlackWhite(JUtility::TColor(0), JUtility::TColor(0xFFFFFFFF));
        field_0x314->setAlpha((s32)field_0x290);
        field_0x314->draw(0.0f, 0.0f, (s32)JUTVideo::getManager()->getRenderMode()->fbWidth,
                          (s32)JUTVideo::getManager()->getRenderMode()->efbHeight, false, false, false);
        if (field_0x274 == 0x80) {
            field_0x234 = 0;
            field_0x236 = 1;
            field_0x24c = 1;
            fn_8036A65C();
        } else if (field_0x274 == 0) {
            field_0x270 = 0;
            vfunc_0x60(0x26);
            fn_8011921C()->fn_801183BC();
        }
    }
}
