#include "game/GameAudio.h"
#include "game/act/act_zora_escort.h"
#include "game/tu_80131D98.h"

// The escort's animation keys: (cel, frames) s16 pairs, ended by a cel of -1.
static const s16 lbl_80464C58[10] = {2, 6, 1, 4, 3, 6, 1, 4, -1, -1};
static const s16 lbl_80464C6C[10] = {6, 6, 4, 4, 5, 6, 4, 4, -1, -1};
static const s16 lbl_80464C80[10] = {8, 6, 7, 4, 9, 6, 7, 4, -1, -1};
static const s16 lbl_80464C94[10] = {11, 6, 10, 4, 12, 6, 10, 4, -1, -1};
static const s16 lbl_80464CA8[12] = {1, 20, 4, 20, 1, 20, 4, 20, 0, 3, -1, -1};
static const s16 lbl_8053C930[2] = {-1, -1};
static const s16 lbl_8053C934[4] = {1, 10, -1, -1};
static const s16 lbl_8053C93C[4] = {4, 10, -1, -1};
static const s16 lbl_8053C944[4] = {0, 10, -1, -1};
static const s16 lbl_8053C94C[4] = {10, 10, -1, -1};
static const s16 lbl_8053C954[2] = {-1, -1};
static const s16 lbl_8053C958[2] = {-1, -1};
static const s16 lbl_8053C95C[2] = {-1, -1};
static const s16 lbl_8053C960[2] = {-1, -1};

// The key tables of mAnim, in address order.
static const s16* lbl_8049DF40[14] = {
    lbl_80464C58, lbl_80464C6C, lbl_80464C80, lbl_80464C94, lbl_8053C930, lbl_8053C934, lbl_8053C93C,
    lbl_8053C944, lbl_8053C94C, lbl_80464CA8, lbl_8053C954, lbl_8053C958, lbl_8053C95C, lbl_8053C960,
};

u8 ZoraEscort_8049DF78::vfunc_0x60() {
    return 0;
}

s32 ZoraEscort_8049DF78::vfunc_0x78() const {
    return 0;
}

void ZoraEscort_8049DF78::vfunc_0x90() {}

void ZoraEscort_8049DF78::vfunc_0x94() {}

bool ZoraEscort_8049DF78::vfunc_0xC4() {
    if (fn_80131F0C()->mCount < 2) {
        return false;
    }
    ActorBase_804A5770* other = fn_80131F0C()->fn_80131E30(1);
    if (other == NULL) {
        return false;
    }
    return mSerial == other->mSerial;
}

s32 ZoraEscort_8049DF78::vfunc_0xC0() {
    return 1;
}

void ZoraEscort_8049DF78::vfunc_0x9C(s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6) {
    EscortBase_8049E0C8::vfunc_0x9C(arg1, arg2, arg3, arg4, arg5, arg6);
}

void ZoraEscort_8049DF78::vfunc_0x8C() {
    vfunc_0x6C(0xC);
}

s32 ZoraEscort_8049DF78::vfunc_0x88() {
    return vfunc_0x6C(9);
}

s32 ZoraEscort_8049DF78::vfunc_0x84() {
    return vfunc_0x6C(0xB);
}

void ZoraEscort_8049DF78::vfunc_0x74() {
    fn_80121DB0();
}

void ZoraEscort_8049DF78::vfunc_0x70() {
    mAnim.fn_801D2608(lbl_8049DF40, 14);
    mAnim.startAnim_801D22DC(7, 2, false);
    mAnim.fn_801D1F14(0.0f);
}

void ZoraEscort_8049DF78::vfunc_0x64() {
    if (field_0x90 & 1) {
        field_0x3c4 = 1;
    }
    field_0x35c = (field_0x90 >> 8) & 0xFF;
    field_0x3d0 = (field_0x90 >> 16) & 0xFF;
}

s32 ZoraEscort_8049DF78::vfunc_0x5C() {
    if (field_0x33c == 0 || field_0x33c == 0xF) {
        return 3;
    }
    bool flag = vfunc_0x58();
    s32 result = 1;
    if (flag) {
        result = 2;
    }
    return result;
}

bool ZoraEscort_8049DF78::vfunc_0x58() {
    switch (field_0x33c) {
    case 7:
    case 11:
    case 12:
        return fn_80121DC8(fn_8012285C()) <= 32.0f;
    case 13: // nothing sets 13 or 14 here
    case 14:
        break;
    }
    return false;
}

bool ZoraEscort_8049DF78::vfunc_0x54() {
    switch (field_0x33c) {
    case 7:
    case 9:
    case 10:
    case 11:
    case 12:
        return true;
    }
    return false;
}

void ZoraEscort_8049DF78::update() {
    if (field_0x33c != 0xF) {
        EscortBase_8049E0C8::update();
    }
}

void ZoraEscort_8049DF78::init() {
    fn_80124448();
    if ((field_0x90 >> 2) & 1) {
        field_0x344 = (field_0x90 >> 3) & 0xF;
        fn_80131F0C()->fn_80131EC0(this);
        vfunc_0x6C(7);
    }
}

ZoraEscort_8049DF78::~ZoraEscort_8049DF78() {}

ZoraEscort_8049DF78::ZoraEscort_8049DF78() : EscortBase_8049E0C8("enemy_both2_sob.bin", 0x242, 0) {
    field_0x3c4 = 0;
    field_0x3c8 = 0;
    field_0x3cc = -1;
    field_0x3d0 = 0;
}
