#include "game/GameAudio.h"
#include "game/DrawShadow.h"
#include "game/act/act_poe.h"

// The six "e_obake_N.bti" texture names drawn by draw().
static const char* lbl_8049F318[6] = {
    "e_obake_0.bti", "e_obake_1.bti", "e_obake_2.bti",
    "e_obake_3.bti", "e_obake_4.bti", "e_obake_5.bti",
};

// Per state and whether the actor's angle runs negative: a frame index for
// draw()'s sprite (lbl_8049F2B8[row * 8 + field_0x238[8]]).
static s32 lbl_8049F2B8[16] = {0, 1, 2, 3, 4, 5, 2, 3, 6, 7, 8, 9, 10, 11, 8, 9};

Poe_8049F330::~Poe_8049F330() {}

void Poe_8049F330::draw() {
    if (field_0x238[7] == 0 && field_0x94 >= 4 && field_0x94 <= 7) {
        if (!vfunc_0x34()) {
            return;
        }
    }
    if (field_0x94 == 8) {
        return;
    }
    if (field_0x230 == 1 || field_0x230 == 0) {
        SpriteParams_801F5AE0 params;
        Mtx mtx;
        drawShadow(field_0x4, &mPos, JUtility::TColor(0, 0, 0, 0x80), 40.0f);
        params.field_0x10.mColor.a = (u8)field_0x270;
        params.field_0x30 = 9;
        if (field_0x94 == 3 || field_0x94 == 7) {
            s32 index = field_0x25c;
            f32 flip = -1.0f;
            if (index > 5) {
                flip = 1.0f;
                index -= 6;
            }
            mtx[0][0] = flip;
            mtx[0][1] = 0.0f;
            mtx[0][2] = 0.0f;
            mtx[1][0] = 0.0f;
            mtx[1][1] = 1.0f;
            mtx[1][2] = 0.0f;
            mtx[2][0] = 0.0f;
            mtx[2][1] = 0.0f;
            mtx[2][2] = 1.0f;
            mtx[0][3] = 0.0f;
            mtx[1][3] = 0.0f;
            mtx[2][3] = 0.0f;
            params.mMtx = mtx;
            void* res = lbl_8053AAF8->field_0xa4->getResource('TIMG', lbl_8049F318[index]);
            fn_801EFD34(res, &params, 7, 0.0f, -8.0f, 0.0f);
        } else {
            fn_801F06F0("enemy_gc_sob.bin", field_0x25c + 0x84, &params, 7, 1.0f, 1.0f);
        }
    }
}

// FIXME: 98.88 %. Regalloc only in the three random draws (the manager and
// LCG-constant registers swap) and the tail's row index (r3 in the original);
// the literals and the jump table are @NNN vs lbl_/jumptable_ names (data
// split, not code).
void Poe_8049F330::init() {
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    field_0x94 = field_0x90 & 0xFF;
    field_0x26c = (field_0x90 >> 8) & 0xFFFF;
    field_0x238[0] = 0;
    field_0x238[1] = 0;
    field_0x238[2] = 0;
    field_0x238[3] = 0;
    field_0x238[4] = 0;
    field_0x238[5] = 0;
    field_0x238[6] = 0;
    field_0x238[7] = 0;
    field_0x238[8] = 0;
    field_0x270 = 255.0f;
    field_0x238[1] = (s32)(60.0f * lbl_8053AAF8->mRandom.get_ufloat_1()) + 60;
    field_0x234 = 0;
    field_0xb0 = 0;
    field_0x260.x = mPos.x;
    field_0x260.y = mPos.y;
    field_0x260.z = mPos.z;
    switch (field_0x94) {
    case 6:
        field_0x270 = 0.0f;
        // fall through
    case 2:
        field_0x238[4] = (s32)lbl_8053AAF8->mRandom.get_ufloat_1();
        field_0x238[0] = (s32)(150.0f * lbl_8053AAF8->mRandom.get_ufloat_1()) + 150;
        if (field_0x94 == 2) {
            field_0xb0 = 0x201;
        }
        mPos.z = 24.0f;
        field_0x230 = 1;
        break;
    case 10:
        fn_801F0E34(0x4001D, 0);
        field_0x270 = 0.0f;
        mPos.z = 24.0f;
        field_0x238[0] = 30;
        field_0x3c.x = 5.0f * JMASinShort(field_0x26c);
        field_0x3c.y = 5.0f * JMACosShort(field_0x26c);
        field_0x230 = 0;
        break;
    case 4:
    case 9:
        fn_801F0E34(0x4001D, 0);
        field_0x270 = 0.0f;
        // fall through
    case 0:
        if (field_0x94 == 0) {
            field_0xb0 = 0x201;
        }
        mPos.z = 24.0f;
        field_0x230 = 1;
        break;
    case 3:
    case 7:
        mBox.i.x = -12.0f;
        mBox.i.y = -12.0f;
        mBox.f.x = 12.0f;
        mBox.f.y = 12.0f;
        mPos.z = 24.0f;
        mLife = 100;
        field_0xb0 = 0x4000000;
        field_0x230 = 1;
        break;
    case 1:
    case 5:
        field_0x270 = 0.0f;
        field_0x230 = 2;
        break;
    }
    s32 row = 0;
    if (field_0x3c.x < 0.0f) {
        row = 1;
    }
    field_0x25c = lbl_8049F2B8[row * 8 + field_0x238[8]];
}
