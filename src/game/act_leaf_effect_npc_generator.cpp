#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/act/act_leaf_effect_npc.h"
#include "game/act/act_leaf_effect_npc_generator.h"

cls_804AE288::~cls_804AE288() {}

// FIXME: 98%. Regalloc only: the original keeps the end in r31, the index in
// r28, the byte offset in r29 and the actor in r30 (ours r30, r29, r31, r28).
void cls_804AE288::fn_8036D858(JGeometry::TVec3f pos) {
    s32 end = fn_801F856C()->mActorEnd;
    for (s32 i = 0; i < end; i++) {
        ActorBase_804A5770* actor = fn_801F856C()->mActors[i];
        if (actor != NULL && actor->mActorId == ACTOR_LEAF && actor->mAlive == 1 &&
            PSVECDistance(&pos, &actor->mPos) <= 57.0f &&
            __fabsf(actor->mPos.x - pos.x) <= 42.0f) {
            static_cast<cls_804AE2E0*>(actor)->field_0x230 = 1;
            break;
        }
    }
}

void cls_804AE288::draw() {}

void cls_804AE288::update() {}

// The tile code under the cell at (x, y) on the actor's layer.
static inline u16 tileCodeAt(cls_804AE288* self, s32 x, s32 y) {
    JGeometry::TVec3f cell;
    cell.x = x;
    cell.y = y;
    return lbl_8053AB10->field_0x20->fn_802F9908(self->field_0x4, fn_80226BC4(&cell, self->field_0x4)) & 0x3FF;
}

// Spawns a LEAF actor on every cell of a 32 x 24 grid (16-unit cells, snapped
// to 512 x 384 around the generator) whose tile code is 0x1E1, 0x1E2, 0x1F1
// or 0x1F2.
// FIXME: 99%. try.py class: operands. The original keeps this in r27, the loop
// counters in r29/r28, the tile code in r26 and x/y in r24/r30 (ours: this in
// r28, counters r30/r26), and its spawn position TVec3f sits at sp+0x10
// (ours sp+0x14).
void cls_804AE288::init() {
    if (fn_802DEBA0()) {
        return;
    }
    f32 originX = 512.0f * (s32)(mPos.x / 512.0f);
    f32 originY = 384.0f * (s32)(mPos.y / 384.0f);
    for (s32 i = 0; i < 24; i++) {
        s32 y = (s32)(16.0f * i + originY);
        for (s32 j = 0; j < 32; j++) {
            s32 x = (s32)(16.0f * j + originX);
            u16 code = tileCodeAt(this, x, y);
            if (code == 0x1E1 || code == 0x1E2 || code == 0x1F1 || code == 0x1F2) {
                JGeometry::TVec3f pos(f32(x), f32(y), 0.0f);
                fn_801F856C()->fn_801F9484(ACTOR_LEAF, field_0x4, &pos, code, -1, -1);
            }
        }
    }
}
