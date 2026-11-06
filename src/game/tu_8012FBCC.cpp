#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/act/tu_8012FBCC.h"
#include "game/tu_8012FF9C.h"

void cls_8049EA00::init() {
    ActorBase_804A5770::init();
    mFlags = field_0x90 & 0xF;
    fn_8012FF9C()->fn_801300B4(mSerial);
}

bool cls_8049EA00::fn_8012FC2C(const s32* v) {
    s32 serial = getActorManager_801F856C()->fn_801F7A08(ACTOR_OPGL);
    if (serial < 0) {
        return true;
    }
    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
    if (actor == NULL) {
        return true;
    }
    if (actor->mActorId != ACTOR_OPGL) {
        return true;
    }
    if ((mFlags & 1) && v[0] <= 0) {
        return true;
    }
    if (((mFlags >> 1) & 1) && v[0] >= 0) {
        return true;
    }
    if (((mFlags >> 2) & 1) && v[1] <= 0) {
        return true;
    }
    if (((mFlags >> 3) & 1) && v[1] >= 0) {
        return true;
    }
    return false;
}

cls_8049EA00::~cls_8049EA00() {}
