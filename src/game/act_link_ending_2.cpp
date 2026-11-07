#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/ActorId.h"
#include "game/Effect.h"
#include "game/Player.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/act_link_ending_2.h"

// FIXME(shotgun): 83.8 %, different
Link2Ending_804AE338::~Link2Ending_804AE338() {
    fn_8036E544();
}

// FIXME(shotgun): 67.0 %, missing/extra
void Link2Ending_804AE338::init() {
    field_0x230 = 0;
    field_0x234 = 0;
    field_0x238 = -1;
    field_0x23C = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24C = 0;
    field_0x2A8 = NULL;
    field_0x2AC = 0;
    field_0x2B0 = 0;
    for (s32 i = 0; i < 20; i++) {
        field_0x258[i] = NULL;
    }
    field_0x2B4 = 0;
    field_0x254 = NULL;
    field_0x250 = NULL;
    field_0x230 = 0;
    field_0x3c.x = 0.0f;
    field_0x3c.y = -0.35f;
    field_0x3c.z = 0.0f;
    field_0x24C = 0x18;
    field_0x240 = 10;
    field_0x23C = 10;
    field_0x244 = 0;
    fn_8023077C();
    fn_80230498(0, &mPos, field_0x24C + field_0x244, 0xFF, -0x80, -0x80, true);
    for (s32 i = 1; i < 4; i++) {
        fn_80230498(i, &mPos, 0, 0, -0x80, -0x80, true);
    }
    JGeometry::TVec3f pos;
    pos.z = 0.0f;
    pos.y = 384.0f * (s32)(mPos.y / 384.0f) + 192.0f;
    pos.x = 512.0f * (s32)(mPos.x * 0.001953125f) + 256.0f;
    field_0x2A8 = spawnEmitter_8013CC50(field_0x4, &pos, 0x274, 0, 0, NULL, NULL, true, 1.0f);
    s32 end = getActorManager_801F856C()->mActorEnd;
    for (s32 i = 0; i < end; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
        if (actor != NULL && actor->mActorId == ACTOR_EOWL && actor->mAlive == true) {
            field_0x250 = actor;
            break;
        }
    }
    end = getActorManager_801F856C()->mActorEnd;
    for (s32 i = 0; i < end; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
        if (actor != NULL && actor->mActorId == ACTOR_ENDM && actor->mAlive == true) {
            field_0x254 = actor;
            break;
        }
    }
}

// FIXME(shotgun): not written (size 0xFD4): update (0x8036E5E8)

void Link2Ending_804AE338::draw() {}

// FIXME(shotgun): 68.6 %, missing/extra
void Link2Ending_804AE338::fn_8036E544() {
    for (s32 i = 0; i < 20; i++) {
        if (field_0x258[i] != NULL) {
            getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(field_0x258[i]);
            field_0x258[i] = NULL;
        }
    }
    if (field_0x2A8 != NULL) {
        getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(field_0x2A8);
        field_0x2A8 = NULL;
    }
}
