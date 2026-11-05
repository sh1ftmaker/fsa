#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/act/act_panel_rewrite_npc_2.h"

PanelRewriteNPC2_804A9F58::~PanelRewriteNPC2_804A9F58() {}

void PanelRewriteNPC2_804A9F58::draw() {}

// Scans the 32 x 32 tile cells of this actor's layer (field_0x4) for tiles
// equal to field_0x230 and rewrites them to field_0x234, with an effect (or,
// on layer 8 and up, a KMRI actor) and a sound when the flags ask for them.
// FIXME: 99%. Regalloc only: the original keeps the loop index in r30 and the
// spawn layer in r31; we swap them.
void PanelRewriteNPC2_804A9F58::update() {
    JGeometry::TVec3f pos;

    if (vfunc_0x34() == true) {
        for (s32 i = 0; i < 0x400; i++) {
            u16 tile = lbl_8053AB10->field_0x20->fn_802F9908(field_0x4, i);
            if (field_0x230 == tile) {
                lbl_8053AB10->field_0x20->fn_802F9974(field_0x4, i, field_0x234);
                if (field_0x23c != 0) {
                    fn_80226CA8(i, field_0x4, &pos);
                    s32 layer = field_0x4;
                    if (layer >= 8) {
                        fn_801F856C()->fn_801F9484(ACTOR_KMRI, layer, &pos, 0, -1, -1);
                    } else {
                        spawnEmitter_8013CC50(layer, &pos, 0x35C, 1, 0, NULL, NULL, true, 1.0f);
                    }
                }
                if (field_0x238 != 0) {
                    startSound_801F0E34(0x30003, 0);
                }
            }
        }
        getGameView_802D6A9C()->fn_802D7584(field_0x16c, 1);
        mAlive = false;
    }
}

void PanelRewriteNPC2_804A9F58::init() {
    field_0x230 = field_0x90 & 0xFFF;
    field_0x234 = (field_0x90 >> 12) & 0xFFF;
    field_0x238 = (field_0x90 >> 24) & 1;
    field_0x23c = (field_0x90 >> 25) & 1;
    if (getGameView_802D6A9C()->fn_802D7544(field_0x16c)) {
        field_0x23c = 0;
        field_0x238 = 0;
    }
    update();
}
