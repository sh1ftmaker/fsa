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

void cls_804A8638::vfunc_0x54(s32 a, s32 b, s32 c, s32 d, s32 e, s32 state) {
    field_0x240 = state;
    if (state == 9) {
        field_0x2e0 = a;
        field_0x2e4 = b;
        field_0x2e8 = c;
        field_0x2ec = d;
        field_0x310 = e;
        return;
    }
    field_0x2f0 = a;
    field_0x2f4 = b;
    field_0x2f8 = c;
    field_0x2ec = d;
    field_0x314 = e;
    field_0x318 = (field_0x2f0 - field_0x2e0) / 4;
    field_0x31c = (field_0x2f4 - field_0x2e4) / 4;
    field_0x320 = (field_0x2f8 - field_0x2e8) / 4;
    if (field_0x310 >= field_0x314) {
        return;
    }
    field_0x240 = 0xc;
}
