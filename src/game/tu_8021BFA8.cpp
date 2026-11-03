#include "game/GameAudio.h"

// FIXME: .sdata2 82.93%. Our section is 0x11 bytes with 8-byte alignment; the
// target's 0x18 is 7 zero bytes of alignment padding up to the next unit's
// 8-aligned .sdata2 at 0x8053FCF8 (the linked DOL is identical). The tables
// and their order match.
static const u8 lbl_8053FCE0[4] = {0x0C, 0x0D, 0x0E, 0x0F};
static const u8 lbl_8053FCE4[5] = {3, 4, 5, 3, 0x1D};
static const u8 lbl_8053FCEC[5] = {5, 5, 7, 7, 0x3C};

// Named after its first method (fn_8021BFA8). Size 0xC.
class cls_8021BFA8 {
public:
    int fn_8021BFA8();
    void fn_8021C00C();

    /* 0x0 */ int field_0x0;
    /* 0x4 */ u32 field_0x4;
    /* 0x8 */ int field_0x8;
};

// Named after its first method (fn_8021C024). Size 0xC.
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
