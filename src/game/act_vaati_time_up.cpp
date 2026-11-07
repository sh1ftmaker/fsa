#include "game/GameAudio.h"
#include "game/GameView.h"
#include "game/tu_80182914.h"
#include "game/tu_802EF76C.h"
#include "game/GameGlobals.h"
#include "game/Camera.h"
#include "game/ActorManager.h"
#include "game/Player.h"
#include "game/cls_804AE9B0.h"
#include "game/act/act_vaati_time_up.h"

// FIXME: 99.2 %. mPos.y's sum takes f4/f5 where the original uses f5/f6, the
// u32-to-float conversions differ in one li, the epilogue reloads r0 and r31 in
// the other order, and the unit's .sdata2 literals are not split (@NNN).
void VaatiTimeUp_804ACCC0::init() {
    cls_801EBA14 rec;

    mViewBox = getGameView_802D6A9C()->mVisibleBox;
    mPos.x = 64.0f + mViewBox.f.x;
    mPos.y = (mViewBox.i.y + mViewBox.f.y) * 0.5f + 96.0f;
    mPos.z = 100.0f;
    rec.field_0x0 = 1;
    rec.field_0x4 = 1;
    rec.field_0x8 = 100.0f;
    rec.field_0xc = 90.0f;
    rec.field_0x10 = 20.0f;
    field_0x230.field_0xc = rec;
    field_0x230.fn_801EB710(0, &mPos);

    field_0x2c8 = 0;
    field_0x2bc = 0;
    field_0x2c0 = 0;
    field_0x2f4 = 0.0f;
    field_0x290 = mViewBox.f.x + (mViewBox.i.x - mViewBox.f.x) * 0.5f;
    field_0x294 = mViewBox.f.y + (mViewBox.i.y - mViewBox.f.y) * 0.5f;
    field_0x294 += 96.0f;
    field_0x2a4 = 0.0f;
    field_0x2a0 = 0.0f;
    field_0x298 = mViewBox.f.x - mViewBox.i.x - 96.0f;
    field_0x29c = mViewBox.f.y - mViewBox.i.y - 96.0f;
    field_0x2c4 = 0;
    field_0x2ce = 0;
    field_0x2cc = 0;
    field_0x2d0 = 0;
    field_0x2f8 = 1;

    field_0x2d4 = fn_8018330C().toUInt32();
    field_0x2d8 = (f32)(field_0x2d4 >> 24);
    field_0x2dc = (f32)((field_0x2d4 >> 16) & 0xFF);
    field_0x2e0 = (f32)((field_0x2d4 >> 8) & 0xFF);
    field_0x2e4 = (f32)(field_0x2d4 & 0xFF);
    field_0x2e8 = -(0.4f * field_0x2d8) / 60.0f;
    field_0x2ec = -(0.4f * field_0x2dc) / 60.0f;
    field_0x2f0 = -(0.4f * field_0x2e0) / 60.0f;
    field_0x3c.x = -2.0f;
    field_0x2b8 = 1;
}

