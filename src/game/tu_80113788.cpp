#include "game/GameAudio.h"
#include "game/cls_8049D83C.h"

s32 cls_8049D83C::vfunc_0x58() {
    return field_0x23c;
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
