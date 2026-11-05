#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/act/act_shutter.h"

Shutter_804A8118::~Shutter_804A8118() {}

void Shutter_804A8118::draw() {}

void Shutter_804A8118::update() {
    switch (mState) {
    case 0:
        if (vfunc_0x34() == true) {
            lbl_8053AC98->fn_802A3948(&mPos, 0, field_0x4, 0x38);
            s32 layer = field_0x4 + 1;
            JGeometry::TVec3f p(mPos.x, mPos.y - 16.0f, 0.0f);
            lbl_8053AC98->fn_802A3948(&p, 0, layer, 0x39);
            startSound_801F0E34(0x30016, 0);
            if (field_0x4 < 8) {
                mState = 2;
                mTimer = 2;
            } else {
                lbl_8053AB10->field_0x20->fn_802FA160(
                    lbl_8053AAF8->field_0xa4->getResource('SCH ', "bg_tenso_sch.bin"), 0, 0, 0x40, 0xC0);
                mState = 1;
            }
        }
        break;
    case 1:
        if (getGameView_802D6A9C()->fn_802D800C(mSwitch)) {
            fn_801F0D20(3);
            mState = 4;
        }
        break;
    case 2:
        mTimer--;
        if (mTimer == 0) {
            lbl_8053AB10->field_0x20->fn_802FA160(
                lbl_8053AAF8->field_0xa4->getResource('SCH ', "bg_tenso_sch.bin"), 0xC0, 0, 0x40, 0xC0);
            startSound_801F0E34(0x30016, 0);
            mState = 3;
            mTimer = 2;
        }
        break;
    case 3:
        mTimer--;
        if (mTimer == 0) {
            lbl_8053AB10->field_0x20->fn_802FA160(
                lbl_8053AAF8->field_0xa4->getResource('SCH ', "bg_tenso_sch.bin"), 0, 0, 0x40, 0xC0);
            mState = 1;
            mTimer = 2;
        }
        break;
    case 4:
        mTimer--;
        if (mTimer == 0) {
            if (field_0x4 < 8) {
                lbl_8053AB10->field_0x20->fn_802FA160(
                    lbl_8053AAF8->field_0xa4->getResource('SCH ', "bg_tenso_sch.bin"), 0xC0, 0, 0x40, 0xC0);
                mState = 5;
                mTimer = 2;
            } else {
                lbl_8053AC98->fn_802A3884(&mPos, 0, field_0x4, mTile);
                s32 layer = field_0x4 + 1;
                JGeometry::TVec3f p(mPos.x, mPos.y - 16.0f, 0.0f);
                lbl_8053AC98->fn_802A3884(&p, 0, layer, 0);
                lbl_8053AB10->field_0x20->fn_802FA160(
                    lbl_8053AAF8->field_0xa4->getResource('SCH ', "bg_tenso_sch.bin"), 0x180, 0, 0x40, 0xC0);
                if (mFlag == 0) {
                    fn_801F35AC();
                } else {
                    mAlive = false;
                }
            }
            startSound_801F0E34(0x30015, 0);
        }
        break;
    case 5:
        mTimer--;
        if (mTimer == 0) {
            lbl_8053AC98->fn_802A3884(&mPos, 0, field_0x4, mTile);
            s32 layer = field_0x4 + 1;
            JGeometry::TVec3f p(mPos.x, mPos.y - 16.0f, 0.0f);
            lbl_8053AC98->fn_802A3884(&p, 0, layer, 0);
            lbl_8053AB10->field_0x20->fn_802FA160(
                lbl_8053AAF8->field_0xa4->getResource('SCH ', "bg_tenso_sch.bin"), 0x180, 0, 0x40, 0xC0);
            if (mFlag == 0) {
                fn_801F35AC();
            } else {
                mAlive = false;
            }
        }
        break;
    }
}

void Shutter_804A8118::init() {
    mSwitch = (field_0x90 >> 22) & 0x1F;
    mFlag = (field_0x90 >> 17) & 0x1F;
    mState = 0;
    mTile = lbl_8053AB10->field_0x20->fn_802F9908(field_0x4, fn_80226AE0(&mPos, field_0x4));
    if (getGameView_802D6A9C()->fn_802D800C(mSwitch)) {
        lbl_8053AC98->fn_802A3884(&mPos, 0, field_0x4, mTile);
        s32 layer = field_0x4 + 1;
        JGeometry::TVec3f p(mPos.x, mPos.y - 16.0f, 0.0f);
        lbl_8053AC98->fn_802A3884(&p, 0, layer, 0);
        lbl_8053AB10->field_0x20->fn_802FA160(
            lbl_8053AAF8->field_0xa4->getResource('SCH ', "bg_tenso_sch.bin"), 0x180, 0, 0x40, 0xC0);
        mAlive = false;
    } else {
        lbl_8053AB10->field_0x20->fn_802FA160(
            lbl_8053AAF8->field_0xa4->getResource('SCH ', "bg_tenso_sch.bin"), 0, 0, 0x40, 0xC0);
    }
}
