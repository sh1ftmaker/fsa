#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/act/act_terrorpin.h"

Terrorpin_804A3B10::Terrorpin_804A3B10() {
    field_0x2a8 = 0;
}

Terrorpin_804A3B10::~Terrorpin_804A3B10() {}

// The two animations' keys (cel pairs for 10 frames each, then -1) and the
// table the init hands to SpriteAnim::fn_801D2608 (names unknown).
static const s16 lbl_80469180[6] = {0, 10, 1, 10, -1, -1};
static const s16 lbl_8046918C[6] = {2, 10, 3, 10, -1, -1};
static const s16* lbl_8053A120[2] = {lbl_80469180, lbl_8046918C};

// The .sdata2 float init uses as the speed and the scale of the hit box
// (not written yet).
extern const f32 lbl_8053EB5C;
extern const f32 lbl_8053EB68;

void Terrorpin_804A3B10::init() {
    EnemyBase_804A4808::init();
    // field_0x90 bit 0 enables the scaled box; bits 1..3 pick field_0x2a8.
    if (field_0x90 & 1) {
        mFlags |= 0x2000;
        u32 sel = (field_0x90 >> 1) & 7;
        if (sel >= 1 && sel <= 4) {
            mFlags |= 0x4000;
            field_0x2a8 = sel - 1;
        }
    }
    if (mFlags & 0x2000) {
        f32 scale = lbl_8053EB68;
        field_0x54.x = scale;
        field_0x54.y = scale;
        field_0x54.z = scale;
        mBox.i.x *= scale;
        mBox.i.y *= scale;
        mBox.f.x *= scale;
        mBox.f.y *= scale;
        field_0x80.i.x = mBox.i.x;
        field_0x80.i.y = mBox.i.y;
        field_0x80.f.x = mBox.f.x;
        field_0x80.f.y = mBox.f.y;
    } else {
        mLife = 1;
    }
    mLifeBackup = mLife;
    field_0xb0 = 0x4130B;
    mState = 0;
    mStateTimer = 0;
    field_0x250 = fn_801CD664(16) + 30;
    mSpeed = lbl_8053EB5C;
    mTargetSpeed = lbl_8053EB5C;
    mAnim.fn_801D2608(lbl_8053A120, 2);
    mAnim.fn_801D22DC(0, 2, false);
    mAnim.fn_801D1D60();
}

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

void Terrorpin_804A3B10::vfunc_0x24() {
    // The original reads field_0x10c here, not mState (0x248) as the other vfuncs do.
    if ((mFlags & 0x2000) && (mFlags & 0x4000) && field_0x10c >= 0 && field_0x10c < 4 && field_0x10c == field_0x2a8) {
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
    ActorBase_804A5770::vfunc_0x24();
}

u8 Terrorpin_804A3B10::vfunc_0x68() {
    u8 hit = 0;
    if (mBox.f.x - mBox.i.x > lbl_8053EB20 && mBox.f.y - mBox.i.y > lbl_8053EB20) {
        bool inState = false;
        // Case 1 has its own body: the original keeps it apart from 2 and 3.
        switch (mState) {
        case 1:
            inState = true;
            break;
        case 2:
        case 3:
            inState = true;
            break;
        }
        if (inState) {
            JGeometry::TBox2f box;
            fn_801F31C4(&box);
            fn_802397B4(&box, field_0x4, mSerial);
        } else {
            hit = fn_801F2B7C(field_0x114, -1, 1);
            if (hit != 0 && mState == 5) {
                for (s32 i = 0; i < 4; i++) {
                    if (hit & (1 << i)) {
                        fn_80236074(i, 0x64, 0);
                    }
                }
            }
        }
    }
    return hit;
}

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
