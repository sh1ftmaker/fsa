#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/RotMatrix.h"
#include "game/act/tu_801A908C.h"

// A counter of the live instances in state 5 (defined elsewhere).
extern s32 lbl_8053ABD0;

// FIXME(shotgun): 100.00 %, match
void cls_804A2FB0::fn_801AA8F4() {
    if (field_0x234 == 0) {
        field_0x24c = field_0x25c + field_0x240;
        switch (field_0x260) {
        case 0:
            field_0x234 = 6;
            field_0x240++;
            field_0x240 &= 1;
            if (field_0x240 != 0) {
                startSound_801F0E34(0x5002C, 0);
            }
            break;
        case 1:
            field_0x234 = 5;
            field_0x240++;
            if (field_0x240 > 3) {
                field_0x240 = 3;
                field_0x2b0 = 1;
            }
            break;
        case 2:
            field_0x234 = 5;
            field_0x240--;
            if (field_0x240 < 0) {
                field_0x240 = 0;
                field_0x2b0 = 1;
            }
            break;
        case 3:
            field_0x234 = 6;
            field_0x240++;
            if (field_0x240 > 3) {
                field_0x240 = 3;
                field_0x2b0 = 1;
            }
            break;
        case 4:
            field_0x234 = 6;
            field_0x240--;
            if (field_0x240 < 0) {
                field_0x240 = 0;
                field_0x2b0 = 1;
            }
            break;
        }
    }
}

// FIXME(shotgun): 100.00 %, match
cls_804A2FB0::~cls_804A2FB0() {
    releaseEmitter_801EE434(field_0x2b8);
    if (field_0x258 == 0 && gGameRoot_8053AAF8->field_0x4c != 9) {
        gGameRoot_8053AAF8->fn_80138B7C();
    }
    if (field_0x258 == 5) {
        lbl_8053ABD0--;
    }
}

// FIXME(shotgun): 96.95 %, missing/extra + reloc-name
void cls_804A2FB0::update() {
    switch (field_0x258) {
    case 0:
    case 5:
        fn_801A96B4();
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        fn_801A9CF4();
        break;
    }
    if (field_0x234 > 0) {
        field_0x234--;
    }
    if (field_0x238 > 0) {
        field_0x238--;
    }
    if (field_0x23c > 0) {
        field_0x23c--;
    }
}

// FIXME(shotgun): 99.00 %, regalloc-only
void cls_804A2FB0::fn_801A9374() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    params.field_0x1d = 2;
    JGeometry::TVec3f rot(0.0f, field_0x2b4 / -10430.228f, 0.0f);
    setRotScaleMtx(mtx, rot, 1.0f);
    params.mMtx = mtx;
    void* res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', lbl_804A2F2C[field_0x24c]);
    fn_801EFD34(res, &params, -1, 0.0f, 0.0f, 0.0f);
}

// FIXME: 99.0 %. regalloc-only: the original keeps the angle (and then
// cos y) in f30 and cos x in f31, ours the other way round, and sin x / sin z
// get f29 / f27 there, f27 / f29 here.
void cls_804A2FB0::fn_801A90D4() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    params.field_0x1d = 1;
    JGeometry::TVec3f rot(0.0f, field_0x2b4 / -10430.228f, 0.0f);
    setRotScaleMtx(mtx, rot, 1.0f);
    params.mMtx = mtx;
    void* res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', lbl_804A2F2C[32]);
    fn_801EFD34(res, &params, -1, 0.0f, 0.0f, 0.0f);
}

void cls_804A2FB0::draw() {
    switch (field_0x258) {
    case 0:
    case 5:
        fn_801A90D4();
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        fn_801A9374();
        break;
    }
}

// fn_801A96B4 (0x640), fn_801A9CF4 (0x770), fn_801AA508 (0x3EC): not written (budget).
