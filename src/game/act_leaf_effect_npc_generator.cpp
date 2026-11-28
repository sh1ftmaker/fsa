#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/act/act_leaf_effect_npc.h"
#include "game/act/act_leaf_effect_npc_generator.h"


// The tile code under the cell at (x, y) on the actor's layer.
static inline u16 tileCodeAt(LeafEffectNPCGenerator_804AE288* self, s32 x, s32 y) {
    JGeometry::TVec2f cell(x, y);
    return gGameRoot2_8053AB10->field_0x20->getTile_802F9908(self->mLayer_0x4, getCellIndex_80226BC4(&cell, self->mLayer_0x4)) & 0x3FF;
}

static inline void spawnLeaves(LeafEffectNPCGenerator_804AE288* self, f32 originY, f32 originX) {
    s32 j;
    for (s32 i = 0; i < 24; i++) {
        s32 y = (s32)(16.0f * i + originY);
        for (j = 0; j < 32; j++) {
            s32 x = (s32)(16.0f * j + originX);
            u16 code = tileCodeAt(self, x, y);
            if (code == 0x1E1 || code == 0x1E2 || code == 0x1F1 || code == 0x1F2) {
                JGeometry::TVec3f pos(f32(x), f32(y), 0.0f);
                getActorManager_801F856C()->spawnActor_801F9484(ACTOR_LEAF, self->mLayer_0x4, &pos, code, -1, -1);
            }
        }
    }
}

// Spawns a LEAF actor on every cell of a 32 x 24 grid (16-unit cells, snapped
// to 512 x 384 around the generator) whose tile code is 0x1E1, 0x1E2, 0x1F1
// or 0x1F2.
void LeafEffectNPCGenerator_804AE288::init() {
    if (fn_802DEBA0()) {
        return;
    }
    f32 originX = 512.0f * (s32)(mPos.x / 512.0f);
    f32 originY = 384.0f * (s32)(mPos.y / 384.0f);
    spawnLeaves(this, originY, originX);
}

void LeafEffectNPCGenerator_804AE288::update() {}

void LeafEffectNPCGenerator_804AE288::draw() {}

// FIXME: 98%. Regalloc only: the original keeps the end in r31, the index in
// r28, the byte offset in r29 and the actor in r30 (ours r30, r29, r31, r28).
void LeafEffectNPCGenerator_804AE288::fn_8036D858(JGeometry::TVec3f pos) {
    s32 end = getActorManager_801F856C()->getActorEnd();
    for (s32 i = 0; i < end; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
        if (actor != NULL && actor->mActorId == ACTOR_LEAF && actor->mAlive == 1 &&
            PSVECDistance(&pos, &actor->mPos) <= 57.0f &&
            __fabsf(actor->mPos.x - pos.x) <= 42.0f) {
            static_cast<LeafEffectNPC_804AE2E0*>(actor)->field_0x230 = 1;
            break;
        }
    }
}

LeafEffectNPCGenerator_804AE288::~LeafEffectNPCGenerator_804AE288() {}
