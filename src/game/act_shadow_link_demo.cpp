#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/act_shadow_link_demo.h"

// Values that vfunc_0x54 compares the object's field_0x8 with, by the index it
// passes to fn_8011B878. Entries 0, 5, 13, 21-26, 30 and 34 are never compared.
static const s32 lbl_80464AF8[35] = {
    0x00000000, 0x0000083E, 0x0000087A, 0x000008DE, 0x00000901, 0x00000956, 0x00000A8C,
    0x00000AF0, 0x00000E1A, 0x00000E56, 0x00000E92, 0x00000ECE, 0x00000F0A, 0x00000000,
    0x0000005A, 0x000000D2, 0x0000014A, 0x00000384, 0x00000479, 0x000004D3, 0x0000050F,
    0x00000573, 0x00000573, 0x00000573, 0x00000573, 0x00000000, 0x00000000, 0x00000000,
    0x00000226, 0x00000280, 0x00000280, 0x000002F8, 0x00000320, 0x00000384, 0x00000000,
};

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

cls_8049DB90::~cls_8049DB90() {}

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
