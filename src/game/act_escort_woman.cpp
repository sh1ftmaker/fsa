#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/act/act_escort_woman.h"

// The escort's animation keys: (cel, frames) s16 pairs, ended by a cel of -1.
static const s16 lbl_80464D98[10] = {8, 10, 9, 7, 10, 10, 9, 7, -1, -1};
static const s16 lbl_80464DAC[10] = {11, 10, 12, 7, 13, 10, 12, 7, -1, -1};
static const s16 lbl_80464DC0[10] = {14, 10, 15, 7, 16, 10, 15, 7, -1, -1};
static const s16 lbl_80464DD4[10] = {17, 10, 18, 7, 19, 10, 18, 7, -1, -1};
static const s16 lbl_80464DE8[22] = {2, 8, 4, 8, 0, 8, 6, 8, 2, 8, 4, 8, 0, 8, 6, 8, 2, 5, 20, 20, -1, -1};
static const s16 lbl_80464E14[6] = {0, 20, 1, 15, -1, -1};
static const s16 lbl_80464E20[6] = {2, 20, 3, 15, -1, -1};
static const s16 lbl_80464E2C[6] = {4, 20, 5, 15, -1, -1};
static const s16 lbl_80464E38[6] = {6, 20, 7, 15, -1, -1};
static const s16 lbl_80464E44[12] = {0, 20, 2, 20, 0, 20, 2, 20, 4, 3, -1, -1};
static const s16 lbl_80464E5C[6] = {0, 10, 1, 15, -1, -1};
static const s16 lbl_80464E68[6] = {2, 10, 3, 15, -1, -1};
static const s16 lbl_80464E74[6] = {4, 10, 5, 15, -1, -1};
static const s16 lbl_80464E80[6] = {6, 10, 7, 15, -1, -1};

// The key tables of mAnim, in address order.
static const s16* lbl_8049E198[14] = {
    lbl_80464D98, lbl_80464DAC, lbl_80464DC0, lbl_80464DD4, lbl_80464DE8, lbl_80464E14, lbl_80464E20,
    lbl_80464E2C, lbl_80464E38, lbl_80464E44, lbl_80464E5C, lbl_80464E68, lbl_80464E74, lbl_80464E80,
};

EscortWoman_8049E1D0::EscortWoman_8049E1D0() : EscortBase_8049E0C8("enemy_gc_sob.bin", 0x2E6, 1) {
    field_0x3c4 = 0;
    field_0x3c8 = 0;
    field_0x3cc = 0;
}

EscortWoman_8049E1D0::~EscortWoman_8049E1D0() {}

void EscortWoman_8049E1D0::init() {
    fn_80124448();
}

bool EscortWoman_8049E1D0::vfunc_0x54() {
    switch (field_0x33c) {
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        return true;
    default:
        return false;
    }
}

bool EscortWoman_8049E1D0::vfunc_0x58() {
    switch (field_0x33c) {
    case 4:
    case 7:
    case 8:
        return fn_80121DC8(fn_8012285C()) <= 32.0f;
    // Empty case: the original's dispatch has an unreachable extra branch for states 15 and 16.
    case 15:
    case 16:
        break;
    }
    return false;
}

u8 EscortWoman_8049E1D0::vfunc_0x60() {
    switch (field_0x33c) {
    case 11:
        return 1;
    default:
        return 0;
    }
}

void EscortWoman_8049E1D0::vfunc_0x64() {
    field_0x35c = field_0x90 & 0xFF;
    field_0x3cc = (field_0x90 >> 22) & 0x1F;
}

