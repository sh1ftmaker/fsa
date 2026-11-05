#include "game/GameAudio.h"
#include "game/act/act_stop_npc.h"
#include "game/act/act_item_pedestal.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"

// FIXME: 99.3%, regalloc-only. The loop registers are rotated: the original
// keeps mActorEnd in r31, the index in r29 and the byte offset in r30 (ours:
// r30, r29, r31). The inlined copy in the destructor matches.
void StopNPC_804AA2D8::fn_802ED140() {
    s32 end = getActorManager_801F856C()->mActorEnd;
    for (s32 i = 0; i < end; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
        if (actor != NULL && actor->mActorId == ACTOR_ITMS) {
            fn_802AC09C(static_cast<ItemPedestal_804A82A8*>(actor));
        }
    }
    lbl_8053AAF8->fn_80134FBC(0);
    gAudioMgr_8053AF80->fn_80458658(false);
    mAlive = false;
}

StopNPC_804AA2D8::~StopNPC_804AA2D8() {
    fn_802ED140();
}

void StopNPC_804AA2D8::init() {
    if (lbl_8053AAF8->field_0x7e == 0) {
        field_0x230 = 100;
        lbl_8053AAF8->fn_80134FBC(1);
        gAudioMgr_8053AF80->fn_80458658(true);
    } else {
        mAlive = false;
    }
}
