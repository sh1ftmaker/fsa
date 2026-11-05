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

// FIXME: 99.2%. Register allocation only: the original keeps the row offset
// (index * 8) in r30 and row[0] in r31; ours has them swapped.
void EndingMapSkipper_804AA498::update() {
    static const s32 sViewParams[3][2] = {{3, 0}, {2, 5}, {4, 0}};
    mTimer--;
    if (mTimer == 0) {
        s32 first = sViewParams[mIndex][0];
        fn_802D6A9C()->fn_802D9FF4(first, sViewParams[mIndex][1]);
        lbl_8053AAF8->fn_80136798(15, 0);
    }
}

void EndingMapSkipper_804AA498::init() {
    mIndex = field_0x90 & 0x7F;
    mTimer = 500;
}
