#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/act/act_pot_2_generator.h"

Pot2Generator_804AA6F8::~Pot2Generator_804AA6F8() {}

void Pot2Generator_804AA6F8::draw() {}

void Pot2Generator_804AA6F8::update() {}

// Walks every cell of layer 0 and layers 8..14 (32 x 32 cells each) and
// replaces every tile whose code is 0x41 with a TBO2 pot actor, or with tile
// 0x12 when the view's bit for the running id is already set.
// FIXME: 98%. Regalloc only: the original keeps this in r30, the running id
// in r31 and the tile code in r29 (we swap this/id and put code in r27/r25).
void Pot2Generator_804AA6F8::init() {
    JGeometry::TVec3f pos;
    u32 id = field_0x90;

    field_0x230 = 0;
    for (s32 i = 0; i < 32; i++) {
        for (s32 j = 0; j < 32; j++) {
            u32 code = gGameRoot2_8053AB10->field_0x20->fn_802F9908(0, field_0x230) & 0x3FF;
            if (code == 0x41) {
                if (!getGameView_802D6A9C()->fn_802D76A8(id)) {
                    fn_80226CA8(field_0x230, 0, &pos);
                    getActorManager_801F856C()->spawnActor_801F9484(ACTOR_TBO2, 0, &pos, code, id, -1);
                } else {
                    gGameRoot2_8053AB10->field_0x20->fn_802F9974(0, field_0x230, 0x12);
                }
                id++;
            }
            field_0x230++;
        }
    }

    for (s32 k = 0; k < 7; k++) {
        s32 layer = k + 8;
        field_0x230 = 0;
        for (s32 i = 0; i < 32; i++) {
            for (s32 j = 0; j < 32; j++) {
                u32 code = gGameRoot2_8053AB10->field_0x20->fn_802F9908(layer, field_0x230) & 0x3FF;
                if (code == 0x41) {
                    if (!getGameView_802D6A9C()->fn_802D76A8(id)) {
                        fn_80226CA8(field_0x230, layer, &pos);
                        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_TBO2, layer, &pos, code, id, -1);
                    } else {
                        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, field_0x230, 0x12);
                    }
                    id++;
                }
                field_0x230++;
            }
        }
    }
}
