#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/act/tu_8012FBCC.h"
#include "game/tu_8012FF9C.h"

cls_8012FFF4* fn_8012FF9C() {
    static cls_8012FFF4 lbl_8053AAC4;
    return &lbl_8053AAC4;
}

bool cls_8012FFF4::fn_80130030(const s32* v) {
    if (mCount <= 0) {
        return true;
    }
    ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(mSerials[0]);
    if (actor == NULL) {
        return true;
    }
    if (actor->mActorId != ACTOR_OGST) {
        return true;
    }
    return static_cast<cls_8049EA00*>(actor)->fn_8012FC2C(v);
}

void cls_8012FFF4::fn_801300B4(s32 value) {
    if (value > 0) {
        mSerials[mCount] = value;
        mCount++;
    }
}

void cls_8012FFF4::fn_801300D8() {
    mCount = 0;
    mSerials[0] = -1;
}
