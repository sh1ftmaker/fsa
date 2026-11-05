#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/ActorManager.h"
#include "game/act/tu_80222CC4.h"

// Zeroes the life of the 'DDGO' actor spawned in state 5 and forgets it.
static inline void releaseShot(cls_804A6C38* self) {
    s32 serial = self->mDDGOSerial;
    if (serial >= 0) {
        ActorBase_804A5770* actor = fn_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            actor->mLife = 0;
        }
        self->mDDGOSerial = -1;
    }
}

// FAKEMATCH(crack-pass): deferred codegen for this file only, so the vtable is
// written with its first user and precedes vfunc_0x90's jump table (original
// .data: vtable 0, jump table 0xA4); the game's -inline auto puts every vtable
// last. Functions are listed in reverse address order because deferred codegen
// emits them in reverse. See docs/CRACK_PASS.md.
#pragma defer_codegen on

cls_804A6C38::cls_804A6C38() {
    mDDGOSerial = -1;
}

cls_804A6C38::~cls_804A6C38() {
    releaseShot(this);
}

void cls_804A6C38::init() {
    ShadowLinkBase_804A6A00::init();
    field_0x98 = field_0x90 & 3;
    mState = 4;
    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mAnim.startAnim_801D22DC(2, 0, false);
}

void cls_804A6C38::vfunc_0x90(u8 mapHit) {
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
        if (mStateTimer >= 120) {
            mAnim.startAnim_801D22DC(field_0x98 + 16, 0, false);
            mStateTimer = 0;
            mState = 5;
        }
        break;
    case 4:
        fn_802216E8(120);
        if (mPos.z <= 0.0f) {
            if (mFlags & CLS_804A6A00_FLAG_0x2000) {
                mAnim.startAnim_801D22DC(field_0x98 + 16, 0, false);
                mStateTimer = 0;
                mState = 5;
            } else {
                field_0x3c.z = lbl_8053FE70;
            }
        }
        break;
    case 5:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            JGeometry::TVec3f pos(mPos.x, mPos.y, 0.0f);
            switch (field_0x98) {
            case 0:
                pos.x += 24.0f;
                break;
            case 1:
                pos.x -= 24.0f;
                break;
            case 2:
                pos.x += 4.0f;
                pos.y += 22.0f;
                break;
            case 3:
                pos.x -= 4.0f;
                pos.y -= 26.0f;
                break;
            }
            mDDGOSerial = fn_801F856C()->fn_801F9484('DDGO', field_0x4, &pos, 1, -1, -1);
            s32 serial = mDDGOSerial;
            if (serial >= 0) {
                ActorBase_804A5770* actor = fn_801F856C()->findActorBySerial_801F666C(serial);
                if (actor != NULL) {
                    actor->field_0x98 = field_0x98;
                }
            }
            mStateTimer = 0;
            mState = 6;
        }
        break;
    case 6:
        if (mStateTimer >= 180) {
            releaseShot(this);
            mAnim.startAnim_801D22DC(field_0x98, 0, false);
            mStateTimer = 0;
            mState = 7;
        }
        break;
    case 7:
        if (mStateTimer >= 120) {
            fn_8022149C(1);
        }
        break;
    }
}

void cls_804A6C38::vfunc_0x88() {
    if (mState == 2) {
        fn_80221250();
    } else {
        EnemyBase_804A4808::vfunc_0x88();
    }
}

void cls_804A6C38::vfunc_0x6C() {
    ShadowLinkBase_804A6A00::vfunc_0x6C();
    releaseShot(this);
}
