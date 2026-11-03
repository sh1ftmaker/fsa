#include "game/GameAudio.h"
#include "game/cls_8049D740.h"

#include "game/ActorId.h"
#include "game/ActorManager.h"

void cls_8049D740::vfunc_0x10() {
    s32 serial = fn_801F856C()->fn_801F7A08(ACTOR_DMGL);
    ActorBase_804A5770* goal = fn_801F856C()->fn_801F666C(serial);
    if (goal != NULL) {
        mGoalPos = goal->mPos;
    }
}

s32 cls_8049D740::vfunc_0x58() {
    return field_0x238;
}
