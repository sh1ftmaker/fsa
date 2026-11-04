#include "game/GameAudio.h"
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

void cls_804A5418::draw() {
    if ((mFlags & 0x8000) == 0) {
        s32 arg = 0;
        if (mState == 2) {
            arg = (s32)field_0x2a8;
        }
        fn_80210630((mFlags >> 13) & 1, 0, 0, arg);
    }
}
