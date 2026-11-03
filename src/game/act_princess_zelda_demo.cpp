#include "game/GameAudio.h"
#include "game/act/act_princess_zelda_demo.h"

s32 PrincessZeldaDemo_8049D208::vfunc_0x58() {
    return field_0x238;
}

void PrincessZeldaDemo_8049D208::fn_8010F9F0() {
    switch (field_0x234) {
    case 0:
    case 1:
        fn_8010E7E8(0x31);
        break;
    case 2:
        fn_8010E2D4(0x31);
        break;
    case 3: // nothing sets this state
        break;
    }
    field_0x238 = 0x31;
}

void PrincessZeldaDemo_8049D208::fn_80111CC0() {
    cls_8049D140::update();
    switch (field_0x234) {
    case 0:
    case 1:
        fn_80110E9C();
        break;
    case 2:
        fn_8011093C();
        break;
    case 3: // nothing sets this state
        break;
    }
    mAnim0.fn_801D1C18();
    mAnim1.fn_801D1C18();
    if (field_0x230 != 0) {
        fn_8010F4DC();
    }
}

void PrincessZeldaDemo_8049D208::fn_80110198() {
    cls_8049D140::draw();
    if (field_0x230 == 0) {
        if (mAnim0.mAnim >= 0x13) {
            fn_8010F7EC(0x21b);
        } else if (mAnim0.mAnim >= 0xf) {
            fn_8010F7EC(0x274);
        } else {
            fn_8010F7EC(0x9b);
        }
    }
}
