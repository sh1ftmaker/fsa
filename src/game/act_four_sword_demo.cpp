#include "game/GameAudio.h"
#include "game/act/act_four_sword_demo.h"

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
