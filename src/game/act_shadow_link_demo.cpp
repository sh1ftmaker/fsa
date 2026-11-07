#include "game/GameAudio.h"
#include "game/Camera.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/SpriteParams.h"
#include "game/EnemyBase.h"
#include "game/act/act_shadow_link_demo.h"

// FIXME(shotgun): not written: fn_8011B878 (0x534), update fn_8011C308 (0x854)

// Values that vfunc_0x54 compares the object's field_0x8 with, by the index it
// passes to fn_8011B878. Entries 0, 5, 13, 21-26, 30 and 34 are never compared.
static const s32 lbl_80464AF8[35] = {
    0x00000000, 0x0000083E, 0x0000087A, 0x000008DE, 0x00000901, 0x00000956, 0x00000A8C,
    0x00000AF0, 0x00000E1A, 0x00000E56, 0x00000E92, 0x00000ECE, 0x00000F0A, 0x00000000,
    0x0000005A, 0x000000D2, 0x0000014A, 0x00000384, 0x00000479, 0x000004D3, 0x0000050F,
    0x00000573, 0x00000573, 0x00000573, 0x00000573, 0x00000000, 0x00000000, 0x00000000,
    0x00000226, 0x00000280, 0x00000280, 0x000002F8, 0x00000320, 0x00000384, 0x00000000,
};

cls_8049DB90::cls_8049DB90() {
    field_0x230 = 1;
    field_0x231 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0.0f;
    field_0x248 = 0.0f;
    field_0x254 = 0.0f;
    field_0x250 = 0.0f;
    field_0x25c = 0.0f;
    field_0x258 = 0.0f;
}

// FIXME(shotgun): 99.9 %, reloc-name when compiled, but then __ct__ drops from 100 % to 99.8 % (the .sdata2 literal pool gains 80.0f), so it stays out of the build.
#ifdef NONMATCHING
void cls_8049DB90::init() {
    cls_8049D140::init();
    field_0x234 = field_0x90 & 0xF;
    field_0x98 = (s8)((field_0x90 >> 4) & 3);
    switch (field_0x234) {
    case 1:
        fn_8011B878(0xD);
        field_0x98 = 3;
        mSequencer.fn_800FEB78(0);
        break;
    case 2:
        fn_8011B878(0x16);
        field_0x98 = 3;
        mSequencer.fn_800FEB78(1);
        mPos.y += 80.0f;
        break;
    default:
        mSequencer.fn_800FEB78(0);
        break;
    }
    mSequencer.mDir = field_0x98;
}
#endif

// FIXME(shotgun): 94.83 %, missing/extra (gated: its literals drop __ct__ from 100 % to 99.83 %)
#ifdef NONMATCHING
void cls_8049DB90::draw() {
    cls_8049D140::draw();
    SpriteParams_801F5AE0 params;
    params.field_0x2e = 0x40;
    s32 frame = mSequencer.mFrame;
    if (mSpriteParams.mMtx == NULL) {
        fn_801EF830(field_0x4, &mPos, &params, 7, "player_sob.bin", frame, NULL);
    }
    if (field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, NULL, 1.0f, 1.0f);
    }
    fn_803075AC(mSerial, field_0x4, &mPos, mSequencer.mFrame, 0, 1, -1, false, 0, 0);
}
#endif

void cls_8049DB90::vfunc_0x54() {
    s32 value = fn_8011921C()->field_0x8;
    switch (field_0x234) {
    case 0:
        if (value == lbl_80464AF8[1]) {
            fn_8011B878(1);
        } else if (value == lbl_80464AF8[2]) {
            fn_8011B878(2);
        } else if (value == lbl_80464AF8[3]) {
            fn_8011B878(3);
        } else if (value == lbl_80464AF8[4]) {
            fn_8011B878(4);
        } else if (value == lbl_80464AF8[6]) {
            fn_8011B878(6);
        } else if (value == lbl_80464AF8[7]) {
            fn_8011B878(7);
        } else if (value == lbl_80464AF8[8]) {
            fn_8011B878(8);
        } else if (value == lbl_80464AF8[9]) {
            fn_8011B878(9);
        } else if (value == lbl_80464AF8[10]) {
            fn_8011B878(10);
        } else if (value == lbl_80464AF8[11]) {
            fn_8011B878(11);
        } else if (value == lbl_80464AF8[12]) {
            fn_8011B878(12);
        }
        break;
    case 1:
        if (fn_8011921C()->field_0x4 == 0x11) {
            if (value == lbl_80464AF8[14]) {
                fn_8011B878(14);
            } else if (value == lbl_80464AF8[15]) {
                fn_8011B878(15);
            } else if (value == lbl_80464AF8[16]) {
                fn_8011B878(16);
            } else if (value == lbl_80464AF8[17]) {
                fn_8011B878(17);
            } else if (value == lbl_80464AF8[18]) {
                fn_8011B878(18);
            } else if (value == lbl_80464AF8[19]) {
                fn_8011B878(19);
            } else if (value == lbl_80464AF8[20]) {
                fn_8011B878(20);
            }
        } else if (fn_8011921C()->field_0x4 == 2) {
            if (value == lbl_80464AF8[27]) {
                fn_8011B878(27);
            } else if (value == lbl_80464AF8[28]) {
                fn_8011B878(28);
            } else if (value == lbl_80464AF8[29]) {
                fn_8011B878(29);
            } else if (value == lbl_80464AF8[31]) {
                fn_8011B878(31);
            } else if (value == lbl_80464AF8[32]) {
                fn_8011B878(32);
            } else if (value == lbl_80464AF8[33]) {
                fn_8011B878(33);
            }
        }
        break;
    case 2: // nothing sets state 2; the original's switch has an entry for it
        break;
    }
}

