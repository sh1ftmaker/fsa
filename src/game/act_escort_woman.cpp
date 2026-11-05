#include "game/GameAudio.h"
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

void EscortWoman_8049E1D0::vfunc_0x94() {
    vfunc_0x6C(9);
}

void EscortWoman_8049E1D0::vfunc_0x90() {
    if (fn_80122834() && field_0x33c < 10) {
        vfunc_0x6C(10);
    }
}

void EscortWoman_8049E1D0::vfunc_0x8C() {
    vfunc_0x6C(8);
}

s32 EscortWoman_8049E1D0::vfunc_0x88() {
    return vfunc_0x6C(5);
}

s32 EscortWoman_8049E1D0::vfunc_0x84() {
    return vfunc_0x6C(7);
}

s32 EscortWoman_8049E1D0::vfunc_0x78() const {
    if (field_0x33c >= 6 || field_0x33c <= 3) {
        return 0;
    }
    return EscortBase_8049E0C8::vfunc_0x78();
}

void EscortWoman_8049E1D0::vfunc_0x74() {
    fn_80121DB0();
}

void EscortWoman_8049E1D0::vfunc_0x70() {
    mAnim.fn_801D2608(lbl_8049E198, 14);
    mAnim.fn_801D22DC(7, 2, false);
    mAnim.fn_801D1F14(0.0f);
}

void EscortWoman_8049E1D0::vfunc_0x64() {
    field_0x35c = field_0x90 & 0xFF;
    field_0x3cc = (field_0x90 >> 22) & 0x1F;
}

void EscortWoman_8049E1D0::init() {
    fn_80124448();
}

EscortWoman_8049E1D0::~EscortWoman_8049E1D0() {}

EscortWoman_8049E1D0::EscortWoman_8049E1D0() : EscortBase_8049E0C8("enemy_gc_sob.bin", 0x2E6, 1) {
    field_0x3c4 = 0;
    field_0x3c8 = 0;
    field_0x3cc = 0;
}
