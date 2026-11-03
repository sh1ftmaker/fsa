#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/act/tu_8020FE50.h"

static inline bool isTiny(const JGeometry::TVec3f& v) {
    return v.squared() <= 32.0f * FLT_EPSILON;
}

void cls_804A6100::draw() {
    if (isTiny(field_0x54) == false) {
        s32 arg = 0;
        if (mState == 2) {
            arg = (s32)field_0x2a8;
        }
        fn_80210630(0, 0, 0, arg);
    }
}

void cls_804A6100::vfunc_0xA0() {
    JGeometry::TBox2f box;
    fn_80210A30(&box);
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    lbl_8053AB10->field_0x24->fn_8030C058(&box);
}

bool cls_804A6100::vfunc_0x60() {
    if (mBox.f.x - mBox.i.x > 0.0f && mBox.f.y - mBox.i.y > 0.0f) {
        s32 serial = fn_801F856C()->fn_801F82CC(this, field_0x195, 32.0f);
        if (serial >= 0 && serial != mSerial) {
            ActorBase_804A5770* other = fn_801F856C()->fn_801F666C(serial);
            if (other != NULL) {
                if (other->mActorId == ACTOR_SWBM) {
                    fn_801CB538(field_0x4, &other->mPos, 0, 0);
                }
                other->field_0x164 = 1;
            }
        }
    }
    return false;
}

void cls_804A6100::vfunc_0x90(u8 mapHit) {
    switch (mState) {
    case 0:
        if (mStateTimer >= 60) {
            mStateTimer = 0;
            mState = 1;
        }
        break;
    case 1:
        if (field_0x54.x != 3.0f) {
            fn_801CD950(&field_0x54.x, 3.0f, 0.004f);
            field_0x54.y = field_0x54.x;
            field_0x54.z = field_0x54.x;
            fn_80210990(field_0x98, &mBox);
            mBox2.set(mBox);
        } else {
            mStateTimer = 0;
            mState = 2;
        }
        break;
    case 2:
        if (fn_80210A6C()) {
            fn_80341CB0(field_0x4, &mPos);
            s32 bit = field_0x2ac;
            if (bit != 0) {
                fn_802D6A9C()->fn_802D8050(bit, true, 0xFF);
            }
            fn_801F3668();
        }
        break;
    }
    if (mAlive) {
        f32 y = mPos.y + field_0x54.y * fn_80210AF8(field_0x98);
        f32 x = mPos.x + field_0x54.x * fn_80210B34(field_0x98);
        JGeometry::TVec3f pos(x, y, 0.0f);
        fn_8013CB44(mEmitter, &pos, 0, 2, true);
        fn_801F0E34(0x50026, 0);
    }
}

void cls_804A6100::init() {
    cls_804A61C8::init();
    field_0x98 = field_0x90 & 3;
    field_0x2ac = (field_0x90 >> 2) & 0x1F;
    field_0x2b0 = ((field_0x90 >> 7) & 1) ? 0x1C1 : 0x1A8;
    fn_80210990(field_0x98, &mBox);
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0xb0 = 0x40004;
    mFlags |= 0x380;
    mLife = 1000;
    mLifeBackup = mLife;
    mState = 0;
    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    lbl_8053AED0->fn_8043D67C(field_0x2b0, 0, 0);
    f32 y = mPos.y + field_0x54.y * fn_80210AF8(field_0x98);
    f32 x = mPos.x + field_0x54.x * fn_80210B34(field_0x98);
    JGeometry::TVec3f pos(x, y, 0.0f);
    mEmitter = fn_8013CC50(field_0x4, &pos, 0x260, 0, 2, NULL, NULL, true, 1.0f);
}

cls_804A6100::~cls_804A6100() {
    fn_801EE434(mEmitter);
}

cls_804A6100::cls_804A6100() {
    field_0x2ac = 0;
    field_0x2b0 = 0;
    mEmitter = NULL;
}
