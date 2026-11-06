#include "game/GameAudio.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/BgRewrite.h"
#include "game/DrawShadow.h"
#include "game/Effect.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_red_blue_switch.h"

void RedBlueSwitch_804A9020::init() {
    mEmitter230 = NULL;
    mEmitter234 = NULL;
    mEmitter238 = NULL;
    mEmitter23c = NULL;
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    if (!getGameView_802D6A9C()->field_0x171) {
        gBgRewrite_8053AC98->fn_802A3948(&mPos, 0, field_0x4, 0xAF);
        field_0x240 = 2;
        JGeometry::TVec3f pos1(mPos.x + 8.0f, mPos.y + 8.0f, mPos.z);
        mEmitter230 = spawnEmitter_8013CC50(field_0x4, &pos1, 0x426, 1, 0, NULL, NULL, true, 1.0f);
        JGeometry::TVec3f pos2(mPos.x + 8.0f, mPos.y + 8.0f, mPos.z);
        mEmitter234 = spawnEmitter_8013CC50(field_0x4, &pos2, 0x427, 1, 0, NULL, NULL, true, 1.0f);
    } else {
        field_0x240 = 0;
        JGeometry::TVec3f pos3(mPos.x + 8.0f, mPos.y + 8.0f, mPos.z);
        mEmitter238 = spawnEmitter_8013CC50(field_0x4, &pos3, 0x42D, 1, 0, NULL, NULL, true, 1.0f);
        JGeometry::TVec3f pos4(mPos.x + 8.0f, mPos.y + 8.0f, mPos.z);
        mEmitter23c = spawnEmitter_8013CC50(field_0x4, &pos4, 0x42E, 1, 0, NULL, NULL, true, 1.0f);
    }
    field_0x244 = 15;
}

RedBlueSwitch_804A9020::~RedBlueSwitch_804A9020() {
    if (mEmitter230 != NULL) {
        releaseEmitter_801EE434(mEmitter230);
    }
    if (mEmitter234 != NULL) {
        releaseEmitter_801EE434(mEmitter234);
    }
    if (mEmitter238 != NULL) {
        releaseEmitter_801EE434(mEmitter238);
    }
    if (mEmitter23c != NULL) {
        releaseEmitter_801EE434(mEmitter23c);
    }
}

