#include "game/GameAudio.h"
#include "game/act/act_floormaster.h"
#include "game/Camera.h"
#include "game/GameGlobals.h"

void Floormaster_804A6004::vfunc_0xA0() {
    if (mState != 0) {
        lbl_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
    }
}

Floormaster_804A6004::~Floormaster_804A6004() {
    fn_801EE434(mEmitter2b0);
}
