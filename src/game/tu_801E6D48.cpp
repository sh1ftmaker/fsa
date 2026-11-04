#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/tu_801E6D48.h"

cls_804A5418::cls_804A5418() {
    field_0x2ac = -1;
    field_0x2b0 = 0;
    field_0x2b4 = 0;
}

void cls_804A5418::update() {
    if ((mFlags & 0x8000) == 0) {
        EnemyBase_804A4808::update();
    }
}

void cls_804A5418::vfunc_0x10() {
    u32 flags = field_0x90 >> 24;
    s32 serial = mSerial;
    cls_801E9AFC* slots = fn_801E9AFC();
    field_0x2ac = slots->field_0x0[slots->field_0xf0].fn_801EA6D8(flags, serial);
}

void cls_804A5418::draw() {
    if ((mFlags & 0x8000) == 0) {
        s32 arg = 0;
        if (mState == 2) {
            arg = (s32)field_0x2a8;
        }
        fn_80210630((mFlags >> 13) & 1, 0, 0, arg);
    }
}
