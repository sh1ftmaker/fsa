#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/act/tu_802201D8.h"

// FIXME: 79%. Same instructions but the original loads field_0x4 (r4) and
// computes &mPos from `this` (r3) before it loads lbl_8053AB10->field_0x2c
// into r3 (receiver last); ours loads the receiver first, moves `this` to r7
// and loads field_0x4 last. Tried a free function and a named TColor local.
void cls_804A6828::draw() {
    lbl_8053AB10->field_0x2c->fn_80158B5C(field_0x4, &mPos, JUtility::TColor(0, 0, 0, 0x80),
                                          24.0f);
}

void cls_804A6828::update() {
    cls_804A48B0::update();
    fn_8013CB44(mEmitter, &mPos, 0, 0, true);
}

void cls_804A6828::init() {
    cls_804A48B0::init();
    mEmitter = fn_8013CC50(field_0x4, &mPos, 0x290, 0, 0, NULL, NULL, true, 1.0f);
}

cls_804A6828::~cls_804A6828() {
    fn_801EE434(mEmitter);
}

cls_804A6828::cls_804A6828() {
    mEmitter = NULL;
}
