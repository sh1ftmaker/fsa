#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/EnemyBase.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/GameState.h"
#include "game/Map.h"
#include "game/Message.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "game/Player.h"
#include "game/act/act_vaati_demo.h"
#include "game/cls_804A8638.h"
#include "game/tu_803B8C74.h"

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

cls_8049D940::cls_8049D940() : field_0x230(0), field_0x234(0) {
    field_0x314 = 0;
    field_0x318 = 0;
    field_0x32c = 0;
    field_0x31c[0] = 0;
    field_0x31c[1] = 0;
    field_0x31c[2] = 0;
    field_0x31c[3] = 0;
}

cls_8049D940::~cls_8049D940() {
    field_0x31c[0] = 0;
    field_0x31c[1] = 0;
    field_0x31c[2] = 0;
    field_0x31c[3] = 0;
    field_0x32c = 0;
    releaseEmitter_801EE434(field_0x314);
    field_0x314 = NULL;
    releaseEmitter_801EE434(field_0x318);
    field_0x318 = NULL;
}

// FIXME: 99.93 %, reloc-name; behaviour clean.
// Only the 20.0f literal: .sdata2 not split (lbl_8053C704 vs @NNN).
void cls_8049D940::init() {
    cls_8049D140::init();
    for (s32 i = 0; i < 4; i++) {
        field_0x31c[i] = fn_804032E8('TIMG', lbl_8049D930[i]);
    }
    // '????' (escaped: "??'" would be a trigraph).
    field_0x32c = gGameRoot_8053AAF8->field_0xa4->getResource('?\?\?\?', "kk_black.bti");
    mPos.z = 20.0f;
    field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
    mDrawEnabled = false;
    field_0x238.fn_801D2608(lbl_80539F08, 1);
    field_0x238.startAnim_801D22DC(0, 2, false);
    field_0x238.fn_801D1F14(0.0f);
    field_0x270.fn_801D2608(lbl_80539F08, 1);
    field_0x270.startAnim_801D22DC(0, 2, false);
    field_0x270.fn_801D1F14(0.0f);
    field_0x308 = mPos;
}

static inline void updateBobbing(cls_8049D940* self) {
    static s16 lbl_8053AA88 = 0;
    self->mPos.z = 4.0f * JMath::sincosTable_.sinShort(lbl_8053AA88) + 20.0f;
    lbl_8053AA88 += 500;
    if (self->field_0x314 != NULL) {
        setEmitterPos_8013CB44(self->field_0x314, &self->mPos, 1, 1, true);
    }
    if (self->field_0x318 != NULL) {
        setEmitterPos_8013CB44(self->field_0x318, &self->mPos, 1, 1, true);
    }
}

// FIXME: 99.79 %, reloc-name; behaviour drift.
// only the .sdata2 literal and .sbss symbol names differ.
void cls_8049D940::update() {
    cls_8049D140::update();
    if (field_0x230 >= 2 && field_0x230 <= 10) {
        cls_804A03A8* sub = gGameRoot2_8053AB10->field_0x30;
        if (sub->field_0x284 == 0) {
            sub->field_0x284 = 1;
            sub->field_0x28c = 0.05f;
        }
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
        JGeometry::TVec3f pos = mPos;
        if (camera->field_0x7f5 == 0) {
            camera->field_0x7f4 = 1;
            camera->field_0x7f8 = 4;
            camera->field_0x7fc.x = pos.x;
            camera->field_0x7fc.y = pos.y;
            camera->field_0x7fc.z = pos.z;
            camera->field_0x7f6 = 0;
        }
    }
    switch (field_0x230) {
    case 3:
        if (mSpawnPos.y - mPos.y <= 24.0f) {
            fn_801CD950(&field_0x3c.y, 0.4f, 0.002f);
        } else {
            fn_801CD950(&field_0x3c.y, 0.0f, -0.01f);
        }
        if (mPos.y - mSpawnPos.y >= 48.0f) {
            fn_8011921C()->fn_801183BC();
        }
        fn_80115A7C();
        break;
    case 4:
        fn_80115A7C();
        break;
    case 6:
        if (field_0x234 < 0xFE) {
            field_0x234 += 5;
            if (field_0x234 >= 0xFE) {
                field_0x234 = 0xFE;
            }
        }
        updateBobbing(this);
        break;
    case 7:
        updateBobbing(this);
        break;
    case 8:
        if (field_0x234 != 0) {
            field_0x234 -= 5;
            if (field_0x234 == 0) {
                field_0x234 = 0;
            }
        }
        break;
    case 10:
        switch (fn_8011921C()->field_0x8) {
        case 0x889:
        case 0x8AC:
        case 0x8CA:
            startSound_801F0E34(0x4006B, 0);
            break;
        }
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_803865B8(0)) {
            fn_80388EEC(0);
            fn_8011921C()->fn_801183BC();
        }
        break;
    case 0:
    case 11:
        break;
    }
    mPos += field_0x3c;
    if (field_0x230 <= 6) {
        field_0x308.x = mPos.x;
        field_0x308.y = mPos.y;
        field_0x308.z = mPos.z;
    }
    field_0x2a8.fn_801EB418(&field_0x308);
    if (field_0x2a8.fn_801EB648()) {
        startSound_801F0E34(0x40051, 0);
    }
    field_0x238.fn_801D1C18();
}

