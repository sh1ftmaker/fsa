#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/act/act_escort_woman.h"
#include "game/tu_80131D98.h"

bool fn_80131D98() {
    s32 serial = getActorManager_801F856C()->fn_801F7A08(ACTOR_OPGL);
    if (serial < 0) {
        return false;
    }
    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
    if (actor == NULL) {
        return false;
    }
    if (actor->mActorId != ACTOR_OPGL) {
        return false;
    }
    return static_cast<EscortWoman_8049E1D0*>(actor)->vfunc_0x60();
}

ActorBase_804A5770* cls_80131F6C::fn_80131E30(s32 idx) {
    if (idx < 0 || idx >= mCount) {
        return NULL;
    }
    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(mSerials[idx]);
    if (actor == NULL) {
        return NULL;
    }
    if (actor->mActorId != ACTOR_FAZR && actor->mActorId != ACTOR_OPGL) {
        return NULL;
    }
    return actor;
}

void cls_80131F6C::fn_80131EC0(const ActorBase_804A5770* actor) {
    mSerials[mCount] = actor->mSerial;
    mCount++;
}

void cls_80131F6C::fn_80131EE4() {
    mCount = 0;
    mSerials[0] = 0;
    mSerials[1] = 0;
}

void cls_80131F6C::fn_80131EF8() {
    mCount = 0;
    mSerials[0] = 0;
    mSerials[1] = 0;
}

cls_80131F6C* fn_80131F0C() {
    static cls_80131F6C lbl_804BE514;
    return &lbl_804BE514;
}