void RedBlueSwitch_804A9020::update() {
    JGeometry::TVec2f center(mPos.x + 8.0f, mPos.y + 8.0f);
    JGeometry::TBox2f box(mBox);
    box.addPos(center);
    s32 player = fn_8023BA68(&box, field_0x4, &center, true);
    field_0xd8 = 0;
    switch (field_0x240) {
    case 0:
        if (player >= 0 || fn_801F2DB4('HBMB', 'NULL', NULL)) {
            gBgRewrite_8053AC98->fn_802A3948(&mPos, player, field_0x4, 0xAF);
            gGameRoot2_8053AB10->field_0x20->fn_802FA160(
                gGameRoot_8053AAF8->field_0xa4->getResource('SCH ', "bg_tenso_sch.bin"), 0x2C0, 0, 0x5C0, 0x40);
            gGameRoot2_8053AB10->field_0x20->fn_802FA160(
                gGameRoot_8053AAF8->field_0xa4->getResource('SCH ', "bg_tenso_sch.bin"), 0x300, 0, 0x7C0, 0x40);
            field_0x240 = 1;
            startSound_801F0E34(0x30051, 0);
            field_0x244 = 60;
            getGameView_802D6A9C()->fn_802D790C(0);
            JGeometry::TVec3f p1(mPos.x + 8.0f, mPos.y + 8.0f, mPos.z);
            spawnEmitter_8013CC50(field_0x4, &p1, 0x42A, 1, 0, NULL, NULL, true, 1.0f);
            JGeometry::TVec3f p2(mPos.x + 8.0f, mPos.y + 8.0f, mPos.z);
            mEmitter230 = spawnEmitter_8013CC50(field_0x4, &p2, 0x426, 1, 0, NULL, NULL, true, 1.0f);
            JGeometry::TVec3f p3(mPos.x + 8.0f, mPos.y + 8.0f, mPos.z);
            mEmitter234 = spawnEmitter_8013CC50(field_0x4, &p3, 0x427, 1, 0, NULL, NULL, true, 1.0f);
            if (mEmitter238 != NULL) {
                mEmitter238->becomeInvalidEmitter();
                mEmitter238 = NULL;
            }
            if (mEmitter23c != NULL) {
                mEmitter23c->becomeInvalidEmitter();
                mEmitter23c = NULL;
            }
        } else if (!getGameView_802D6A9C()->field_0x171) {
            gBgRewrite_8053AC98->fn_802A3948(&mPos, player, field_0x4, 0xAF);
            JGeometry::TVec3f p4(mPos.x + 8.0f, mPos.y + 8.0f, mPos.z);
            mEmitter230 = spawnEmitter_8013CC50(field_0x4, &p4, 0x426, 1, 0, NULL, NULL, true, 1.0f);
            JGeometry::TVec3f p5(mPos.x + 8.0f, mPos.y + 8.0f, mPos.z);
            mEmitter234 = spawnEmitter_8013CC50(field_0x4, &p5, 0x427, 1, 0, NULL, NULL, true, 1.0f);
            if (mEmitter238 != NULL) {
                mEmitter238->becomeInvalidEmitter();
                mEmitter238 = NULL;
            }
            if (mEmitter23c != NULL) {
                mEmitter23c->becomeInvalidEmitter();
                mEmitter23c = NULL;
            }
            field_0x240 = 2;
        }
        break;

    case 1:
        if (field_0x244 == 0) {
            field_0x240 = 2;
        } else {
            field_0x244--;
        }
        break;

    case 2:
        if (player >= 0 || fn_801F2DB4('HBMB', 'NULL', NULL)) {
            gBgRewrite_8053AC98->fn_802A3884(&mPos, player, field_0x4, 0xAE);
            gGameRoot2_8053AB10->field_0x20->fn_802FA160(
                gGameRoot_8053AAF8->field_0xa4->getResource('SCH ', "bg_tenso_sch.bin"), 0x240, 0, 0x5C0, 0x40);
            gGameRoot2_8053AB10->field_0x20->fn_802FA160(
                gGameRoot_8053AAF8->field_0xa4->getResource('SCH ', "bg_tenso_sch.bin"), 0x280, 0, 0x7C0, 0x40);
            field_0x240 = 3;
            startSound_801F0E34(0x30051, 0);
            getGameView_802D6A9C()->fn_802D790C(1);
            JGeometry::TVec3f p6(mPos.x + 8.0f, mPos.y + 8.0f, mPos.z);
            spawnEmitter_8013CC50(field_0x4, &p6, 0x42C, 1, 0, NULL, NULL, true, 1.0f);
            JGeometry::TVec3f p7(mPos.x + 8.0f, mPos.y + 8.0f, mPos.z);
            mEmitter238 = spawnEmitter_8013CC50(field_0x4, &p7, 0x42D, 1, 0, NULL, NULL, true, 1.0f);
            JGeometry::TVec3f p8(mPos.x + 8.0f, mPos.y + 8.0f, mPos.z);
            mEmitter23c = spawnEmitter_8013CC50(field_0x4, &p8, 0x42E, 1, 0, NULL, NULL, true, 1.0f);
            if (mEmitter230 != NULL) {
                mEmitter230->becomeInvalidEmitter();
                mEmitter230 = NULL;
            }
            if (mEmitter234 != NULL) {
                mEmitter234->becomeInvalidEmitter();
                mEmitter234 = NULL;
            }
            field_0x244 = 60;
        } else if (getGameView_802D6A9C()->field_0x171 == 1) {
            gBgRewrite_8053AC98->fn_802A3884(&mPos, player, field_0x4, 0xAE);
            JGeometry::TVec3f p9(mPos.x + 8.0f, mPos.y + 8.0f, mPos.z);
            mEmitter238 = spawnEmitter_8013CC50(field_0x4, &p9, 0x42D, 1, 0, NULL, NULL, true, 1.0f);
            JGeometry::TVec3f p10(mPos.x + 8.0f, mPos.y + 8.0f, mPos.z);
            mEmitter23c = spawnEmitter_8013CC50(field_0x4, &p10, 0x42E, 1, 0, NULL, NULL, true, 1.0f);
            if (mEmitter230 != NULL) {
                mEmitter230->becomeInvalidEmitter();
                mEmitter230 = NULL;
            }
            if (mEmitter234 != NULL) {
                mEmitter234->becomeInvalidEmitter();
                mEmitter234 = NULL;
            }
            field_0x240 = 0;
        }
        break;

    case 3:
        if (field_0x244 == 0) {
            field_0x240 = 0;
        } else {
            field_0x244--;
        }
        break;

    default:
        mAlive = false;
        break;
    }
}


void RedBlueSwitch_804A9020::draw() {
    drawShadow(field_0x4, &mPos, JUtility::TColor(0, 0, 0, 0x80), 70.0f);
}

