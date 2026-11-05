#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/act/act_ice_floor_check_npc.h"
#include "game/tu_8012F3E4.h"

bool cls_8012F64C::fn_8012F3E4() const {
    if (mCount <= 0) {
        return false;
    }
    s32 count = mCount;
    for (s32 i = 0; i < count; i++) {
        ActorBase_804A5770* actor = fn_801F856C()->findActorBySerial_801F666C(mSerials[i]);
        if (actor == NULL) {
            return false;
        }
        if (actor->mActorId != ACTOR_IFCK) {
            return false;
        }
        if (!static_cast<cls_8049E950*>(actor)->fn_8012F008()) {
            return false;
        }
    }
    return true;
}

void cls_8012F64C::fn_8012F494() {
    mCount = 0;
    for (int i = 0; i < 64; i++) {
        mSerials[i] = -1;
    }
}

void cls_8012F64C::fn_8012F530(const ActorBase_804A5770* actor) {
    mSerials[mCount] = actor->mSerial;
    mCount++;
}

cls_8012F64C* fn_8012F554() {
    static cls_8012F64C lbl_804BDDFC;
    return &lbl_804BDDFC;
}
