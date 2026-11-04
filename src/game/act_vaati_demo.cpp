#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/GameState.h"
#include "game/Map.h"
#include "JSystem/JMath/JMath.h"
#include "game/Player.h"
#include "game/act/act_vaati_demo.h"

// The animation table of field_0x238: (cel, duration) pairs ended by a cel < 0.
static const s16 lbl_80464990[] = {0, 10, 1, 10, 2, 10, 3, 10, -1, -1};

// Values of the object's field_0x8 that select fn_80115F9C's index; entry 0 is unused.
static const s32 lbl_804649A4[12] = {
    0x00000000, 0x00000334, 0x000003DE, 0x00000474, 0x000004B0, 0x00000564,
    0x000005B4, 0x0000062C, 0x000006F4, 0x000007A8, 0x0000085C, 0x0000091A,
};

static const s16* lbl_80539F08[2] = {lbl_80464990, NULL};

static const char* lbl_8049D930[4] = {
    "e_gufu_1.bti", "e_gufu_2.bti", "e_gufu_3.bti", "e_gufu_4.bti",
};

// FIXME: 94.4 %. The original's branch to the fn_8023DFF4 arm is laid out at the
// end of the loop body with the fn_80230498 arm falling through, and it
// converts the 255 argument from a float at run time (fctiwz of 255.0f);
// the 90.0f literal is lbl_ in the original and @NNN here (data split).
bool cls_8049D940::fn_80115A7C() {
    for (s32 i = 0; i < 4; i++) {
        JGeometry::TVec3f out;
        JGeometry::TVec3f pos = *fn_8023E724(i);
        fn_80115BC4(&pos, &out);
        pos += out;
        f32 dx = pos.x - mPos.x;
        f32 dy = pos.y - mPos.y;
        f32 len = JMAFastSqrt(square(dx) + square(dy));
        if (!(len <= 90.0f)) {
            fn_80230498(i, &pos, field_0x270.mCel + 0x3B5, 255, -128, -128, true);
        } else {
            fn_8023DFF4(i, &pos);
        }
    }
    field_0x270.fn_801D1C18();
    return true;
}

s32 cls_8049D940::vfunc_0x5C() {
    switch (field_0x230) {
    case 11:
        return 1;
    default:
        return 0;
    }
}

s32 cls_8049D940::vfunc_0x58() {
    return field_0x230;
}

void cls_8049D940::vfunc_0x54() {
    if (fn_8011921C()->field_0x4 == 2) {
        s32 value = fn_8011921C()->field_0x8;
        if (value == lbl_804649A4[1]) {
            fn_80115F9C(1);
        } else if (value == lbl_804649A4[2]) {
            fn_80115F9C(2);
        } else if (value == lbl_804649A4[3]) {
            fn_80115F9C(3);
        } else if (value == lbl_804649A4[4]) {
            fn_80115F9C(4);
        } else if (value == lbl_804649A4[5]) {
            fn_80115F9C(5);
        } else if (value == lbl_804649A4[6]) {
            fn_80115F9C(6);
        } else if (value == lbl_804649A4[7]) {
            fn_80115F9C(7);
        } else if (value == lbl_804649A4[8]) {
            fn_80115F9C(8);
        } else if (value == lbl_804649A4[9]) {
            fn_80115F9C(9);
        } else if (value == lbl_804649A4[10]) {
            fn_80115F9C(10);
        } else if (value == lbl_804649A4[11]) {
            fn_80115F9C(11);
        }
    }
}

// FIXME: 98.1 %. The only differences are the literal references (lbl_ in the
// original, @NNN here: data split) and &mPos, which the original computes a
// few instructions earlier, before the layer test.
void cls_8049D940::draw() {
    if (field_0x230 >= 10) {
        fn_801158B8();
    }
    cls_8049D140::draw();
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    params.field_0x30 = 8;
    params.field_0x10.mColor.a = field_0x234;
    params.field_0x1c = 1;
    params.field_0x1d = 1;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    mtx[0][0] = 1.0f;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = 1.0f;
    mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = 1.0f;
    params.mMtx = mtx;
    if (field_0x238.mKey < 4) {
        void* res = field_0x31c[field_0x238.mKey];
        if (res != NULL) {
            if (field_0x4 < 8) {
                lbl_8053AB10->field_0x20->fn_802F745C(&mPos, res, &params);
            }
        }
    }
}

// Defined after draw(), which calls it: above it, MWCC inlines it into draw.
// FIXME: 88.7 %. The original loads the 64.0f and 1.0f literals again after the
// PSMTXTrans call and uses other registers for them; the literal references
// are lbl_ in the original and @NNN here (data split).
void cls_8049D940::fn_801158B8() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    JGeometry::TVec3f pos;
    PSMTXTrans(mtx, 0.0f, 0.0f, 0.0f);
    mtx[0][0] = 64.0f;
    mtx[1][1] = 1.0f;
    mtx[2][2] = 64.0f;
    params.mMtx = mtx;
    params.field_0x14.mColor.r = 0;
    params.field_0x14.mColor.g = 0;
    params.field_0x14.mColor.b = 0;
    params.field_0x14.mColor.a = 255;
    params.field_0x1c = 1;
    params.field_0x1d = 3;
    params.field_0x2d = 1;
    pos = mPos;
    pos.z = 0.0f;
    if (field_0x4 < 8) {
        lbl_8053AB10->field_0x20->fn_802F745C(&pos, field_0x32c, &params);
    }
}

void cls_8049D940::init() {
    cls_8049D140::init();
    for (s32 i = 0; i < 4; i++) {
        field_0x31c[i] = fn_804032E8('TIMG', lbl_8049D930[i]);
    }
    // 0x3F3F3F3F is the four-character type '????'.
    field_0x32c = lbl_8053AAF8->field_0xa4->getResource(0x3F3F3F3F, "kk_black.bti");
    mPos.z = 20.0f;
    field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
    mDrawEnabled = false;
    field_0x238.fn_801D2608(lbl_80539F08, 1);
    field_0x238.fn_801D22DC(0, 2, false);
    field_0x238.fn_801D1F14(0.0f);
    field_0x270.fn_801D2608(lbl_80539F08, 1);
    field_0x270.fn_801D22DC(0, 2, false);
    field_0x270.fn_801D1F14(0.0f);
    field_0x308 = mPos;
}

cls_8049D940::~cls_8049D940() {
    field_0x31c[0] = 0;
    field_0x31c[1] = 0;
    field_0x31c[2] = 0;
    field_0x31c[3] = 0;
    field_0x32c = 0;
    fn_801EE434(field_0x314);
    field_0x314 = NULL;
    fn_801EE434(field_0x318);
    field_0x318 = NULL;
}

cls_8049D940::cls_8049D940() : field_0x230(0), field_0x234(0) {
    field_0x314 = 0;
    field_0x318 = 0;
    field_0x32c = 0;
    field_0x31c[0] = 0;
    field_0x31c[1] = 0;
    field_0x31c[2] = 0;
    field_0x31c[3] = 0;
}