// FIXME: 98.09 %, scheduling + reloc-name; behaviour drift.
// The only differences are the literal references (lbl_ in the
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
    params.field_0x10.a = field_0x234;
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
                gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, res, &params);
            }
        }
    }
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

s32 cls_8049D940::vfunc_0x58() {
    return field_0x230;
}

s32 cls_8049D940::vfunc_0x5C() {
    switch (field_0x230) {
    case 11:
        return 1;
    default:
        return 0;
    }
}

// FIXME: 94.36 %, scheduling+regalloc + reloc-name; behaviour drift.
// register allocation and instruction scheduling
// in cases 9, 10 and 11, plus .sdata2 literal symbol names.
void cls_8049D940::fn_80115F9C(s32 index) {
    switch (index) {
    case 1:
        field_0x2a8.fn_801EB710(field_0x4, &field_0x308);
        break;
    case 2:
        field_0x2a8.field_0x58 = 255.0f;
        gGameRoot_8053AAF8->fn_80134DE4(5, 0);
        break;
    case 3:
        fn_8011921C()->fn_80118454();
        break;
    case 4:
        field_0x3c.y = 0.0f;
        for (s32 i = 0; i < 4; i++) {
            fn_80230FD0(i, 0x10003);
        }
        break;
    case 6:
        mDrawEnabled = true;
        field_0x314 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x228, 1, 1, NULL, NULL, true, 1.0f);
        field_0x318 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x229, 1, 1, NULL, NULL, true, 1.0f);
        startSound_801F0E34(0x40089, 0);
        break;
    case 7:
        startSound_801F0E34(0x40041, 0);
        break;
    case 8:
        mDrawEnabled = false;
        releaseEmitter_801EE434(field_0x314);
        field_0x314 = NULL;
        releaseEmitter_801EE434(field_0x318);
        field_0x318 = NULL;
        field_0x2a8.field_0x5c = 1;
        field_0x2a8.field_0x58 = 0.0f;
        startSound_801F0E34(0x400BA, 0);
        break;
    case 9: {
        fn_80119DD0()->fn_80119998(120, 0, 1, 0, 0, 70, 0);
        s32 serial = getActorManager_801F856C()->fn_801F7A08('SWEV');
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            static_cast<cls_804A8638*>(actor)->fn_802B3A3C();
        }
        break;
    }
    case 10: {
        mDrawEnabled = true;
        JGeometry::TVec3f pos;
        pos.z = 0.0f;
        pos.y = (s32)JUTVideo::getManager()->getRenderMode()->efbHeight * 0.5f - 100.0f;
        pos.x = (s32)JUTVideo::getManager()->getRenderMode()->fbWidth * 0.5f;
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_8038BD08(0, 0x4B4, field_0x4, pos, 0x62)) {
            fn_8011921C()->fn_80118454();
            startSound_801F0E34(0x4006C, 0);
        }
        break;
    }
    case 11:
        lbl_80529DEC.field_0x237 = 6;
        lbl_80529DEC.field_0x118 = 0;
        lbl_80529DEC.field_0x11c = 0;
        lbl_8050EC80.getCurrentFile().field_0x65 |= 1;
        gGameRoot_8053AAF8->fn_80136798(6, 0);
        break;
    case 0:
        break;
    }
    field_0x230 = index;
}

