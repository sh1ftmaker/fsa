#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/ActorManager.h"
#include "game/act/act_shadow_link_jump.h"

// The two boxes (min i, max f) set to the same square: stores go in TBox2f
// order, as the original's.
static inline void setBox(JGeometry::TBox2f& box, f32 lo, f32 hi) {
    box.i.x = lo;
    box.i.y = lo;
    box.f.x = hi;
    box.f.y = hi;
}

// Function-local statics of fn_8022089C, in the original's .sbss order.
static f32 lbl_8053AC80;
static s8 lbl_8053AC84;
static f32 lbl_8053AC88;
static s8 lbl_8053AC8C;

cls_804A68D0::cls_804A68D0() {
    mKind = 0;
    mHops = 0;
    field_0x2c0 = fn_801CD664(1) + 3;
}

cls_804A68D0::~cls_804A68D0() {}

void cls_804A68D0::init() {
    cls_804A6A00::init();
    mKind = field_0x90 & 3;
    field_0x98 = 2;
    mState = 3;
    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mAnim.fn_801D22DC(2, 0, false);
}

void cls_804A68D0::vfunc_0x88() {
    switch (mState) {
    case 2:
        fn_80221250();
        break;
    case 4:
    case 6:
        field_0x3c.z = field_0x3c.z + lbl_8053EFB8;
        break;
    default:
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        vfunc_0x84();
        break;
    }
}

// FIXME: 99.86%. Only the jump table relocation differs (jumptable_804A6974 vs
// @NNN), as in act_shadow_link_bow's vfunc_0x90.
void cls_804A68D0::vfunc_0x90(u8 mapHit) {
    switch (mState) {
    case 0:
        fn_80221408();
        break;
    case 1:
        fn_80221334();
        break;
    case 2:
        fn_802212AC();
        break;
    case 3:
        if (mFlags & 0x4000) {
            if (mPos.z <= 0.0f) {
                fn_8022089C(4);
            }
        } else if (fn_801D05F8(&mPos, 48.0f) >= 0) {
            mFlags |= 0x4000;
            setBox(mBox, 1.0f, -1.0f);
            mBox2.i = mBox.i;
            mBox2.f = mBox.f;
            if (mPos.z <= 0.0f) {
                fn_8022089C(4);
            }
        } else if (mPos.z <= 0.0f) {
            field_0x3c.z = lbl_8053FE70;
        }
        break;
    case 4:
        if (mPos.z <= 0.0f) {
            mHops++;
            fn_8022089C(5);
        }
        break;
    case 5:
        if (mStateTimer >= field_0x250) {
            if (mHops >= field_0x2c0) {
                fn_8022149C(1);
            } else {
                fn_8022089C(3);
            }
        }
        break;
    case 6:
        if (mStateTimer >= 120) {
            fn_801F3668();
        }
        break;
    }
}

void cls_804A68D0::fn_8022089C(s32 state) {
    switch (state) {
    case 3:
        setBox(mBox, -8.0f, 8.0f);
        setBox(field_0x80, -8.0f, 8.0f);
        break;
    case 4:
        mFlags &= ~0x4000;
        setBox(mBox, 1.0f, -1.0f);
        mBox2.i = mBox.i;
        mBox2.f = mBox.f;
        setBox(field_0x80, 1.0f, -1.0f);
        if (lbl_8053AC84 == 0) {
            lbl_8053AC84 = 1;
            lbl_8053AC80 = 0.5f * (45.0f * -lbl_8053EFB8);
        }
        field_0x3c.z = lbl_8053AC80;
        switch (mKind) {
        case 0:
        case 1:
            field_0x3c.x = 2.8444445f;
            if (mKind == 1) {
                field_0x3c.x = -field_0x3c.x;
            }
            if (mHops & 1) {
                field_0x3c.x = -field_0x3c.x;
            }
            field_0x3c.y = 0.0f;
            break;
        case 2:
        case 3:
            field_0x3c.y = 2.8444445f;
            if (mKind == 3) {
                field_0x3c.y = -field_0x3c.y;
            }
            if (mHops & 1) {
                field_0x3c.y = -field_0x3c.y;
            }
            field_0x3c.x = 0.0f;
            break;
        }
        fn_801F0E34(0x400C7, 0);
        break;
    case 5:
        setBox(mBox, -8.0f, 8.0f);
        setBox(field_0x80, -8.0f, 8.0f);
        if (lbl_8053AAF8->field_0x7e) {
            field_0x250 = 6;
        } else {
            field_0x250 = 12;
        }
        break;
    case 6:
        fn_801F3604();
        setBox(mBox, 1.0f, -1.0f);
        mBox2.i = mBox.i;
        mBox2.f = mBox.f;
        setBox(field_0x80, 1.0f, -1.0f);
        if (lbl_8053AC8C == 0) {
            lbl_8053AC8C = 1;
            lbl_8053AC88 = 0.5f * (120.0f * -lbl_8053EFB8);
        }
        field_0x3c.z = lbl_8053AC88;
        fn_80221670();
        field_0x3c.x = (field_0x2b0 - mPos.x) * 0.008333334f;
        field_0x3c.y = (field_0x2b4 - mPos.y) * 0.008333334f;
        s32 angle = JMath::atanTable_.atan2_(field_0x3c.x, field_0x3c.y) *
                    (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
        field_0x98 = fn_801F681C((u16)angle);
        mAnim.fn_801D22DC(field_0x98 + 8, 0, false);
        break;
    }
    mStateTimer = 0;
    mState = state;
}
