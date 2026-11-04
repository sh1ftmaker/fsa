#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/act/act_quicksand.h"
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

void cls_804A6340::vfunc_0x24() {
    if ((s32)field_0x90 >= 0) {
        ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C((s32)field_0x90);
        if (actor != NULL) {
            ((cls_804A6270*)actor)->fn_80210D90();
        }
    }
    ActorBase_804A5770::vfunc_0x24();
}

void cls_804A6340::vfunc_0x44() {
    if (field_0xd4 == 1 && mState != 0 && !(mFlags & 0x4000)) {
        fn_80213774(4);
    }
}
