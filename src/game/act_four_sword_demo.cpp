#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/act_four_sword_demo.h"

// Values of the object's field_0x8 that select fn_80117074's index; vfunc_0x54
// never compares entries 0, 1, 6 and 10.
static const s32 lbl_80464A7C[13] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000096, 0x000000D2, 0x000000F0, 0x000000F0,
    0x000000FF, 0x000001DB, 0x0000032A, 0x00000348, 0x000003C0, 0x000003DE,
};

s32 cls_8049DA0C::vfunc_0x5C() {
    switch (field_0x234) {
    case 12:
        return 1;
    default:
        return 0;
    }
}

s32 cls_8049DA0C::vfunc_0x58() {
    return field_0x234;
}

void cls_8049DA0C::vfunc_0x54() {
    if (fn_8011921C()->field_0x4 == 2) {
        s32 value = fn_8011921C()->field_0x8;
        if (value == lbl_80464A7C[2]) {
            fn_80117074(2);
        } else if (value == lbl_80464A7C[3]) {
            fn_80117074(3);
        } else if (value == lbl_80464A7C[4]) {
            fn_80117074(4);
        } else if (value == lbl_80464A7C[5]) {
            fn_80117074(5);
        } else if (value == lbl_80464A7C[7]) {
            fn_80117074(7);
        } else if (value == lbl_80464A7C[8]) {
            fn_80117074(8);
        } else if (value == lbl_80464A7C[9]) {
            fn_80117074(9);
        } else if (value == lbl_80464A7C[11]) {
            fn_80117074(11);
        } else if (value == lbl_80464A7C[12]) {
            fn_80117074(12);
        }
    }
}

cls_8049DA0C::~cls_8049DA0C() {
    for (s32 i = 0; i < 4; i++) {
        fn_801EE434(mEmitters288[i]);
        mEmitters288[i] = NULL;
        fn_801EE434(mEmitters298[i]);
        mEmitters298[i] = NULL;
        fn_801EE434(mEmitter2a8);
        mEmitter2a8 = NULL;
    }
}
