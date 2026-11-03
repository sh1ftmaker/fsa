#include "game/GameAudio.h"
#include "game/act/act_terrorpin.h"

Terrorpin_804A3B10::Terrorpin_804A3B10() {
    field_0x2a8 = 0;
}

Terrorpin_804A3B10::~Terrorpin_804A3B10() {}

bool Terrorpin_804A3B10::vfunc_0x60() {
    bool hit = false;
    bool againstActor = true;
    bool active = false;
    s32 state = mState;
    switch (state) {
    case 1:
    case 2:
    case 3:
    case 5:
        active = true;
        break;
    }
    if (active) {
        againstActor = false;
        if (mStateTimer < 10) {
            switch (state) {
            case 5:
            case 1:
                againstActor = true;
                break;
            }
        }
    }
    if (againstActor) {
        fn_801F2DB4(mActorId, 'NULL', NULL);
    } else {
        hit = fn_801F2DB4('NULL', 'NULL', NULL);
    }
    return hit;
}

// The 0.1f is lbl_8053EB34, the sixth float of the unit's .sdata2 (after the
// literals of draw, vfunc_0x24, vfunc_0x68 and vfunc_0x5C, not written yet).
extern const f32 lbl_8053EB34;

void Terrorpin_804A3B10::vfunc_0x7C() {
    if (__fabsf(field_0x3c.z) >= lbl_8053EB34) {
        if (mFlags & 0x2000) {
            fn_801F0E34(0x50014, 0);
        } else {
            fn_801F0E34(0x50015, 0);
        }
    }
}

// The two rates of the .sdata2 floats used by vfunc_0x94 (not written yet).
extern const f32 lbl_8053EB24;
extern const f32 lbl_8053EB50;

// 0x2A0 is mAnim.mFrameCtrl.mRate (J3DFrameCtrl _0C at 0x270 + 0x24 + 0xC).
void Terrorpin_804A3B10::vfunc_0x94() {
    // The bool and the 0/1 argument are separate locals: the original keeps
    // both (li r3 / li r4) and tests the bool with clrlwi. to build the flag.
    s32 flag = 0;
    bool active = false;
    switch (mState) {
    case 1:
    case 2:
    case 3:
    case 5:
        active = true;
        break;
    }
    if (active) {
        flag = 1;
    }
    mAnim.fn_801D22A0(flag, 2);
    f32 rate = lbl_8053EB24;
    if (mState == 3) {
        rate = lbl_8053EB50;
    }
    mAnim.mFrameCtrl.setRate(rate);
    EnemyBase_804A4808::vfunc_0x94();
}
