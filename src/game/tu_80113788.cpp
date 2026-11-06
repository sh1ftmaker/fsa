#include "game/GameAudio.h"
#include "game/cls_8049D83C.h"

static const s32 lbl_80464830[15] = {
    0x199, 0x1E7, 0x17F, 0x201, 0x1CD, 0x1B3, 0x21B, 0x21B,
    0x14C, 0x173, 0x20E, 0x21A, 0x29B, 0x2B1, 0x2CD,
};

void cls_8049D83C::draw() {
    cls_8049D140::draw();
    fn_80113790();
    switch (field_0x23c) {
    case 0x10:
        fn_80113B54(0x1D6);
        break;
    default:
        if (field_0x232 != 0) {
            fn_80113B54(lbl_80464830[field_0x238]);
        }
        break;
    }
}

s32 cls_8049D83C::vfunc_0x5C() {
    switch (field_0x23c) {
    case 0xA:
    case 0x11:
        return 1;
    default:
        return 0;
    }
}

s32 cls_8049D83C::vfunc_0x58() {
    return field_0x23c;
}