// FIXME(shotgun): 89.7 %, missing/extra + reloc-name
void VaatiTimeUp_804ACCC0::update() {
    if (field_0x2b8 != 0) {
        cls_804A03A8* sub = gGameRoot2_8053AB10->field_0x30;
        if (sub->field_0x284 == 0) {
            sub->field_0x284 = 1;
            sub->field_0x28c = 0.05f;
        }
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
        if (camera->field_0x7f4 == 0) {
            camera->field_0x7f5 = 1;
        }
    }

    switch (field_0x2b8) {
    case 0:
        gGameRoot_8053AAF8->fn_80134FC4(1);
        lbl_8053AD70->fn_8037AE00();
        gGameRoot_8053AAF8->fn_80134FBC(1);
        field_0x2c0 = 60;
        field_0x2b8 = 1;
        break;
    case 1:
        mPos += field_0x3c;
        if (field_0x2c0 <= 0) {
            startSound_801F0E34(0x4003F, 0);
            field_0x230.field_0x58 = 255.0f;
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.z = 0.0f;
            field_0x2c0 = 0;
            field_0x2b8 = 2;
            field_0x2d8 = 0.6f * (f32)(field_0x2d4 >> 24);
            field_0x2dc = 0.6f * (f32)((field_0x2d4 >> 16) & 0xFF);
            field_0x2e0 = 0.6f * (f32)((field_0x2d4 >> 8) & 0xFF);
        } else {
            field_0x2d8 += field_0x2e8;
            field_0x2dc += field_0x2ec;
            field_0x2e0 += field_0x2f0;
        }
        field_0x2c0--;
        break;
    case 2:
        if (field_0x2c0 <= 0) {
            field_0x2c0 = 300;
            field_0x2b8 = 3;
        } else {
            field_0x2c0--;
        }
        field_0x2bc--;
        if (field_0x2bc < 0) {
            field_0x2bc = 10;
            field_0x2c8++;
            field_0x2c8 &= 3;
        }
        break;
    case 3:
        if (field_0x2c4 <= 0 || (field_0x2c0 <= 0 && mPos.x >= field_0x290 && mPos.y >= field_0x294)) {
            if (field_0x2c0 <= 0) {
                field_0x290 = mViewBox.i.x - 192.0f;
                field_0x294 = (mViewBox.i.y + mViewBox.f.y) * 0.5f + 96.0f;
            }
            field_0x2d0 = (s16)(JMath::atanTable_.atan2_(field_0x290 - mPos.x, field_0x294 - mPos.y) * 10430.378f);
            field_0x2c4 = 6;
        }
        addCalcAngle2_801F714C((u16*)&field_0x2cc, field_0x2d0, 20, 0x1000);
        field_0x2a0 = 6.0f * JMath::sincosTable_.sinShort(field_0x2cc);
        field_0x2a4 = 6.0f * JMath::sincosTable_.cosShort(field_0x2cc);
        addCalc2_801F71A4(&field_0x3c.x, field_0x2a0, 0.1f, 0.2f);
        addCalc2_801F71A4(&field_0x3c.y, field_0x2a4, 0.1f, 0.2f);
        field_0x2c4--;
        mPos += field_0x3c;
        field_0x230.fn_801EB418(&mPos);
        if (field_0x2c0 <= 0) {
            if (mViewBox.i.x - 192.0f > mPos.x) {
                field_0x2b8 = 4;
            }
        } else if (field_0x2c0 <= 40 && field_0x2f8 == 1) {
            startSound_801F0E34(0x40041, 0);
            field_0x2f8 = 0;
        }
        field_0x2c0--;
        startSound_801F0E34(0x40051, 0);
        field_0x2bc--;
        if (field_0x2bc < 0) {
            field_0x2bc = 10;
            field_0x2c8++;
            field_0x2c8 &= 3;
        }
        break;
    case 4:
        gGameRoot_8053AAF8->fn_80134FC4(0);
        lbl_8053AD70->fn_8037AE30();
        for (s32 i = 0; i < 4; i++) {
            fn_80238548(i, 40, (const JGeometry::TVec2f*)&mPos, 18);
        }
        field_0x230.field_0x5c = 1;
        field_0x230.field_0x58 = 0.0f;
        gGameRoot_8053AAF8->fn_80134FBC(0);
        fn_801F3668();
        break;
    }

    u32 color = ((u32)field_0x2d8 << 24) | ((u32)field_0x2dc << 16) | ((u32)field_0x2e0 << 8) | (u32)field_0x2e4;
    fn_8013A884(&color);
}

// Frame offsets of the "enemy_gc_sob.bin" sprite, indexed by field_0x2c8.
static const s32 lbl_80472288[4] = {0, 1, 0, 2};

// FIXME: 99.9 %. Only the 1.0f literal: the unit's .sdata2 is not split
// (lbl_80542750 vs @NNN).
void VaatiTimeUp_804ACCC0::draw() {
    SpriteParams_801F5AE0 params;
    params.field_0x2d = 1;
    params.field_0x1c = 1;
    params.field_0x1d = 1;
    params.field_0x30 = 8;
    drawSprite_801F06F0("enemy_gc_sob.bin", 0x466 + lbl_80472288[field_0x2c8], &params, 7, 1.0f, 1.0f);
}

VaatiTimeUp_804ACCC0::~VaatiTimeUp_804ACCC0() {}
