#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/act_great_fairy_spfr.h"

// lbl_80465204 (.rodata): values fn_8011921C()->field_0x8 is matched against, by index.
static const s32 lbl_80465204[27] = {
    0, 0, 0, 0, 0, 0x28, 0x64, 0xC8, 0x104, 0x208, 0x26C, 0x32A, 0x366, 0x55A,
    0x578, 0x5BE, 0x5F0, 0x663, 0x6C7, 0x703, 0x767, 0x7CB, 0xC8, 0x12C, 0x190, 0x1F4, 0,
};

s32 cls_8049EB8C::vfunc_0x58() {
    return field_0x234;
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

cls_8049EB8C::~cls_8049EB8C() {
    fn_801EE434(field_0x288);
    field_0x288 = 0;
}

cls_8049EB8C::cls_8049EB8C()
    : field_0x230(0), field_0x234(0), field_0x238(0), field_0x23c(-1), field_0x240(0),
      field_0x244(0), field_0x248(0), field_0x24c(0), field_0x288(0) {}
