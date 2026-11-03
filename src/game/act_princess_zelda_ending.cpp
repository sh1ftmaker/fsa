#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/act/act_princess_zelda_ending.h"

PrincessZeldaEnding_804AD6F0::~PrincessZeldaEnding_804AD6F0() {}

// The state table: update() calls the function for mState. The original's
// copy of it is zero in .data and __sinit_act_princess_zelda_ending_cpp fills
// it at run time (that copy matches at 99.35 %); the initialised table here
// leaves .data short of 100 %.
static void (PrincessZeldaEnding_804AD6F0::*lbl_804AD6A8[6])() = {
    &PrincessZeldaEnding_804AD6F0::fn_8035E394,
    &PrincessZeldaEnding_804AD6F0::fn_8035E274,
    &PrincessZeldaEnding_804AD6F0::fn_8035E238,
    &PrincessZeldaEnding_804AD6F0::fn_8035E070,
    &PrincessZeldaEnding_804AD6F0::fn_8035DFE4,
    &PrincessZeldaEnding_804AD6F0::fn_8035DF48,
};

void PrincessZeldaEnding_804AD6F0::update() {
    if (field_0x234 != 0) {
        field_0x234--;
    }
    if (field_0x244 != 0) {
        field_0x244--;
    }
    (this->*lbl_804AD6A8[mState])();
}

// State 0: starts the first step (fn_8035C1C4 with id 0) once field_0x238 is set.
void PrincessZeldaEnding_804AD6F0::fn_8035E394() {
    if (field_0x238 != 0) {
        mState = 1;
        fn_8035C1C4(this, 0, 0x274);
        field_0x238 = 0;
    }
}

// State 2: once field_0x238 is set, moves to state 3 with a 100-frame countdown.
void PrincessZeldaEnding_804AD6F0::fn_8035E238() {
    if (field_0x244 == 0) {
        field_0x248 = 0;
    }
    if (field_0x238 != 0) {
        mState = 3;
        field_0x234 = 100;
        field_0x238 = 0;
    }
}

// State 4: waits for the countdown, then tells the partner to start (its
// field_0x234 = 1) and moves on to state 5; field_0x248 is set while the
// countdown runs.
void PrincessZeldaEnding_804AD6F0::fn_8035DFE4() {
    if (field_0x234 == 0) {
        if (field_0x238 == 0) {
            EndingPartnerView* partner =
                static_cast<EndingPartnerView*>(fn_801F856C()->fn_801F666C(field_0x23c));
            if (partner != NULL) {
                partner->field_0x234 = 1;
            }
        } else {
            fn_8035C1C4(this, 6, 0x9b);
            mState++;
        }
    } else if (field_0x234 < 0xF0) {
        field_0x248 = 0x18;
    }
}

// State 5: walks mPos.y up to 1966, then hands the partner over to state 7 and
// clears mAlive.
void PrincessZeldaEnding_804AD6F0::fn_8035DF48() {
    if (mPos.y < 1966.0f) {
        if (fn_8035C15C(this, 6) == 1) {
            fn_8035C1C4(this, 6, 0x9b);
        }
        mPos.y += 0.23f;
    } else {
        EndingPartnerView* partner =
            static_cast<EndingPartnerView*>(fn_801F856C()->fn_801F666C(field_0x23c));
        if (partner != NULL) {
            partner->mState = 7;
            partner->field_0x234 = 0;
        }
        mAlive = false;
    }
}

// State 1: a sequence of steps on field_0x238, each waiting for fn_8035C15C
// (the other unit's test) before it starts its fn_8035C1C4 action.
void PrincessZeldaEnding_804AD6F0::fn_8035E274() {
    if (field_0x238 == 0) {
        if (fn_8035C15C(this, 0) == 1) {
            fn_8035C1C4(this, 1, 0x21b);
            field_0x240 = 0;
            field_0x238 = 4;
        }
    } else if (field_0x238 == 4) {
        if (fn_8035C15C(this, 1) == 1) {
            fn_8035C1C4(this, 1, 0x21b);
        }
    } else if (field_0x238 == 1) {
        fn_8035C1C4(this, 3, 0x274);
        field_0x240 = 1;
        field_0x238 = 2;
    } else if (field_0x238 == 2) {
        if (fn_8035C15C(this, 2) == 1) {
            fn_8035C1C4(this, 2, 0x274);
        }
    } else if (field_0x238 == 3) {
        field_0x248 = 3;
        field_0x244 = 0x45;
        field_0x24c = 0;
        mState = 2;
        field_0x238 = 0;
    }
}

// State 3: walks mPos across x, then y, with a 16-frame pause between the
// legs (field_0x234), then moves on to state 4.
// FIXME: 83.1 %. The original tests x with `cror eq, gt, eq; bne` and lays
// the 1234 block out after the 0.31 block. `x >= 1234` gets the cror but puts
// the 1234 block right after the bne (64.7 %); `x < 1234` (this form) falls
// through to the 0.31 block but compiles to a bge, so the far-block layout
// is not reached.
void PrincessZeldaEnding_804AD6F0::fn_8035E070() {
    if (field_0x234 != 0) {
        if (field_0x234 == 1) {
            if (field_0x238 == 0) {
                fn_8035C1C4(this, 5, 0x9b);
            }
        }
    } else {
        if (field_0x238 == 0) {
            if (fn_8035C15C(this, 5) == 1) {
                fn_8035C1C4(this, 5, 0x9b);
            }
            if (mPos.x < 1234.0f) {
                mPos.x -= 0.31f;
            } else {
                mPos.x = 1234.0f;
                fn_8035C1C4(this, 6, 0x9b);
                field_0x238++;
                field_0x234 = 0x10;
            }
        } else if (field_0x238 == 1) {
            if (fn_8035C15C(this, 6) == 1) {
                fn_8035C1C4(this, 6, 0x9b);
            }
            if (mPos.y >= 1810.0f) {
                mPos.y = 1810.0f;
                fn_8035C1C4(this, 4, 0x9b);
                field_0x238++;
                field_0x234 = 0x10;
            } else {
                mPos.y += 0.31f;
            }
        } else {
            if (fn_8035C15C(this, 4) == 1) {
                fn_8035C1C4(this, 4, 0x9b);
            }
            if (mPos.x >= 1260.0f) {
                mPos.y = 1810.0f;
                fn_8035C1C4(this, 4, 0x9b);
                field_0x238 = 0;
                mState++;
                field_0x234 = 0x16f;
            } else {
                mPos.x += 0.31f;
            }
        }
    }
}

// Draws the SOB animation at frame field_0x250 + field_0x248; the file is
// enemy_gc_sob.bin, or enemy_both2_sob.bin when field_0x240 is set.
void PrincessZeldaEnding_804AD6F0::draw() {
    SpriteParams_801F5AE0 params;
    if (mState < 3 || (mState == 3 && field_0x238 == 0)) {
        params.field_0x1d = -1;
    } else {
        params.field_0x1d = 0;
    }
    if (field_0x240 == 0) {
        fn_801F06F0("enemy_gc_sob.bin", field_0x250 + field_0x248, &params, 7, 1.0f, 1.0f);
    } else {
        fn_801F06F0("enemy_both2_sob.bin", field_0x250 + field_0x248, &params, 7, 1.0f, 1.0f);
    }
}

void PrincessZeldaEnding_804AD6F0::init() {
    mState = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x240 = 1;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0x274;
}