// Not auto-inlined: fn_80115BC4 calls fn_80115D8C out of line in the original (docs/INLINING.md, pragma
// rule 1).
#pragma auto_inline off
// FIXME: 98.22 %, regalloc-only + reloc-name; behaviour clean.
// FPR allocation in the distance and target position
// math, plus .sdata2 literal symbol names.
void cls_8049D940::fn_80115D8C(const JGeometry::TVec3f* pos, JGeometry::TVec3f* out) {
    out->x = 0.0f;
    out->y = 0.0f;
    out->z = 0.0f;
    if (pos->z <= 100.0f) {
        f32 dx = pos->x - mPos.x;
        f32 dy = pos->y - mPos.y;
        if (square(dx) + square(dy) <= 400.0f) {
            f32 c = JMath::sincosTable_.cosShort(-0x600);
            f32 s = JMath::sincosTable_.sinShort(-0x600);
            JGeometry::TVec2f rot;
            rot.x = dx * c - dy * s;
            rot.y = dx * s + dy * c;
            if (rot.squared() <= JGeometry::TUtilf::epsilon()) {
                rot.x = 0.0f;
                rot.y = 0.1f;
            } else {
                f32 len = rot.length();
                fn_801CD950(&len, 20.0f, 0.9f);
                rot.normalize();
                rot.scale(len);
            }
            JGeometry::TVec3f target;
            target.x = rot.x + mPos.x;
            target.y = rot.y + mPos.y;
            target.z = pos->z;
            out->sub(target, *pos);
            out->z = 0.7f;
        }
    }
}
#pragma auto_inline reset

// Not auto-inlined: fn_80115A7C calls fn_80115BC4 out of line in the original (docs/INLINING.md, pragma
// rule 1).
#pragma auto_inline off
// FIXME: 92.98 %, missing/extra + reloc-name; behaviour near.
// one extra frsp after the __fsel
// (the intrinsic returns a double; the original scales by the fsel result directly),
// plus .sdata2 literal symbol names.
void cls_8049D940::fn_80115BC4(const JGeometry::TVec3f* pos, JGeometry::TVec3f* out) {
    JGeometry::TVec3f pull(0.0f, 0.0f, 0.0f);
    if (pos->z <= 100.0f) {
        f32 dx = pos->x - mPos.x;
        f32 dy = pos->y - mPos.y;
        f32 dist = JMAFastSqrt(square(dx) + square(dy));
        if (dist <= 90.0f) {
            f32 t = (dist - 90.0f) * -0.014285714f;
            if (t > 1.0f) {
                t = 1.0f;
            } else if (t < 0.0f) {
                t = 0.0f;
            }
            f32 speed = 0.2f + 0.5f * t;
            pull.sub(mPos, *pos);
            pull.z = 0.0f;
            pull.normalize();
            pull.scale(__fsel(dist - speed, speed, dist));
        }
    }
    JGeometry::TVec3f spin;
    fn_80115D8C(pos, &spin);
    out->set(pull);
    out->add(spin);
}
#pragma auto_inline reset

// Not auto-inlined: update calls fn_80115A7C out of line in the original (docs/INLINING.md, pragma
// rule 1).
#pragma auto_inline off
// FIXME: 94.39 %, missing/extra; behaviour drift.
// The original's branch to the fn_8023DFF4 arm is laid out at the
// end of the loop body with the fn_80230498 arm falling through, and it
// converts the 255 argument from a float at run time (fctiwz of 255.0f);
// the 90.0f literal is lbl_ in the original and @NNN here (data split).
bool cls_8049D940::fn_80115A7C() {
    for (s32 i = 0; i < 4; i++) {
        JGeometry::TVec3f out;
        JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
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
#pragma auto_inline reset

// Not auto-inlined: draw calls fn_801158B8 out of line in the original (docs/INLINING.md, pragma
// rule 1).
#pragma auto_inline off
// FIXME: 88.70 %, missing/extra + reloc-name; behaviour clean.
// The original loads the 64.0f and 1.0f literals again after the
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
    params.field_0x14.r = 0;
    params.field_0x14.g = 0;
    params.field_0x14.b = 0;
    params.field_0x14.a = 255;
    params.field_0x1c = 1;
    params.field_0x1d = 3;
    params.field_0x2d = 1;
    pos = mPos;
    pos.z = 0.0f;
    if (field_0x4 < 8) {
        gGameRoot2_8053AB10->field_0x20->fn_802F745C(&pos, field_0x32c, &params);
    }
}
#pragma auto_inline reset