void EscortWoman_8049E1D0::vfunc_0x68() {
    JGeometry::TBox2f box(mBox);
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;

    if (field_0x33c >= 13) {
        cls_804A03A8* obj = gGameRoot2_8053AB10->field_0x30;
        if (!obj->field_0x284) {
            obj->field_0x284 = 1;
            obj->field_0x28c = 0.05f;
        }
    }

    switch (field_0x33c) {
    case 0: {
        vfunc_0x98(0, 1, 64.0f);
        s32 res = fn_80122898(0x84D);
        if (res >= 0) {
            field_0x344 = res;
            vfunc_0x6C(1);
        }
        break;
    }

    case 1:
        fn_8022FF8C(field_0x34c, &box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x34c, mSerial)) {
            vfunc_0x6C(4);
        }
        break;

    case 3: {
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
        camera->addFocusPoint_8030C210(&mPos);
        camera = gGameRoot2_8053AB10->field_0x24;
        if (camera->field_0x840 == 0) {
            camera->field_0x840 = 1;
            // The original reads mPos.x and mPos.y once for both corners; direct reads reload them.
            f32 posX = mPos.x;
            camera->field_0x844.f.x = posX;
            camera->field_0x844.i.x = posX;
            f32 posY = mPos.y;
            camera->field_0x844.f.y = posY;
            camera->field_0x844.i.y = posY;
            camera->field_0x854 = 1.0f;
            camera->field_0x1d4 = -1.0f;
            camera->field_0x1d8 = -1.0f;
        }
        fn_8022FE80(fn_8022EA0C(fn_8012285C()), field_0x4, mSerial, false, false);
        if (fn_80388CAC(fn_8022EA0C(fn_8012285C()), mSerial)) {
            vfunc_0x6C(field_0x3d0);
        }
        break;
    }

    case 4:
        if (fn_8012285C() < 0) {
            vfunc_0x6C(0);
            break;
        }
        if (vfunc_0x34()) {
            vfunc_0x6C(14);
            break;
        }
        if (gGameRoot_8053AAF8->fn_80138A30() > 1) {
            vfunc_0x9C(fn_8012285C(), 0.03f, 0.08f, 1.0f, 0.2f, 16.0f);
        } else {
            vfunc_0x9C(fn_8012285C(), 0.03f, 0.08f, 1.0f, 0.2f, 8.0f);
        }
        {
            s32 serial = fn_801F2718(0x48415254);
            if (serial >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (actor != NULL) {
                    actor->mAlive = false;
                    mLife += 4;
                    if (mLife >= field_0x360 * 4) {
                        mLife = field_0x360 * 4;
                    }
                    field_0x335 = 1;
                }
            }
        }
        break;

    case 5:
        if (fn_8012285C() < 0) {
            vfunc_0x6C(0);
        } else if (!fn_8022D75C(fn_8012285C()) && field_0x4 == getPlayerLayer_8023DE58(fn_8012285C())) {
            vfunc_0x6C(6);
        }
        break;

    case 6:
        if (fn_8012285C() < 0) {
            vfunc_0x6C(0);
        }
        if (fn_80122804()) {
            vfunc_0x6C(4);
        }
        break;

    case 7:
        fn_80122548();
        if (!getGameView_802D6A9C()->fn_802DCD0C()) {
            vfunc_0x6C(4);
        }
        break;

    case 8:
        field_0x4 = getPlayerLayer_8023DE58(fn_8012285C());
        fn_80122148();
        if (!fn_8022D75C(fn_8012285C()) && fn_801216D0(fn_8012285C()) < 0) {
            vfunc_0x6C(4);
        }
        break;

    case 9:
        if (fn_80121E40() && mAnim.fn_801D208C()) {
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
            field_0x3c8++;
            if (field_0x3c8 >= 30) {
                field_0x3c8 = 0;
                vfunc_0x6C(field_0x340);
            }
        }
        break;

    case 10:
        if (getActorManager_801F856C()->fn_801F68E0() && mAnim.fn_801D208C() && fn_801227D4()) {
            vfunc_0x6C(11);
        }
        break;

    case 11:
        mAnim.fn_801D1C18();
        if (lbl_8053AAA0) {
            vfunc_0x6C(12);
        }
        break;

    case 12:
        if (fn_8012285C() < 0) {
            vfunc_0x6C(0);
        } else {
            vfunc_0x6C(4);
        }
        break;

    case 14:
        if (getActorManager_801F856C()->fn_801F68E0()) {
            vfunc_0x6C(15);
        }
        break;

    case 15:
        fn_8022FE80(0, field_0x4, mSerial, false, false);
        field_0x3c4++;
        if (field_0x3c4 >= 90) {
            s32 layer = field_0x4;
            if (fn_8038BD08(0, 0x4BA, layer, mPos, 0x22)) {
                field_0x3c4 = 0;
                vfunc_0x6C(16);
            }
        }
        break;

    case 16:
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_803865B8(0)) {
            fn_80388EEC(0);
            vfunc_0x6C(13);
        }
        break;

    case 13: {
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
        if (camera->field_0x7f5 == 0) {
            camera->field_0x7f4 = 1;
            camera->field_0x7f8 = 4;
            camera->field_0x7fc.x = mPos.x;
            camera->field_0x7fc.y = mPos.y;
            camera->field_0x7fc.z = mPos.z;
            camera->field_0x7f6 = 0;
        }
        field_0x380 = 0.7f;
        if (vfunc_0xC8(JGeometry::TVec3f(0.0f, 8.0f, 0.0f), field_0x380)) {
            vfunc_0x6C(17);
        }
        break;
    }
    case 17: // empty case: the original's dispatch has an extra unreachable branch
        break;
    }
}

