#include "game/ActorBase.h"
#include "game/GameAudio.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/cls_804A1630.h"

// The actor's destructor: unless field_0x90 is set, it turns the view's flag
// off and calls fn_8022ADCC; then it releases its two emitters.
cls_804A1630::~cls_804A1630() {
    if (field_0x90 == 0) {
        fn_802D6A9C()->fn_802D7440(true);
        fn_8022ADCC();
    }
    for (s32 i = 0; i < 2; i++) {
        if (mEmitter318[i] != NULL) {
            fn_801EE434(mEmitter318[i]);
        }
    }
}

void cls_804A1630::vfunc_0x24() {
    fn_801F0E34(0x40038, 0);
    vfunc_0x28(2, 0.0f, -24.0f);
}

// The view's box for an actor layer: mVisibleBox for layers 0-7, field_0xdc
// for the rest.
extern "C" JGeometry::TBox2f* fn_801826A4(s32 layer) {
    GameView_804A9950* view = fn_802D6A9C();
    if (layer < 8) {
        return &view->mVisibleBox;
    }
    return &view->field_0xdc;
}

void ActorBase_804A5770::fn_80182660(const JGeometry::TVec3f& pos) {
    mPos = pos;
}
