#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/act/act_shadow_link_2.h"

void ShadowLink2_804A43E8::fn_801C6A4C() {
    if (field_0x340.mState == 9) {
        if (field_0x340.mFlags & 0x20) {
            field_0x340.fn_800FEB78(6);
            switch (mState) {
            case 0x25:
                startSound_801F0E34(0x400CF, 0);
                startSound_801F0E34(0x400CD, 0);
                break;
            case 0x26:
                mTargetSpeed = 1.2f;
                break;
            }
        }
    } else {
        if (field_0x340.mFlags & 0x20) {
            field_0x340.mFlags &= ~0x20;
            field_0x340.mIndex = 3;
            field_0x594++;
        }
        s32 limit = 5;
        if (mState == 0x26) {
            limit = 20;
            startSound_801F0E34(0x400D0, 0);
            if (field_0x338 >= 0 && !fn_801CAED4(field_0x338) && fn_801D0B44(field_0x338)) {
                fn_801CD930(&field_0x33c, fn_801D0280(field_0x338), 0x500);
            }
        }
        if (field_0x594 >= limit) {
            fn_801C3B7C(0x28);
        }
    }
}

void ShadowLink2_804A43E8::fn_801C6BA8() {
    if (field_0x340.mFlags & 0x20) {
        switch (field_0x340.mState) {
        case 9:
            field_0x340.fn_800FEB78(3);
            startSound_801F0E34(0x400C6, 0);
            mSpeed = mTargetSpeed = 2.0f;
            field_0x33c = fn_801F6874(field_0x98);
            break;
        case 3:
            fn_801C3B7C(0x27);
            break;
        case 2:
            field_0x340.mFlags &= ~0x20;
            field_0x340.mIndex = 0;
            field_0x594++;
            if (field_0x594 >= 2) {
                fn_801C3B7C(0x27);
            }
            break;
        }
    }
}

void ShadowLink2_804A43E8::fn_801C6C7C() {
    bool done = false;
    if (mStateTimer >= field_0x250) {
        done = true;
    } else if (field_0x338 >= 0 && !fn_801CAED4(field_0x338) && fn_801D0B44(field_0x338)) {
        f32 dist = fn_801D0DA0(field_0x338, &mPos);
        if (mStateTimer > 1 && (field_0x524 & 0x8) && dist <= square(field_0x5b0)) {
            if (field_0x524 & 0x20) {
                switch (fn_801CD664(6)) {
                case 0:
                    if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.6f) {
                        fn_801C3B7C(0x23);
                    } else {
                        fn_801C3B7C(0x24);
                    }
                    break;
                case 1:
                case 2:
                    fn_801C3B7C(0x25);
                    break;
                default:
                    fn_801C3B7C(0x26);
                    break;
                }
            } else if (field_0x524 & 0x10) {
                switch (fn_801CD664(3)) {
                case 0:
                    if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.6f) {
                        fn_801C3B7C(0x23);
                    } else {
                        fn_801C3B7C(0x24);
                    }
                    break;
                default:
                    fn_801C3B7C(0x25);
                    break;
                }
            } else if (field_0x524 & 0x8) {
                if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.6f) {
                    fn_801C3B7C(0x23);
                } else {
                    fn_801C3B7C(0x24);
                }
            }
        } else if (dist > square(lbl_8053EDF4)) {
            done = true;
        } else if (!fn_801D12CC()) {
            done = true;
        } else {
            field_0x33c = fn_801D02E0(&field_0x32c);
        }
    } else {
        done = true;
    }
    if (done) {
        if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.7f) {
            fn_801C3B7C(0xF);
        } else {
            fn_801C3B7C(0x11);
        }
    }
}
