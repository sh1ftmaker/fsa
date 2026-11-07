#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/act/act_quicksand.h"
#include "game/act/tu_80212E3C.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/SpriteParams.h"
#include "JSystem/JParticle/JPAEmitter.h"

// Sprite frame tables of the actor (.data 0x804A6330, not written as source here).
extern const s16* lbl_804A6330[];

// The dust puff both the mode-3 setup and the last frames of mState 6 start: scaled like the
// actor and tinted like the shadow circle.
static inline void spawnDust(cls_804A6340* self) {
    JPABaseEmitter* emitter = spawnEmitter_8013CC50(self->field_0x4, &self->mPos, 0x1DA, 0, 0, NULL, NULL, true, 1.0f);
    if (emitter != NULL) {
        emitter->setGlobalScale(self->field_0x54);
        JUtility::TColor color = gGameRoot2_8053AB10->field_0x2c->field_0x4;
        emitter->setGlobalPrmColor(color.r, color.g, color.b);
    }
}

cls_804A6340::cls_804A6340() {}

// FIXME(shotgun): 96.48 %, missing/extra + reloc-name.
void cls_804A6340::init() {
    EnemyBase_804A4808::init();
    mBox.i.x = 1.0f;
    mBox.i.y = 1.0f;
    mBox.f.x = -1.0f;
    mBox.f.y = -1.0f;
    mBox2.i.x = 2.0f;
    mBox2.i.y = 2.0f;
    mBox2.f.x = 8.0f;
    mBox2.f.y = 8.0f;
    field_0x80.i.x = 1.0f;
    field_0x80.i.y = 1.0f;
    field_0x80.f.x = -1.0f;
    field_0x80.f.y = -1.0f;
    field_0xb0 = 0;
    field_0x98 = 2;
    mFlags |= 0x80;
    mPos.y -= 2.0f;
    mSpawnPos.x = mPos.x;
    mSpawnPos.y = mPos.y;
    mSpawnPos.z = mPos.z;
    mState = 0;
    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mAnim.fn_801D2608(lbl_804A6330, 4);
    mAnim.startAnim_801D22DC(1, 2, false);
}

// FIXME(shotgun): 85.98 %, missing/extra.
void cls_804A6340::fn_80213774(int mode) {
    switch (mode) {
    case 0:
    case 1:
        if (mode == 0) {
            mFlags &= ~0x4000;
        }
        mBox.i.x = 1.0f;
        mBox.i.y = 1.0f;
        mBox.f.x = -1.0f;
        mBox.f.y = -1.0f;
        break;
    case 2:
        mBox.i.x = 1.0f;
        mBox.i.y = 1.0f;
        mBox.f.x = -1.0f;
        mBox.f.y = -1.0f;
        if (mState == 0 || mState == 1) {
            field_0x250 = fn_801CD664(0x15) + 10;
        } else {
            field_0x250 = fn_801CD664(0x79) + 0x3C;
        }
        break;
    case 3:
        mBox.i.x = 1.0f;
        mBox.i.y = 1.0f;
        mBox.f.x = -1.0f;
        mBox.f.y = -1.0f;
        mAnim.startAnim_801D22DC(0, 0, false);
        spawnDust(this);
        startSound_801F0E34(0x50033, 0);
        break;
    case 4:
        mBox.i.x = -8.0f * field_0x54.x;
        mBox.i.y = -8.0f * field_0x54.y;
        mBox.f.x = 8.0f * field_0x54.x;
        mBox.f.y = 8.0f * field_0x54.y;
        field_0x250 = fn_801CD664(0x3D) + 0x1E;
        mAnim.fn_801D22A0(1, 2);
        startSound_801F0E34(0x50035, 0);
        break;
    case 5:
        mBox.i.x = -8.0f * field_0x54.x;
        mBox.i.y = -8.0f * field_0x54.y;
        mBox.f.x = 8.0f * field_0x54.x;
        mBox.f.y = 8.0f * field_0x54.y;
        mAnim.startAnim_801D22DC(2, 0, false);
        break;
    case 6:
        mBox.i.x = -8.0f * field_0x54.x;
        mBox.i.y = -8.0f * field_0x54.y;
        mBox.f.x = 8.0f * field_0x54.x;
        mBox.f.y = 8.0f * field_0x54.y;
        mAnim.startAnim_801D22DC(3, 0, false);
        break;
    }
    mStateTimer = 0;
    mState = mode;
    if (mState == 0) {
        field_0xb0 = 0;
    } else if (mBox.f.x - mBox.i.x > 0.0f && mBox.f.y - mBox.i.y > 0.0f) {
        if (mFlags & 0x2000) {
            field_0xb0 = 0x40004;
        } else {
            field_0xb0 = 0x11101;
        }
    } else if (mFlags & 0x2000) {
        field_0xb0 = 0;
    } else {
        field_0xb0 = 0x1000;
    }
}

