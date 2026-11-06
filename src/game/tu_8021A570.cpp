#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/tu_8021A570.h"
#include <math.h>

// True when the angle is within a quarter turn of 0x8000.
static inline bool nearHalfTurn(s16 angle) {
    s16 diff = angle + 0x8000;
    f32 mag = __fabsf(diff);
    return (1.0f / 32768.0f) * mag < 0.5f;
}

// The body of fn_8021ABC0, inlined into fn_8021A570.
static inline void pickNext(cls_8021ACE0* self) {
    if (self->field_0xc == 0) {
        if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.9f) {
            self->mState = 1;
            self->mTimer2 = fn_801CD664(0xB5) + 0x78;
            if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
                self->mSpeed = -0x180;
            } else {
                self->mSpeed = 0x180;
            }
        } else {
            self->mState = 0;
            self->mTimer2 = fn_801CD664(0xB5) + 0x78;
            self->mSpeed = 0;
        }
    } else {
        self->mTimer = 0;
    }
}

cls_8021ACE0::cls_8021ACE0() {
    mState = 0;
    mAngle = 0;
    mTimer = 0;
    mTimer2 = 0;
    mSpeed = 0;
    field_0xc = 0;
    mFlipped = 0;
}

void cls_8021ACE0::fn_8021ABC0() {
    pickNext(this);
}

// FIXME: 98.8%. In the settle-at-half-turn block (c == 0: state 0, timer 0xF)
// the original keeps the bool in r3 and reuses that register as the 0 for the
// state/timer2/speed stores and the else branch's timer = 0; we put the bool in
// r4 and use separate li 0s (so the branch layout and the registers of the
// stores after it differ).
bool cls_8021ACE0::fn_8021A570(bool flag) {
    bool result = false;
    if (mTimer != 0) {
        mTimer--;
        if (mState == 0) {
            mAngle += mFlipped ? 0x889 : -0x889;
            if (mTimer == 0) {
                if (nearHalfTurn(mAngle)) {
                    mAngle = -0x8000;
                    mFlipped = 1;
                } else {
                    mAngle = 0;
                    mFlipped = 0;
                }
                if (flag) {
                    if (field_0xc == 0) {
                        mState = 1;
                        mTimer = fn_801CD664(0xB5) + 0x78;
                        mTimer2 = 0;
                        if (mFlipped) {
                            mSpeed = -0x180;
                        } else {
                            mSpeed = 0x180;
                        }
                    } else {
                        mTimer = 0;
                    }
                } else {
                    pickNext(this);
                }
            }
        } else if (mState == 1) {
            s16 prev = mAngle;
            bool hit = false;
            mAngle += mSpeed;
            if (mFlipped) {
                if ((u16)mAngle >= 0x9555) {
                    mAngle = -0x6AAB;
                    hit = true;
                } else if ((u16)mAngle <= 0x6AAB) {
                    mAngle = 0x6AAB;
                    hit = true;
                }
            } else {
                if (mAngle >= 0x1555) {
                    mAngle = 0x1555;
                    hit = true;
                } else if (mAngle <= -0x1555) {
                    mAngle = -0x1555;
                    hit = true;
                }
            }
            if (hit) {
                mSpeed = -mSpeed;
                result = true;
            }
            if ((prev <= 0 && mAngle >= 0) || (prev >= 0 && mAngle <= 0)) {
                if (mTimer == 0 || field_0xc != 0) {
                    if (nearHalfTurn(mAngle)) {
                        bool settled = false;
                        mAngle = -0x8000;
                        if (field_0xc == 0) {
                            mState = 0;
                            mTimer = 0xF;
                            mTimer2 = 0;
                            mSpeed = 0;
                            settled = true;
                        } else {
                            mTimer = 0;
                        }
                        if (settled) {
                            result = true;
                        }
                    } else {
                        mAngle = 0;
                        pickNext(this);
                        mTimer = 0;
                    }
                }
            } else if (mTimer == 0) {
                mTimer = 1;
            }
        }
    }
    if (mTimer2 != 0) {
        mTimer2--;
        if (mTimer2 == 0) {
            if (flag && mTimer == 0 && field_0xc == 0) {
                if (mState == 0) {
                    mTimer = 0xF;
                    result = true;
                } else if (mState == 1) {
                    mTimer = fn_801CD664(0xB5) + 0x78;
                }
            } else {
                pickNext(this);
            }
        }
    }
    return result;
}
