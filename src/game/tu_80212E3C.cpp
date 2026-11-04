#include "game/GameAudio.h"
#include "game/act/tu_80212E3C.h"

cls_804A6340::cls_804A6340() {}

cls_804A6340::~cls_804A6340() {}

void cls_804A6340::fn_80212F14() {
    if (mFlags & 0x4000) {
        fn_80213774(0);
    } else if (mState != 1) {
        fn_80213774(1);
    }
}
