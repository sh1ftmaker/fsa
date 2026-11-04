#include "game/GameAudio.h"
#include "game/GameView.h"
#include "game/tu_802EF76C.h"
#include "game/tu_80182914.h"
#include "game/act/act_vaati_time_up.h"

// Frame offsets of the "enemy_gc_sob.bin" sprite, indexed by field_0x2c8.
static const s32 lbl_80472288[4] = {0, 1, 0, 2};

VaatiTimeUp_804ACCC0::~VaatiTimeUp_804ACCC0() {}

void VaatiTimeUp_804ACCC0::init() {
    cls_801EBA14 rec;

    mViewBox = fn_802D6A9C()->mVisibleBox;
    mPos.x = 64.0f + mViewBox.f.x;
    mPos.y = (mViewBox.i.y + mViewBox.f.y) * 0.5f + 96.0f;
    mPos.z = 100.0f;
    rec.field_0x0 = 1;
    rec.field_0x4 = 1;
    rec.field_0x8 = 100.0f;
    rec.field_0xc = 90.0f;
    rec.field_0x10 = 20.0f;
    field_0x230.field_0xc = rec;
    field_0x230.fn_801EB710();

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

    field_0x2d4.mColor = fn_8018330C();
    field_0x2d8 = (f32)(field_0x2d4.mWord >> 24);
    field_0x2dc = (f32)((field_0x2d4.mWord >> 16) & 0xFF);
    field_0x2e0 = (f32)((field_0x2d4.mWord >> 8) & 0xFF);
    field_0x2e4 = (f32)(field_0x2d4.mWord & 0xFF);
    field_0x2e8 = -(0.4f * field_0x2d8) / 60.0f;
    field_0x2ec = -(0.4f * field_0x2dc) / 60.0f;
    field_0x2f0 = -(0.4f * field_0x2e0) / 60.0f;
    field_0x3c.x = -2.0f;
    field_0x2b8 = 1;
}

void VaatiTimeUp_804ACCC0::draw() {
    SpriteParams_801F5AE0 params;
    params.field_0x2d = 1;
    params.field_0x1c = 1;
    params.field_0x1d = 1;
    params.field_0x30 = 8;
    fn_801F06F0("enemy_gc_sob.bin", 0x466 + lbl_80472288[field_0x2c8], &params, 7, 1.0f, 1.0f);
}