s32 EscortWoman_8049E1D0::vfunc_0x6C(s32 state) {
    field_0x330 = 0;
    field_0x335 = 0;
    field_0x378 = 0;
    switch (state) {
    case 0:
        field_0x330 = 1;
        mAnim.startAnim_801D22DC(7, 2, false);
        break;
    case 4:
        field_0x11e = 1;
        fn_801F3604();
        break;
    case 5:
        fn_8012202C();
        break;
    case 6:
        fn_80121FA8();
        mAnim.startAnim_801D22DC(2, 2, false);
        break;
    case 7:
        mAnim.startAnim_801D22DC(fn_80230CFC(fn_8012285C()), 2, false);
        field_0x333 = 0;
        field_0x354 = 0;
        break;
    case 8:
        mAnim.startAnim_801D22DC(fn_80230CFC(fn_8012285C()), 2, false);
        break;
    case 9:
        mAnim.startAnim_801D22DC(field_0x98 + 10, 0, false);
        field_0x333 = 1;
        startSound_801F0E34(0x40071, 0);
        field_0x3c8 = 0;
        fn_80121F34();
        break;
    case 10:
        mAnim.startAnim_801D22DC(4, 0, false);
        startSound_801F0E34(0x40072, 0);
        break;
    case 11:
        getActorManager_801F856C()->fn_801F6884();
        fn_80121764();
        break;
    case 12:
        mAnim.startAnim_801D22DC(2, 2, false);
        fn_80121764();
        fn_80121ABC();
        field_0x333 = 1;
        break;
    case 14:
        fn_80121D68();
        fn_801220C4();
        break;
    case 13:
        field_0x331 = 0;
        break;
    case 17:
        getGameView_802D6A9C()->fn_802D8050(field_0x3cc, 1, 0xFF);
        mAlive = false;
        break;
    }
    return EscortBase_8049E0C8::vfunc_0x6C(state);
}

void EscortWoman_8049E1D0::vfunc_0x70() {
    mAnim.fn_801D2608(lbl_8049E198, 14);
    mAnim.startAnim_801D22DC(7, 2, false);
    mAnim.fn_801D1F14(0.0f);
}

void EscortWoman_8049E1D0::vfunc_0x74() {
    fn_80121DB0();
}

s32 EscortWoman_8049E1D0::vfunc_0x78() const {
    if (field_0x33c >= 6 || field_0x33c <= 3) {
        return 0;
    }
    return EscortBase_8049E0C8::vfunc_0x78();
}

s32 EscortWoman_8049E1D0::vfunc_0x84() {
    return vfunc_0x6C(7);
}

s32 EscortWoman_8049E1D0::vfunc_0x88() {
    return vfunc_0x6C(5);
}

void EscortWoman_8049E1D0::vfunc_0x8C() {
    vfunc_0x6C(8);
}

void EscortWoman_8049E1D0::vfunc_0x90() {
    if (fn_80122834() && field_0x33c < 10) {
        vfunc_0x6C(10);
    }
}

void EscortWoman_8049E1D0::vfunc_0x94() {
    vfunc_0x6C(9);
}
