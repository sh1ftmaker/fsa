// The EMSK actor (see game/act/act_ending_map_skipper.h). init stores the low
// 7 bits of the spawn argument; update counts 500 frames down, then sets the
// view's field_0x24/0x28 from a row of a three-row table and calls
// lbl_8053AAF8->fn_80136798(15, 0).

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/act/act_ending_map_skipper.h"

EndingMapSkipper_804AA498::~EndingMapSkipper_804AA498() {}

void EndingMapSkipper_804AA498::draw() {}

// FIXME: 71%. The original loads row[0] into a saved register (r31) before
// calling fn_802D6A9C and recomputes the table address for row[1] after it;
// ours computes the row pointer once (saved in r31) and loads both after the
// call, so the stack frame also drops the r30 save.
void EndingMapSkipper_804AA498::update() {
    static const s32 sViewParams[3][2] = {{3, 0}, {2, 5}, {4, 0}};
    mTimer--;
    if (mTimer == 0) {
        const s32* row = sViewParams[mIndex];
        fn_802D6A9C()->fn_802D9FF4(row[0], row[1]);
        lbl_8053AAF8->fn_80136798(15, 0);
    }
}

void EndingMapSkipper_804AA498::init() {
    mIndex = field_0x90 & 0x7F;
    mTimer = 500;
}
