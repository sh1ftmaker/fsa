#include "game/ActorBase.h"
#include "game/Camera.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/act/act_thief.h"

void Thief_804A5A30::vfunc_0xA0() {
    if (mState != 9 && mState != 8) {
        lbl_8053AB10->field_0x24->fn_8030C210(&mPos);
    }
}

bool ActorBase_804A5770::fn_80204D44() {
    bool result = false;
    if (field_0x94 == 1 || field_0xc4 != 0) {
        result = true;
    }
    return result;
}

bool Thief_804A5A30::fn_802064E0() {
    bool result = false;
    s32 bit = field_0x354;
    if (bit != 0) {
        if (fn_802D6A9C()->fn_802D7F40(bit)) {
            result = true;
        }
    }
    return result;
}
