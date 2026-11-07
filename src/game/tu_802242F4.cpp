#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/ViewLayer.h"

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
                    !(__fabsf(actor->mPos.z - getPlayerPos_8023E724(player)->z) > 32.0f)) {
                    actor->fn_801A3C50(player);
                    result = true;
                }
            }
        }
    }
    return result;
}
