#include "game/GameAudio.h"

// FIXME: .sdata2 82.93%. The original's section is 24 bytes (the last table
// symbol is 12 bytes long); ours is 17, and the cause of the missing tail is
// unknown. Order matches the original (lowest address first).
static const u8 lbl_8053FCE0[4] = {0x0C, 0x0D, 0x0E, 0x0F};
static const u8 lbl_8053FCE4[5] = {3, 4, 5, 3, 0x1D};
static const u8 lbl_8053FCEC[5] = {5, 5, 7, 7, 0x3C};

class cls_8021BFA8 {
public:
    int fn_8021BFA8();
    void fn_8021C00C();

    /* 0x0 */ int field_0x0;
    /* 0x4 */ u32 field_0x4;
    /* 0x8 */ int field_0x8;
};

class cls_8021C024 {
public:
    int fn_8021C024();
    void fn_8021C078();

    /* 0x0 */ int field_0x0;
    /* 0x4 */ u32 field_0x4;
    /* 0x8 */ int field_0x8;
};

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

void cls_8021BFA8::fn_8021C00C() {
    field_0x0 = 3;
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

void cls_8021C024::fn_8021C078() {
    field_0x0 = 12;
    field_0x4 = 0;
    field_0x8 = 0;
}
