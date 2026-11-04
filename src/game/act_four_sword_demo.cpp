#include "game/GameAudio.h"
#include "game/act/act_four_sword_demo.h"

s32 cls_8049DA0C::vfunc_0x58() {
    return field_0x234;
}

s32 cls_8049DA0C::vfunc_0x5C() {
    switch (field_0x234) {
    case 12:
        return 1;
    default:
        return 0;
    }
}
