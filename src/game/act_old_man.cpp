// The old man (actor JIJI, "Old Man" in docs/names/actors.tsv). The class is
// cls_804AEED0 (its vtable, set by the constructor at 0x8037CE00); its
// declaration is in game/act/tu_801F9484.h with the spawner.

#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/cls_8031709C.h"
#include "game/act/tu_801F9484.h"

// Animation step tables (.sdata, in address order).
static u8 lbl_8053A1D0[4] = {7, 10, 7, 10};
static u8 lbl_8053A1D4[4] = {0, 1, 0, 2};
static u8 lbl_8053A1D8[2] = {20, 15};
static u8 lbl_8053A1DC[4] = {20, 13, 16, 13};
static u8 lbl_8053A1E0[4] = {0, 1, 2, 1};
static u8 lbl_8053A1E4[4] = {10, 8, 10, 8};
static u8 lbl_8053A1E8[4] = {0, 1, 0, 2};
static u8 lbl_8053A1EC[4] = {12, 10, 12, 10};
static u8 lbl_8053A1F0[4] = {0, 1, 2, 0};
static u8 lbl_8053A1F4[2] = {15, 20};
static u8 lbl_8053A1F8[4] = {10, 7, 10, 7};
static u8 lbl_8053A1FC[4] = {1, 0, 2, 0};
static u8 lbl_8053A200[4] = {12, 10, 12, 10};
static u8 lbl_8053A204[2][4] = {{0, 1, 0, 1}, {0, 1, 0, 2}};
// 3 bytes in the original (symbol size).
static u8 lbl_8053A20C[3] = {60, 10, 0};

void cls_804AEED0::fn_8037B374(JGeometry::TVec3f pos, s32 arg) {
    mPos = pos;
    field_0x248 = arg / 3;
    field_0x273 = 1;
}

void cls_804AEED0::fn_8037B3A8() {
    if (field_0x234 > 0) {
        return;
    }
    field_0x238++;
    field_0x238 &= 1;
    field_0x234 = lbl_8053A20C[field_0x238];
}

void cls_804AEED0::fn_8037B3E0() {
    if (field_0x248 < 3) {
        if (field_0x234 == 10) {
            field_0x238 = 1;
        } else if (field_0x234 <= 0) {
            field_0x234 = (s32)(50.0f * lbl_8053AAF8->mRandom.get_ufloat_1()) + 40;
            field_0x238 = 0;
        }
    } else {
        field_0x234 = 0;
        field_0x238 = 0;
    }
}

void cls_804AEED0::fn_8037B488() {
    if (field_0x234 == 25) {
        field_0x238 = 1;
    } else if (field_0x234 == 10) {
        field_0x238 = 2;
    } else if (field_0x234 <= 0) {
        field_0x234 = 30;
        field_0x238 = 0;
    }
}

void cls_804AEED0::fn_8037B4D0() {
    if (field_0x234 == 35) {
        field_0x238 = 1;
    } else if (field_0x234 == 20) {
        field_0x238 = 2;
    } else if (field_0x234 == 18) {
        field_0x238 = 0;
    } else if (field_0x234 == 17) {
        field_0x238 = 1;
    } else if (field_0x234 == 2) {
        field_0x238 = 2;
    } else if (field_0x234 <= 0) {
        field_0x238 = 0;
        field_0x234 = 55;
    }
}

void cls_804AEED0::fn_8037B554() {
    if (field_0x234 > 0) {
        return;
    }
    field_0x23c++;
    field_0x23c &= 3;
    field_0x234 = lbl_8053A200[field_0x23c];
    field_0x238 = lbl_8053A204[field_0x248 / 2][field_0x23c];
}

void cls_804AEED0::fn_8037B5B4() {
    if (field_0x234 > 0) {
        return;
    }
    field_0x23c++;
    field_0x23c &= 3;
    field_0x234 = lbl_8053A1F8[field_0x23c];
    field_0x238 = lbl_8053A1FC[field_0x23c];
}

void cls_804AEED0::fn_8037B5FC() {
    if (field_0x234 > 0) {
        return;
    }
    field_0x238++;
    field_0x238 &= 1;
    field_0x234 = lbl_8053A1F4[field_0x238];
}

void cls_804AEED0::fn_8037B634() {
    if (field_0x234 > 0) {
        return;
    }
    field_0x23c++;
    field_0x23c &= 3;
    field_0x234 = lbl_8053A1EC[field_0x23c];
    field_0x238 = lbl_8053A1F0[field_0x23c];
}

void cls_804AEED0::fn_8037B67C() {
    if (field_0x234 > 0) {
        return;
    }
    field_0x23c++;
    field_0x23c &= 3;
    field_0x234 = lbl_8053A1E4[field_0x23c];
    field_0x238 = lbl_8053A1E8[field_0x23c];
}

void cls_804AEED0::fn_8037B6C4() {
    if (field_0x234 > 0) {
        return;
    }
    field_0x23c++;
    field_0x23c &= 3;
    field_0x234 = lbl_8053A1DC[field_0x23c];
    field_0x238 = lbl_8053A1E0[field_0x23c];
}

void cls_804AEED0::fn_8037B70C() {
    if (field_0x234 > 0) {
        return;
    }
    field_0x238++;
    field_0x238 &= 1;
    field_0x234 = lbl_8053A1D8[field_0x238];
}

void cls_804AEED0::fn_8037B744() {
    if (field_0x234 > 0) {
        return;
    }
    field_0x23c++;
    field_0x23c &= 3;
    field_0x234 = lbl_8053A1D0[field_0x23c];
    field_0x238 = lbl_8053A1D4[field_0x23c];
}

void cls_804AEED0::fn_8037B78C() {
    if (field_0x234 > 0) {
        return;
    }
    field_0x234 = field_0x23c;
    field_0x238++;
    field_0x238 &= 7;
}

void cls_804AEED0::fn_8037B7BC() {
    if (field_0x234 > 0) {
        return;
    }
    field_0x234 = field_0x23c;
    field_0x238++;
    field_0x238 &= 1;
}

void cls_804AEED0::fn_8037B7EC() {
    field_0x238 = 0;
}

cls_804AEED0::~cls_804AEED0() {
    if (field_0x274 != NULL) {
        delete field_0x274;
    }
    if (field_0x278 != NULL) {
        releaseEmitter_801EE434(field_0x278);
    }
}

cls_804AEED0::cls_804AEED0() {
    field_0x274 = new cls_8031709C;
}
