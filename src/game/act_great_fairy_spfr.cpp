#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Effect.h"
#include "game/Camera.h"
#include "game/ActorManager.h"
#include "game/Player.h"
#include "game/Message.h"
#include "game/cls_804AE9B0.h"
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

// FIXME: 96.26%. missing/extra + reloc-name: ours has 2 fewer instrs (345 vs 347); jump table and float literal relocations.
void cls_8049EB8C::update() {
    cls_8049D140::update();
    JGeometry::TBox2f box;
    box.set(mBox);
    box.i.x += mPos.x;
    box.f.x += mPos.x;
    box.i.y += mPos.y;
    box.f.y += mPos.y;
    fn_801321E4();
    switch (field_0x234) {
    case 0:
        switch (field_0x238) {
        case 0:
            fn_8011921C()->fn_801183BC();
            fn_80132454(4);
            break;
        case 1:
            if (vfunc_0x34()) {
                fn_8011921C()->fn_801186B0(14);
            }
            break;
        }
        break;
    case 1:
        field_0x244++;
        if (field_0x244 >= 40) {
            JGeometry::TVec3f pos = mPos;
            field_0x288 = spawnEmitter_8013CC50(field_0x4, &pos, 0x184, 0, -1, NULL, NULL, true, 1.0f);
            mDrawEnabled = true;
            startSound_801F0E34(0x40090, 0);
            fn_80132454(2);
        }
        break;
    case 2:
        switch (field_0x238) {
        case 1: {
            JGeometry::TBox2f box2;
            box2.set(mBox);
            box2.i.x += mPos.x;
            box2.f.x += mPos.x;
            box2.i.y += mPos.y;
            box2.f.y += mPos.y;
            fn_8022FEB8(&box2, field_0x4, mSerial, true, false);
            s32 player = fn_8022FD8C(mSerial);
            if (player < 0) {
                player = -1;
            } else if (!fn_802300EC(player)) {
                player = -1;
            } else {
                JGeometry::TVec3f pos = mPos;
                pos.y -= 64.0f;
                if (fn_8038AD24(player, 0x2B6, mSerial, field_0x4, pos, 0x28)) {
                } else {
                    player = -1;
                }
            }
            if (player >= 0) {
                field_0x23c = player;
                fn_80132454(3);
            }
            break;
        }
        }
        break;
    case 3:
        fn_8022FF8C(field_0x23c, &box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x23c, mSerial)) {
            fn_80132454(2);
        }
        break;
    case 6:
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_803865B8(0)) {
            fn_80388EEC(0);
            fn_8011921C()->fn_801183BC();
        }
        break;
    case 8:
        field_0x24c++;
        if (field_0x24c >= 15) {
            field_0x24c = 0;
            JGeometry::TVec3f pos(0.0f, 0.0f, 0.0f);
            s32 bari = getActorManager_801F856C()->fn_801F7A08('BARI');
            if (bari >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(bari);
                if (actor != NULL) {
                    pos = actor->mPos;
                }
            }
            pos.x -= 40.0f;
            pos.y += 180.0f;
            pos.x += 80.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
            pos.y += 32.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
            getActorManager_801F856C()->spawnActor_801F9484('SLR1', field_0x4, &pos, 0x102, -1, -1);
        }
        break;
    case 9:
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_803865B8(0)) {
            fn_80388EEC(0);
            fn_8011921C()->fn_801183BC();
        }
        break;
    case 12:
        lbl_8053AD70->fn_8037AE00();
        break;
    case 15:
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_803865B8(0)) {
            fn_80388EEC(0);
            fn_8011921C()->fn_801183BC();
        }
        break;
    case 19:
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_803865B8(0)) {
            fn_80388EEC(0);
            fn_8011921C()->fn_801183BC();
        }
        break;
    case 26:
        break;
    default:
        break;
    }
    setEmitterPos_8013CB44(field_0x288, &mPos, 0, -1, true);
    if (field_0x230 != 0) {
        fn_80239914(&box, field_0x4, mSerial);
    }
    mAnim.fn_801D1C18();
    if (mDrawEnabled) {
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
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

// FIXME: 92.79% (scheduling+regalloc: f1/f2/r4; reloc-name: literals lbl_8053CD18, lbl_8053CD1C, lbl_8053CD20)
void cls_8049EB8C::fn_801321E4() {
    switch (field_0x234) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 21:
    case 23:
    case 24:
    case 25:
    case 26:
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 22:
        if (field_0x238 == 0) {
            gGameRoot2_8053AB10->field_0x24->field_0x17d = 1;
        } else {
            gGameRoot2_8053AB10->field_0x24->field_0x179 = 1;
            Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
            f32 y1 = mPos.y + 96.0f;
            f32 x = mPos.x;
            f32 y0 = mPos.y - 96.0f;
            if (camera->field_0x7f5 == 0) {
                camera->field_0x7f4 = 1;
                camera->field_0x7f8 = 5;
                camera->field_0x808.set(x, y0, x, y1);
                camera->field_0x7f6 = 0;
            }
        }
        break;
    case 8: {
        gGameRoot2_8053AB10->field_0x24->field_0x179 = 1;
        JGeometry::TVec3f pos;
        s32 serial = getActorManager_801F856C()->fn_801F7A08('BARI');
        if (serial >= 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != NULL) {
                pos = actor->mPos;
            }
        }
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
        pos.y += 96.0f;
        if (camera->field_0x840 == 0) {
            camera->field_0x840 = 1;
            camera->field_0x844.set(pos.x, pos.y - 108.0f, pos.x, pos.y + 108.0f);
            camera->field_0x854 = 1.0f;
            camera->field_0x1d4 = -1.0f;
            camera->field_0x1d8 = -1.0f;
        }
        break;
    }
    case 9:
    case 10: {
        gGameRoot2_8053AB10->field_0x24->field_0x179 = 1;
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
        f32 y1 = mPos.y + 96.0f;
        f32 x = mPos.x;
        f32 y0 = mPos.y - 96.0f;
        if (camera->field_0x840 == 0) {
            camera->field_0x840 = 1;
            camera->field_0x844.set(x, y0, x, y1);
            camera->field_0x854 = 1.0f;
            camera->field_0x1d4 = -1.0f;
            camera->field_0x1d8 = -1.0f;
        }
        break;
    }
    case 11: {
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
        if (camera->field_0x7f4 == 0) {
            camera->field_0x7f5 = 1;
        }
        break;
    }
    case 12:
    case 13:
        gGameRoot2_8053AB10->field_0x24->field_0x17d = 1;
        break;
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20: {
        gGameRoot2_8053AB10->field_0x24->field_0x179 = 1;
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
        f32 y1 = mPos.y + 96.0f;
        f32 x = mPos.x;
        f32 y0 = mPos.y - 96.0f;
        if (camera->field_0x840 == 0) {
            camera->field_0x840 = 1;
            camera->field_0x844.set(x, y0, x, y1);
            camera->field_0x854 = 1.0f;
            camera->field_0x1d4 = -1.0f;
            camera->field_0x1d8 = -1.0f;
        }
        break;
    }
    }
}

s32 cls_8049EB8C::vfunc_0x58() {
    return field_0x234;
}
