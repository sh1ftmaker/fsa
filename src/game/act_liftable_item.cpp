#include "game/GameAudio.h"
#include "game/act/act_liftable_item.h"

LiftableItem_804A01D0::~LiftableItem_804A01D0() {
    releaseEmitter_801EE434(mEmitter2a8);
    releaseEmitter_801EE434(mEmitter2ac);
    releaseEmitter_801EE434(mEmitter2b0);
}

void LiftableItem_804A01D0::fn_801597C0() {
    vfunc_0x24();
}

s32 LiftableItem_804A01D0::fn_801597B8() {
    return field_0x230;
}
