#include "game/GameAudio.h"
#include "game/EmitterDelete.h"
#include "game/cls_804A8638.h"

void cls_804A8638::draw() {}

// Called by act_blizzard_battle's notifySwev with the SWEV actor. Its layout
// (0x23C emitter, 0x290 word) is cls_804A8638's, so the parameter is typed as
// that class; GameGlobals.h still declares it with ActorBase_804A5770* (see the
// report: the SWEV class is not confirmed).
extern "C" void fn_802B39E8(cls_804A8638* actor) {
    actor->field_0x290 = 0;
    deleteEmitter(&actor->mEmitter);
}
