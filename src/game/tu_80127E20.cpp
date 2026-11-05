#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Player.h"
#include "game/act/act_zora_escort_goal.h"
#include "game/tu_80127E20.h"

void cls_80128460::fn_80127E20(const JGeometry::TVec3f* pos) {
    for (s32 i = 0; i < mCount; i++) {
        ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(mSerials[i]);
        if (actor != NULL && actor->mActorId == ACTOR_FZRA &&
            static_cast<ZoraEscortGoal_8049E424*>(actor)->field_0x258 == 0) {
            actor->mPos = *pos;
        }
    }
}

void cls_80128460::fn_80127ED8(const JGeometry::TVec3f* pos) {
    for (s32 i = 0; i < mCount; i++) {
        ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(mSerials[i]);
        if (actor != NULL && actor->mActorId == ACTOR_FZRA &&
            static_cast<ZoraEscortGoal_8049E424*>(actor)->field_0x258 != 0) {
            actor->mPos = *pos;
        }
    }
}

const JGeometry::TVec3f* cls_80128460::fn_80127F90() {
    if (mCount <= 1) {
        JGeometry::TVec3f zero(0.0f, 0.0f, 0.0f);
        return &zero;
    }
    JGeometry::TVec3f pos[2];
    for (s32 i = 0; i < 2; i++) {
        ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(mSerials[i]);
        if (actor != NULL && actor->mActorId == ACTOR_FZRA) {
            pos[i] = actor->mPos;
        }
    }
    JGeometry::TVec3f center((pos[0].x + pos[1].x) / 2, (pos[0].y + pos[1].y) / 2, 0.0f);
    return &center;
}

void cls_80128460::fn_80128074() {
    for (s32 i = 0; i < mCount; i++) {
        ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(mSerials[i]);
        if (actor != NULL && actor->mActorId == ACTOR_FZRA) {
            static_cast<ZoraEscortGoal_8049E424*>(actor)->fn_80126A4C(0x10);
        }
    }
}

bool cls_80128460::fn_801280F8() {
    for (s32 i = 0; i < mCount; i++) {
        ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(mSerials[i]);
        if (actor != NULL && actor->mActorId == ACTOR_FZRA) {
            switch (static_cast<ZoraEscortGoal_8049E424*>(actor)->field_0x230) {
            case 2:
            case 3:
            case 9:
                return true;
            }
        }
    }
    return false;
}

void cls_80128460::fn_801281A4() {
    for (s32 i = 0; i < mCount; i++) {
        ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(mSerials[i]);
        if (actor != NULL && actor->mActorId == ACTOR_FZRA) {
            static_cast<ZoraEscortGoal_8049E424*>(actor)->fn_80126A4C(0xA);
        }
    }
}

void cls_80128460::fn_80128228() {
    fn_80230534(false);
}

bool cls_80128460::fn_8012824C() {
    fn_8023077C();
    for (s32 i = 0; i < mCount; i++) {
        ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(mSerials[i]);
        if (actor != NULL) {
            if (actor->mActorId != ACTOR_FZRA) {
                return false;
            }
            static_cast<ZoraEscortGoal_8049E424*>(actor)->fn_80126A4C(5);
        }
    }
    return true;
}

bool cls_80128460::fn_801282E0() {
    if (mCount <= 0) {
        return false;
    }
    for (s32 i = 0; i < mCount; i++) {
        ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(mSerials[i]);
        if (actor == NULL) {
            return false;
        }
        if (actor->mActorId != ACTOR_FZRA) {
            return false;
        }
        if (!static_cast<ZoraEscortGoal_8049E424*>(actor)->fn_80126EF8()) {
            return false;
        }
    }
    return true;
}

void cls_80128460::fn_80128398(const ActorBase_804A5770* actor) {
    mSerials[mCount] = actor->mSerial;
    mCount++;
}

void cls_80128460::fn_801283BC() {
    mCount = 0;
    for (s32 i = 0; i < 8; i++) {
        mSerials[i] = 0;
    }
}

cls_80128460* fn_801283E8() {
    static cls_80128460 lbl_804BD36C;
    return &lbl_804BD36C;
}