s32 cls_8049DB90::vfunc_0x58() {
    return field_0x238;
}

s32 cls_8049DB90::vfunc_0x5C() {
    switch (field_0x238) {
    case 12:
    case 21:
    case 24:
    case 33:
    case 34:
        return 1;
    }
    return 0;
}

// Pushes the 4-float box at 0x250 to the scripted camera (mode 5).
static inline void setCameraBox(cls_8049DB90* self) {
    Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
    if (cam->field_0x7f5 == 0) {
        cam->field_0x7f4 = 1;
        cam->field_0x7f8 = 5;
        cam->field_0x808.set(self->field_0x250, self->field_0x254, self->field_0x258, self->field_0x25c);
        cam->field_0x7f6 = 0;
    }
}

// FIXME(shotgun): 5.55 %, missing/extra (gated: its literals drop __ct__ from 100 % to 99.83 %)
#ifdef NONMATCHING
void cls_8049DB90::fn_8011B3F0() {
    Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
    if (cam->field_0x179 == 0) {
        cam->field_0x17a = 1;
        cam->field_0x179 = 0;
    }
    switch (field_0x238) {
    case 1:
    case 2:
    case 3:
        field_0x250 = mPos.x - 64.0f;
        field_0x254 = mPos.y - 32.0f;
        field_0x258 = 64.0f + mPos.x;
        field_0x25c = 32.0f + mPos.y;
        setCameraBox(this);
        break;
    case 4:
    case 5: {
        JGeometry::TVec3f p(mPos);
        p.y += field_0x248;
        fn_801CD950(&field_0x248, 32.0f, 0.8f);
        field_0x250 = p.x - 64.0f;
        field_0x254 = p.y - 32.0f;
        field_0x258 = 64.0f + p.x;
        field_0x25c = 32.0f + p.y;
        setCameraBox(this);
        break;
    }
    case 6: {
        JGeometry::TVec3f p(mPos);
        p.y += field_0x248;
        fn_801CD950(&field_0x248, 0.0f, 0.8f);
        field_0x250 = p.x - 64.0f;
        field_0x254 = p.y - 32.0f;
        field_0x258 = 64.0f + p.x;
        field_0x25c = 32.0f + p.y;
        setCameraBox(this);
        break;
    }
    case 7: {
        s32 diff = fn_8011921C()->field_0x8 - lbl_80464AF8[7];
        if (diff >= 0xB4) {
            if (diff > 0x19A && diff != 0) {
                fn_801CD950(&field_0x244, 0.0f, -0.007f);
            } else {
                fn_801CD950(&field_0x244, 0.1f, 0.0035f);
            }
            fn_801CD950(&field_0x254, (mPos.y - 32.0f) - 24.0f, -field_0x244);
            fn_801CD950(&field_0x25c, (32.0f + mPos.y) + 24.0f, field_0x244);
        }
        setCameraBox(this);
        break;
    }
    case 8: {
        Camera_804AA9E4* c = gGameRoot2_8053AB10->field_0x24;
        if (c->field_0x7f5 == 0) {
            c->field_0x7f4 = 1;
            c->field_0x7f8 = 4;
            c->field_0x7fc.set(mPos.x, mPos.y, mPos.z);
            c->field_0x7f6 = 0;
        }
        break;
    }
    case 9:
    case 10:
    case 11: {
        Camera_804AA9E4* c = gGameRoot2_8053AB10->field_0x24;
        if (c->field_0x840 == 0) {
            c->field_0x840 = 1;
            c->field_0x844.set(mPos.x, mPos.y, mPos.x, mPos.y);
            c->field_0x854 = 1.0f;
            c->field_0x1d4 = -1.0f;
            c->field_0x1d8 = -1.0f;
        }
        break;
    }
    default:
        setCameraBox(this);
        break;
    }
}
#endif

cls_8049DB90::~cls_8049DB90() {}
