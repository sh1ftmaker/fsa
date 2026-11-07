#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/tu_802ED538.h"

void fn_802EEA24(u8 value) {
    lbl_8053AB60 = value;
}

// FIXME(shotgun): 83.7 %, missing/extra
void cls_804AA350::vfunc_0x10() {
    s32 count = getActorManager_801F856C()->mActorEnd;
    for (s32 i = 0; i < count; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
        if (actor != NULL && actor->mActorId == ACTOR_WARP && field_0x25c == actor->vfunc_0x3C()) {
            JGeometry::TVec3f pos = actor->mPos;
            pos.x = mPos.x;
            pos.y = 32.0f + mPos.y;
            actor->mPos = pos;
        }
    }
    s32 found = 0;
    for (s32 i = 0; i < count; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
        if (actor != NULL && actor->mActorId == ACTOR_SLR4) {
            field_0x260[found] = i;
            found++;
            fn_802EEA24(1);
            if (found >= 2) {
                break;
            }
        }
    }
}

cls_804AA350::~cls_804AA350() {
    if (mEmitter230 != NULL) {
        releaseEmitter_801EE434(mEmitter230);
    }
    if (mEmitter234 != NULL) {
        releaseEmitter_801EE434(mEmitter234);
    }
    if (mEmitter238 != NULL) {
        releaseEmitter_801EE434(mEmitter238);
    }
    if (mEmitter23c != NULL) {
        releaseEmitter_801EE434(mEmitter23c);
    }
    if (mEmitter240 != NULL) {
        releaseEmitter_801EE434(mEmitter240);
    }
    if (mEmitter244 != NULL) {
        releaseEmitter_801EE434(mEmitter244);
    }
    if (mEmitter248 != NULL) {
        releaseEmitter_801EE434(mEmitter248);
    }
}

void cls_804AA350::fn_802ED7D8() {
    field_0x275 = 0xFF;
    field_0x24c++;
}
