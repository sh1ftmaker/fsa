#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/act/act_terrorpin.h"


// FIXME: vfunc_0x24, 0x44, 0x68, 0x6C, 0x7C, 0x94 and init are instruction-identical;
// only their float literals are @NNN, not the original lbl_ (.sdata2 order, owned
// partly by the unwritten draw / vfunc_0x5C / vfunc_0x88 / vfunc_0x90).

// The two animations' keys (cel pairs for 10 frames each, then -1) and the
// table the init hands to SpriteAnim::fn_801D2608 (names unknown).
static const s16 lbl_80469180[6] = {0, 10, 1, 10, -1, -1};
static const s16 lbl_8046918C[6] = {2, 10, 3, 10, -1, -1};
static const s16* lbl_8053A120[2] = {lbl_80469180, lbl_8046918C};

// The scale part of m (its translation column is the caller's). The draw
// loads the three scales z first, as arguments of an inline call (cf. tu_80264998.cpp).
static inline void setScaleMtx(Mtx m, f32 x, f32 y, f32 z) {
    m[0][0] = x;
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[1][0] = 0.0f;
    m[1][1] = y;
    m[1][2] = 0.0f;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = z;
}

Terrorpin_804A3B10::Terrorpin_804A3B10() {
    field_0x2a8 = 0;
}

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
        f32 scale = 1.4f;
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
    mSpeed = 0.35f;
    mTargetSpeed = 0.35f;
    mAnim.fn_801D2608(lbl_8053A120, 2);
    mAnim.startAnim_801D22DC(0, 2, false);
    mAnim.fn_801D1D60();
}

void Terrorpin_804A3B10::fn_801B91A8(s32 state) {
    switch (state) {
    case 0:
        field_0x250 = fn_801CD664(16) + 30;
        mTargetSpeed = 0.35f;
        if (mState != 4) {
            field_0x98 = fn_801CF6B8();
        }
        break;
    case 1:
    case 4:
        if (state == 1) {
            if (mFlags & 0x2000) {
                startSound_801F0E34(0x50016, 0);
            } else {
                startSound_801F0E34(0x50017, 0);
            }
        } else {
            if (mFlags & 0x2000) {
                startSound_801F0E34(0x50021, 0);
            } else {
                startSound_801F0E34(0x50022, 0);
            }
        }
        field_0x250 = -1;
        field_0x98 = fn_801CD354(field_0x98);
        mTargetSpeed = 0.6f;
        mSpeed = 0.6f;
        if (mFlags & 0x2000) {
            field_0x3c.z = 2.0f;
        } else {
            field_0x3c.z = 3.0f;
        }
        break;
    case 2:
        field_0x250 = fn_801CD664(101) + 400;
        mTargetSpeed = 0.0f;
        break;
    case 3:
        field_0x250 = fn_801CD664(11) + 30;
        mTargetSpeed = 0.0f;
        break;
    case 5:
        field_0x250 = fn_801CD664(1) + 540;
        field_0x98 = fn_801CD354(field_0x98);
        mTargetSpeed = 2.5f;
        mSpeed = 2.5f;
        field_0x3c.z = 2.0f;
        break;
    }
    mStateTimer = 0;
    mState = state;
}

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
    f32 rate = 1.0f;
    if (mState == 3) {
        rate = 2.0f;
    }
    mAnim.mFrameCtrl.setRate(rate);
    EnemyBase_804A4808::vfunc_0x94();
}

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
        if (active && mPos.z <= 0.0f) {
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

void Terrorpin_804A3B10::vfunc_0x7C() {
    if (__fabsf(field_0x3c.z) >= 0.5f) {
        if (mFlags & 0x2000) {
            startSound_801F0E34(0x50014, 0);
        } else {
            startSound_801F0E34(0x50015, 0);
        }
    }
}

u8 Terrorpin_804A3B10::vfunc_0x60() {
    u8 hit = 0;
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

// While mState is 5, spawns two 'RUPY' actors at mPos and sets each one's
// field_0x3c: x is 1.5f for the first and its negation for the second,
// y and z are fixed.
void Terrorpin_804A3B10::vfunc_0x6C() {
    if (mState == 5) {
        f32 speed = 1.5f;
        for (s32 i = 0; i < 2; i++) {
            s32 serial = getActorManager_801F856C()->fn_801F8544(ACTOR_RUPY, field_0x4, &mPos, 2);
            if (serial >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (actor != NULL) {
                    f32 x = speed;
                    if (i != 0) {
                        x = -speed;
                    }
                    actor->field_0x3c = JGeometry::TVec3f(x, 0.0f, 4.5f);
                }
            }
        }
    }
}

u8 Terrorpin_804A3B10::vfunc_0x5C() {
    u8 hit = 0;
    bool check = true;
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
        check = false;
        if (mStateTimer < 10) {
            switch (state) {
            case 5:
            case 1:
                check = true;
                break;
            }
        }
    }
    if (check) {
        if (mFlags & 0x2000) {
            fn_801CEBD0();
        } else if (state == 0) {
            s32 player = fn_801F285C(NULL, NULL, true, -1);
            if (player >= 0) {
                fn_801B91A8(5);
                JGeometry::TVec2f dir;
                dir.y = mPos.y - getPlayerPos_8023E724(player)->y;
                dir.x = mPos.x - getPlayerPos_8023E724(player)->x;
                if (dir.squared() <= JGeometry::TUtilf::epsilon()) {
                    field_0x3c.x = 0.0f;
                    field_0x3c.y = mSpeed;
                } else {
                    dir.normalize();
                    dir.scale(mSpeed);
                    field_0x3c.x = dir.x;
                    field_0x3c.y = dir.y;
                }
                startSound_801F0E34(0x4003B, 0);
            }
        }
    } else {
        hit = fn_801F2FAC();
    }
    return hit;
}

u8 Terrorpin_804A3B10::vfunc_0x68() {
    u8 hit = 0;
    if (mBox.f.x - mBox.i.x > 0.0f && mBox.f.y - mBox.i.y > 0.0f) {
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

void Terrorpin_804A3B10::vfunc_0x24() {
    // The original reads field_0x10c here, not mState (0x248) as the other vfuncs do.
    if ((mFlags & 0x2000) && (mFlags & 0x4000) && field_0x10c >= 0 && field_0x10c < 4 && field_0x10c == field_0x2a8) {
        f32 speed = 1.5f;
        for (s32 i = 0; i < 2; i++) {
            s32 serial = getActorManager_801F856C()->fn_801F8544(ACTOR_RUPY, field_0x4, &mPos, 2);
            if (serial >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (actor != NULL) {
                    f32 x = speed;
                    if (i != 0) {
                        x = -speed;
                    }
                    actor->field_0x3c = JGeometry::TVec3f(x, 0.0f, 4.5f);
                }
            }
        }
    }
    ActorBase_804A5770::vfunc_0x24();
}

void Terrorpin_804A3B10::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    if (mFlags & 0x2000) {
        mtx[0][3] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][3] = 0.0f;
        setScaleMtx(mtx, field_0x54.x, field_0x54.y, field_0x54.z);
        params.mMtx = mtx;
    }
    if (mFlags & 0x4000) {
        params.field_0x0[8] = field_0x2a8 + 2;
    }
    drawSprite_801F06F0("enemy_gc_sob.bin", mAnim.mCel + 0xBD, &params, 0, 1.0f, 1.0f);
}

Terrorpin_804A3B10::~Terrorpin_804A3B10() {}
