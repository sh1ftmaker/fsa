#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
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

// The .sdata2 floats vfunc_0x6C copies into the 'RUPY' actors (not written yet).
extern const f32 lbl_8053EB20;
extern const f32 lbl_8053EB28;
extern const f32 lbl_8053EB2C;

// While mState is 5, spawns two 'RUPY' actors at mPos and sets each one's
// field_0x3c: x is lbl_8053EB28 for the first and its negation for the second,
// y and z are fixed.
void Terrorpin_804A3B10::vfunc_0x6C() {
    if (mState == 5) {
        f32 negX = -lbl_8053EB28;
        for (s32 i = 0; i < 2; i++) {
            s32 serial = fn_801F856C()->fn_801F8544(ACTOR_RUPY, field_0x4, &mPos, 2);
            if (serial >= 0) {
                ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(serial);
                if (actor != NULL) {
                    f32 x = lbl_8053EB28;
                    if (i != 0) {
                        x = negX;
                    }
                    actor->field_0x3c = JGeometry::TVec3f(x, lbl_8053EB20, lbl_8053EB2C);
                }
            }
        }
    }
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

// The s32 of .sdata (lbl_8053A100) that vfunc_0x44 compares with field_0x2a8
// (not written yet).
extern const s32 lbl_8053A100;
// The .sdata2 float vfunc_0x44 tests mPos.z against (not written yet).

void Terrorpin_804A3B10::vfunc_0x44() {
    if (field_0xd4 == 1) {
        bool active = false;
        if (mFlags & 0x4000) {
            if (field_0x2a8 == lbl_8053A100) {
                active = true;
            }
        } else {
            active = true;
        }
        if (active && mPos.z <= lbl_8053EB20) {
            switch (mState) {
            case 0:
                fn_801B91A8(1);
                break;
            case 2:
            case 3:
            case 5:
                fn_801B91A8(4);
                break;
            }
        }
        field_0xd4 = 0;
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
