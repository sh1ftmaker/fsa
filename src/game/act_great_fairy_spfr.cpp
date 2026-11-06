#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Effect.h"
#include "game/act/act_great_fairy_spfr.h"

static const s16 lbl_80465150[] = {
    2, 10,
    4, 10,
    5, 10,
    6, 10,
    -1, -1,
};

static const s16 lbl_80465164[] = {
    2, 1,
    0, 8,
    3, 8,
    1, 8,
    2, 6,
    0, 6,
    3, 6,
    1, 6,
    2, 4,
    0, 4,
    3, 4,
    1, 4,
    2, 4,
    0, 4,
    3, 4,
    1, 4,
    2, 4,
    0, 4,
    3, 4,
    1, 4,
    2, 4,
    0, 4,
    3, 4,
    1, 4,
    7, 30,
    -1, -1,
};

static const s16 lbl_804651CC[] = {
    2, 1,
    0, 8,
    3, 8,
    1, 8,
    2, 6,
    0, 6,
    3, 6,
    1, 6,
    2, 4,
    0, 4,
    3, 4,
    1, 4,
    7, 30,
    -1, -1,
};

// lbl_80465204 (.rodata): values fn_8011921C()->field_0x8 is matched against, by index.
static const s32 lbl_80465204[27] = {
    0, 0, 0, 0, 0, 0x28, 0x64, 0xC8, 0x104, 0x208, 0x26C, 0x32A, 0x366, 0x55A,
    0x578, 0x5BE, 0x5F0, 0x663, 0x6C7, 0x703, 0x767, 0x7CB, 0xC8, 0x12C, 0x190, 0x1F4, 0,
};

static const s16* lbl_8049EB80[3] = {
    lbl_80465150,
    lbl_80465164,
    lbl_804651CC,
};

cls_8049EB8C::cls_8049EB8C()
    : field_0x230(0), field_0x234(0), field_0x238(0), field_0x23c(-1), field_0x240(0),
      field_0x244(0), field_0x248(0), field_0x24c(0), field_0x288(0) {}

cls_8049EB8C::~cls_8049EB8C() {
    releaseEmitter_801EE434(field_0x288);
    field_0x288 = 0;
}

// FIXME: 99.71% (reloc-name: pooled float literals lbl_8053CD28, lbl_8053CD08, lbl_8053CCF0)
void cls_8049EB8C::init() {
    cls_8049D140::init();
    field_0x238 = field_0x90 & 1;
    field_0x248 = (field_0x90 >> 22) & 0x1f;
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    mBox2.set(mBox);
    field_0x80.set(mBox);
    mAnim.fn_801D2608(lbl_8049EB80, 3);
    mAnim.startAnim_801D22DC(0, 2, false);
    mAnim.fn_801D1F14(0.0f);
    mPos.z = 8.0f;
    mDrawEnabled = false;
    if (field_0x238 == 1 && vfunc_0x34()) {
        mDrawEnabled = true;
        JGeometry::TVec3f pos = mPos;
        field_0x288 = spawnEmitter_8013CC50(field_0x4, &pos, 0x184, 0, -1, NULL, NULL, true, 1.0f);
        fn_80132454(2);
    }
}

// FIXME: 99.94% (reloc-name: string lbl_80465270, float lbl_8053CD00)
void cls_8049EB8C::draw() {
    cls_8049D140::draw();
    SpriteParams_801F5AE0 params;
    fn_801F02BC("enemy_gc_sob.bin", mAnim.mCel + 0x2fb, &params, 0x407, 0, false, 1.0f, 1.0f);
}

void cls_8049EB8C::vfunc_0x54() {
    if (fn_8011921C()->field_0x4 != 0xE) {
        return;
    }
    s32 value = fn_8011921C()->field_0x8;
    switch (field_0x238) {
    case 0:
        if (value == lbl_80465204[5]) {
            fn_80132454(5);
        } else if (value == lbl_80465204[6]) {
            fn_80132454(6);
        } else if (value == lbl_80465204[7]) {
            fn_80132454(7);
        } else if (value == lbl_80465204[8]) {
            fn_80132454(8);
        } else if (value == lbl_80465204[9]) {
            fn_80132454(9);
        } else if (value == lbl_80465204[10]) {
            fn_80132454(0xA);
        } else if (value == lbl_80465204[11]) {
            fn_80132454(0xB);
        } else if (value == lbl_80465204[12]) {
            fn_80132454(0xC);
        } else if (value == lbl_80465204[13]) {
            fn_80132454(0xD);
        } else if (value == lbl_80465204[14]) {
            fn_80132454(0xE);
        } else if (value == lbl_80465204[15]) {
            fn_80132454(0xF);
        } else if (value == lbl_80465204[16]) {
            fn_80132454(0x10);
        } else if (value == lbl_80465204[17]) {
            fn_80132454(0x11);
        } else if (value == lbl_80465204[18]) {
            fn_80132454(0x12);
        } else if (value == lbl_80465204[19]) {
            fn_80132454(0x13);
        } else if (value == lbl_80465204[20]) {
            fn_80132454(0x14);
        } else if (value == lbl_80465204[21]) {
            fn_80132454(0x15);
        }
        break;
    case 1:
        if (value == lbl_80465204[4]) {
            fn_80132454(4);
        } else if (value == lbl_80465204[5]) {
            fn_80132454(5);
        } else if (value == lbl_80465204[6]) {
            fn_80132454(6);
        } else if (value == lbl_80465204[22]) {
            fn_80132454(0x16);
        } else if (value == lbl_80465204[23]) {
            fn_80132454(0x13);
        } else if (value == lbl_80465204[24]) {
            fn_80132454(0x14);
        } else if (value == lbl_80465204[25]) {
            fn_80132454(0x19);
        }
        break;
    }
}

// FIXME: 85.83%. missing/extra: ours has 1 more instr (13 vs 12; return 1 branch inverted vs target beq/b)
s32 cls_8049EB8C::vfunc_0x5C() {
    if (field_0x234 < 0x19) {
        if (field_0x234 == 0x15) {
            return 1;
        }
    } else if (field_0x234 < 0x1b) {
        return 1;
    }
    return 0;
}

s32 cls_8049EB8C::vfunc_0x58() {
    return field_0x234;
}
