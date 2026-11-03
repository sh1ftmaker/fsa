#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/act/tu_80207548.h"
#include "game/act/tu_80209C14.h"

cls_804A5D60::~cls_804A5D60() {}

void cls_804A5D60::vfunc_0x24() {
    ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(mTargetSerial);
    if (actor != NULL) {
        static_cast<cls_804A5B28*>(actor)->fn_802075D8(field_0x94);
    }
    ActorBase_804A5770::vfunc_0x24();
}

void cls_804A5D60::fn_80209D44() {
    mFlags |= 0x2000;
}

void cls_804A5D60::vfunc_0x54() {
    JGeometry::TVec2f offset(mSpawnPos.x - mPos.x, mSpawnPos.y - mPos.y);
    field_0x2c0 = offset;
}

bool cls_804A5D60::vfunc_0x60() {
    fn_801F2DB4(mActorId, 'NULL', NULL);
    return false;
}

void cls_804A5D60::vfunc_0x74() {
    mFlags |= ENEMY_FLAG_NO_HURT;
}
