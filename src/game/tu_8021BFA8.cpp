#include "game/GameAudio.h"
#include "game/cls_8021BFA8.h"
#include "game/cls_8021C024.h"

static const u8 lbl_8053FCE0[4] = {0x0C, 0x0D, 0x0E, 0x0F};
static const u8 lbl_8053FCE4[5] = {3, 4, 5, 3, 0x1D};
static const u8 lbl_8053FCEC[5] = {5, 5, 7, 7, 0x3C};

void cls_8021C024::fn_8021C078() {
    field_0x0 = 12;
    field_0x4 = 0;
    field_0x8 = 0;
}

int cls_8021C024::fn_8021C024() {
    field_0x8++;
    if (field_0x8 >= 7) {
        field_0x8 = 0;
        field_0x4++;
        if (field_0x4 >= 4) {
            field_0x4 = 0;
        }
    }
    field_0x0 = lbl_8053FCE0[field_0x4];
    return field_0x0;
}

void cls_8021BFA8::fn_8021C00C() {
    field_0x0 = 3;
    field_0x4 = 0;
    field_0x8 = 0;
}

int cls_8021BFA8::fn_8021BFA8() {
    field_0x8++;
    if (field_0x8 >= lbl_8053FCEC[field_0x4]) {
        field_0x8 = 0;
        field_0x4++;
        if (field_0x4 >= 5) {
            field_0x4--;
        }
    }
    field_0x0 = lbl_8053FCE4[field_0x4];
    return field_0x0;
}
