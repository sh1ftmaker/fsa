#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/act/tu_8012FBCC.h"

cls_8049EA00::~cls_8049EA00() {}

bool cls_8049EA00::fn_8012FC2C(const s32* v) {
    s32 serial = fn_801F856C()->fn_801F7A08(ACTOR_OPGL);
    if (serial < 0) {
        return true;
    }
    ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(serial);
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

void cls_8049EA00::init() {
    ActorBase_804A5770::init();
    mFlags = field_0x90 & 0xF;
    fn_801300B4(fn_8012FF9C(), mSerial);
}
