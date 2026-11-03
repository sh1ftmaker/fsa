#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"

// GameView byte 0x131DF + layer. The layer is loaded before fn_802D6A9C is
// called, as for an argument (the same helper is in game/tu_801CDC8C.cpp).
static inline u8 viewLayerFlag(s32 layer) {
    return fn_802D6A9C()->field_0x131df[layer];
}

// FIXME: 99.85 %. Only the index add differs: the target does addis r0,r3,1;
// add r3,r0,r31 (view + layer), ours addis r0,r31,1; add r3,r0,r3.
// Returns whether an actor with field_0x1a8 set and field_0x196 clear has
// its box overlapping a player of its layer (the first such one is passed to
// fn_801A3C50); on layers without the view's flag at GameView 0x131DF the actor must also be
// within 32 units of the player in z.
bool fn_802242F4(ActorBase_804A5770* actor) {
    bool result = false;
    if (actor->field_0x1a8 && !actor->field_0x196) {
        JGeometry::TBox2f box;
        actor->fn_801F31C4(&box);
        if (box.f.x - box.i.x > 0.0f && box.f.y - box.i.y > 0.0f) {
            JGeometry::TVec2f out;
            s32 player = fn_8023A7BC(&box, actor->field_0x4, &out);
            if (player >= 0 && player < 4) {
                if (viewLayerFlag(actor->field_0x4) != 0 ||
                    !(__fabsf(actor->mPos.z - fn_8023E724(player)->z) > 32.0f)) {
                    fn_801A3C50(actor, player);
                    result = true;
                }
            }
        }
    }
    return result;
}
