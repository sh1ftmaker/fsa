#include "game/GameAudio.h"

#include "game/cls_80218A8C.h"
#include "game/GameGlobals.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameMath.h"
#include "game/act/act_shadow_link_2.h"
#include "game/act/act_bomb.h"

cls_80218A8C::cls_80218A8C() {
    mOwner = NULL;
    mTimer = fn_801CD664(0x79) + 0x3c;
}

bool cls_80218A8C::fn_802189AC() {
    ActorBase_804A5770* actors[ACTOR_MANAGER_MAX_ACTORS];
    bool found = false;
    s32 count = getActorManager_801F856C()->fn_801F7818(ACTOR_HBMB, mOwner->field_0x4, actors, -1);
    for (s32 i = 0; i < count; i++) {
        Bomb_8049F784* bomb = static_cast<Bomb_8049F784*>(actors[i]);
        if (bomb != NULL && bomb->mAlive && bomb->field_0x94 != 1 && bomb->field_0x238 < 5) {
            f32 dx = bomb->mPos.x - mOwner->mPos.x;
            f32 dy = bomb->mPos.y - mOwner->mPos.y;
            if (square(dx) + square(dy) <= 1024.0f) {
                found = true;
                break;
            }
        }
    }
    return found;
}

void cls_80218A8C::fn_802188B0() {
    if (fn_801C0D70()->field_0x0 & 0x400) {
        return;
    }
    if (mOwner->field_0x564 == 3) {
        return;
    }
    if (mTimer > 1) {
        mTimer--;
        return;
    }
    if (mOwner->mPos.z <= 0.0f && mOwner->mSpeed >= lbl_8053EDF0 - 0.001f
        && !mOwner->fn_801C5348(64.0f)) {
        switch (mOwner->mState) {
        case 0x11:
        case 0x1b:
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_HBMB, mOwner->field_0x4, &mOwner->mPos, 10, -1, -1);
            mTimer = fn_801CD664(0x79) + 0xf0;
            break;
        }
    }
}