// FIXME(shotgun): 95.18 %, missing/extra.
void cls_804A6340::vfunc_0x90(u8 mapHit) {
    if (mState >= 3 || mState < 0) {
        fn_801F2B20(0);
    }
    switch (mState) {
    case 1:
        if (mFlags & 0x4000) {
            fn_80213774(0);
        }
        break;
    case 2:
        if (mFlags & 0x4000) {
            fn_80213774(0);
        } else if (mStateTimer >= field_0x250) {
            fn_80213774(3);
        }
        break;
    case 3:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            if (mFlags & 0x4000) {
                fn_80213774(6);
            } else {
                fn_80213774(4);
            }
        }
        break;
    case 4:
        if (mFlags & 0x4000) {
            fn_80213774(6);
        } else if (mStateTimer >= field_0x250) {
            if (mAnim.fn_801D217C()) {
                startSound_801F0E34(0x50035, 0);
                fn_80213774(5);
            }
        }
        break;
    case 5:
        if (mFlags & 0x4000) {
            fn_80213774(6);
        } else if (mStateTimer > 1) {
            if (mAnim.mFrameCtrl.checkPass((f32)(mAnim.mLength - 2))) {
                if (fn_801CECF4('ARSH', 1.5f, 300.0f)) {
                    startSound_801F0E34(0x50031, 0);
                }
            }
            if (mAnim.fn_801D21F0()) {
                fn_80213774(6);
            }
        }
        break;
    case 6:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            if (mFlags & 0x4000) {
                fn_80213774(0);
            } else {
                fn_80213774(2);
            }
        } else if (mStateTimer == 0x10) {
            spawnDust(this);
            startSound_801F0E34(0x50034, 0);
        }
        break;
    }
}

// FIXME(shotgun): 88.45 %, missing/extra.
void cls_804A6340::vfunc_0x94() {
    if (mState == 4 && mAnim.mAnim == 1 && mAnim.fn_801D217C()) {
        startSound_801F0E34(0x50035, 0);
    }
    EnemyBase_804A4808::vfunc_0x94();
    if (mState == 3 || mState == 6) {
        if (mAnim.mCel == 0) {
            mBox.i.x = 1.0f;
            mBox.i.y = 1.0f;
            mBox.f.x = -1.0f;
            mBox.f.y = -1.0f;
        } else {
            mBox.i.x = -8.0f * field_0x54.x;
            mBox.i.y = -8.0f * field_0x54.y;
            mBox.f.x = 8.0f * field_0x54.x;
            mBox.f.y = 8.0f * field_0x54.y;
        }
        if (mState == 0) {
            field_0xb0 = 0;
        } else if (mBox.f.x - mBox.i.x > 0.0f && mBox.f.y - mBox.i.y > 0.0f) {
            if (mFlags & 0x2000) {
                field_0xb0 = 0x40004;
            } else {
                field_0xb0 = 0x11101;
            }
        } else if (mFlags & 0x2000) {
            field_0xb0 = 0;
        } else {
            field_0xb0 = 0x1000;
        }
    }
}

// FIXME(shotgun): 95.16 %, missing/extra + reloc-name.
void cls_804A6340::draw() {
    if (field_0x54.squared() > JGeometry::TUtilf::epsilon() && (mState >= 3 || mState < 0)) {
        s32 frame = mAnim.mCel + 0x13D;
        if (mFlags & 0x2000) {
            frame += 4;
        }
        SpriteParams_801F5AE0 params;
        Mtx mtx;
        mtx[0][3] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][3] = 0.0f;
        mtx[0][0] = field_0x54.x;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = field_0x54.y;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = field_0x54.z;
        params.mMtx = mtx;
        if (mFlags & 0x2000) {
            if (field_0x4 < 8) {
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
            }
        } else {
            s32 flags = -1;
            if (mPos.z > 0.0f) {
                flags = 7;
            }
            drawSprite_801F06F0("enemy_gc_sob.bin", frame, &params, flags, 1.0f, 1.0f);
        }
    }
}

void cls_804A6340::vfunc_0x44() {
    if (field_0xd4 == 1 && mState != 0 && !(mFlags & 0x4000)) {
        fn_80213774(4);
    }
}

void cls_804A6340::vfunc_0x24() {
    if ((s32)field_0x90 >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C((s32)field_0x90);
        if (actor != NULL) {
            static_cast<cls_804A6270*>(actor)->fn_80210D90();
        }
    }
    ActorBase_804A5770::vfunc_0x24();
}

void cls_804A6340::fn_80212F5C() {
    mPos.y -= 1.0f;
    mSpawnPos.x = mPos.x;
    mSpawnPos.y = mPos.y;
    mSpawnPos.z = mPos.z;
    mFlags |= 0x2200;
    mLife = 1000;
    mLifeBackup = mLife;
    field_0x54.x = 1.4f;
    field_0x54.y = 1.4f;
    field_0x54.z = 1.4f;
    mBox2.i.x *= field_0x54.x;
    mBox2.f.x *= field_0x54.x;
    mBox2.i.y *= field_0x54.y;
    mBox2.f.y *= field_0x54.y;
}

void cls_804A6340::fn_80212F14() {
    if (mFlags & 0x4000) {
        fn_80213774(0);
    } else if (mState != 1) {
        fn_80213774(1);
    }
}

void cls_804A6340::fn_80212ECC() {
    if (mFlags & 0x4000) {
        fn_80213774(0);
    } else if (mState != 2) {
        fn_80213774(2);
    }
}

cls_804A6340::~cls_804A6340() {}
