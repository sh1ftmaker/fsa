#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/GameMath.h"
#include "JSystem/JMath/JMath.h"
#include "game/act/act_slime_manager.h"
#include "game/act/tu_80207548.h"
#include "game/act/tu_80209C14.h"

// FIXME(shotgun): 99.83 %, reloc-name
cls_804A5D60::cls_804A5D60() : mTargetSerial(-1), field_0x2ac(0), field_0x2b0(1), field_0x2b1(0) {
    field_0x2b4 = 0.0f;
    field_0x2b8 = 0.0f;
    field_0x2bc = 0.0f;
    field_0x2c0.set(0.0f, 0.0f);
    field_0x2c8.set(0.0f, 0.0f);
}

// The animation tables (.data 0x804A5D38), not written yet.
extern const s16* lbl_804A5D38[];

// FIXME(shotgun): 86.98 %, missing/extra + reloc-name
void cls_804A5D60::init() {
    EnemyBase_804A4808::init();
    mBox.i.set(1.0f, 1.0f);
    mBox.f.set(-1.0f, -1.0f);
    mBox2.set(mBox);
    field_0x80.i.set(1.0f, 1.0f);
    field_0x80.f.set(-1.0f, -1.0f);
    field_0xb0 = 0x50303;
    field_0x98 = 2;
    mFlags |= ENEMY_FLAG_NO_HURT;
    field_0x194 = 1;
    mTargetSerial = field_0x90;
    cls_804A5BD8* manager = static_cast<cls_804A5BD8*>(getActorManager_801F856C()->findActorBySerial_801F666C(mTargetSerial));
    mAnim.fn_801D2608(lbl_804A5D38, 10);
    switch (manager->field_0x340) {
    case 0:
        mState = 6;
        mAnim.startAnim_801D22DC(5, 2, false);
        mBox.i.set(-8.0f, -8.0f);
        mBox.f.set(8.0f, 8.0f);
        mBox2.set(mBox);
        break;
    case 1:
    case 2:
        mState = 0;
        mAnim.startAnim_801D22DC(0, 0, false);
        startSound_801F0E34(0x50037, 0);
        break;
    case 3: {
        mState = 3;
        mAnim.startAnim_801D22DC(7, 0, false);
        startSound_801F0E34(0x50037, 0);
        s32 player = fn_801D05F8(&mPos, 0.0f);
        s16 angle;
        if (player >= 0) {
            angle = fn_801D0280(player) - 0x8000;
        } else {
            angle = fn_801CD664(0x10000);
        }
        field_0x2b4 = JMASSin(angle) * 1.5f;
        field_0x2b8 = JMASCos(angle) * 1.5f;
        field_0x2bc = 0.5f * (50.0f * (0.3f * -lbl_8053EFB8));
        field_0x3c.x = field_0x2b4;
        field_0x3c.y = field_0x2b8;
        field_0x3c.z = field_0x2bc;
        break;
    }
    }
    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
}

void cls_804A5D60::vfunc_0x54() {
    JGeometry::TVec2f offset(mSpawnPos.x - mPos.x, mSpawnPos.y - mPos.y);
    field_0x2c0 = offset;
}

// FIXME(shotgun): 78.11 %, missing/extra + reloc-name
// 0x8020AA78: enters `state`, starting its animation and
// velocity. While bit 0x2000 is set only 8, 11 and 12 are taken.
void cls_804A5D60::fn_8020AA78(s32 state) {
    if (mFlags & 0x2000) {
        if (state < 11) {
            if (state != 8) {
                return;
            }
        } else if (state >= 13) {
            return;
        }
    }
    switch (state) {
    case 1:
        mAnim.startAnim_801D22DC(1, 0, false);
        field_0x2b4 = 0.0f;
        field_0x2b8 = 0.0f;
        field_0x2bc = 0.0f;
        break;
    case 2:
        mAnim.startAnim_801D22DC(4, 0, false);
        break;
    case 4:
        mAnim.startAnim_801D22DC(8, 0, false);
        break;
    case 5:
        mAnim.startAnim_801D22DC(1, 0, false);
        field_0x2b4 = 0.083333336f * field_0x2c0.x;
        field_0x2b8 = 0.083333336f * field_0x2c0.y;
        field_0x2bc = 0.5f * (12.0f * -lbl_8053EFB8);
        break;
    case 6:
        mAnim.fn_801D22A0(5, 2);
        break;
    case 7:
        mFlags &= ~0x4000;
        field_0x98 = field_0x2ac;
        mAnim.startAnim_801D22DC(1, 0, false);
        field_0x2b4 = 0.0f;
        field_0x2b8 = 0.0f;
        switch (field_0x98) {
        case 0:
            field_0x2b4 = 1.4f;
            field_0x2b8 = 0.0f;
            break;
        case 1:
            field_0x2b4 = -1.4f;
            field_0x2b8 = 0.0f;
            break;
        case 2:
            field_0x2b4 = 0.0f;
            field_0x2b8 = 1.4f;
            break;
        case 3:
            field_0x2b4 = 0.0f;
            field_0x2b8 = -1.4f;
            break;
        }
        field_0x2bc = 0.5f * (12.0f * -lbl_8053EFB8);
        field_0x2b1 = field_0x2b0;
        if (field_0x2b1 < 1) {
            field_0x2b1 = 1;
        }
        break;
    case 8: {
        mFlags &= ~0x2000;
        mAnim.startAnim_801D22DC(1, 0, false);
        s32 player = fn_801D05F8(&mPos, 0.0f);
        s16 angle;
        if (player >= 0) {
            angle = fn_801D0280(player) - 0x8000;
        } else {
            angle = fn_801CD664(0x10000);
        }
        field_0x2b4 = JMASSin(angle) * 4.0f;
        field_0x2b8 = JMASCos(angle) * 4.0f;
        field_0x2bc = 0.5f * (12.0f * -lbl_8053EFB8);
        field_0x2b1 = 3;
        break;
    }
    case 9:
        mFlags &= ~0x8000;
        mAnim.startAnim_801D22DC(1, 0, false);
        field_0x2b4 = 0.045454547f * (field_0x2c8.x - mPos.x);
        field_0x2b8 = 0.045454547f * (field_0x2c8.y - mPos.y);
        field_0x2bc = 0.5f * (22.0f * -lbl_8053EFB8);
        break;
    case 10:
        mAnim.startAnim_801D22DC(4, 0, false);
        break;
    case 11:
        mAnim.startAnim_801D22DC(9, 0, false);
        startSound_801F0E34(0x50038, 0);
        mBox.i.set(1.0f, 1.0f);
        mBox.f.set(-1.0f, -1.0f);
        mBox2.set(mBox);
        break;
    case 12:
        break;
    }
    mStateTimer = 0;
    mState = state;
}

// The animation steps of the box-shrinking state machine; vfunc_0x90 is its per-frame step.
// FIXME(shotgun): 92.36 %, missing/extra + reloc-name
void cls_804A5D60::vfunc_0x90(u8 mapHit) {
    if (mFlags & 0x2000) {
        if (!(mState == 8 || mState == 11 || mState == 12)) {
            if (!(mAnim.mAnim == 6 || mAnim.mAnim == 2)) {
                fn_8020AA78(11);
            }
        }
    }
    switch (mState) {
    case 0:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            mBox.i.set(-8.0f, -8.0f);
            mBox.f.set(8.0f, 8.0f);
            mBox2 = mBox;
            fn_8020AA78(1);
        }
        break;
    case 1:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            switch (mAnim.mAnim) {
            case 1:
                mAnim.startAnim_801D22DC(2, 0, false);
                break;
            case 2:
                mAnim.startAnim_801D22DC(3, 0, false);
                break;
            case 3:
                fn_8020AA78(2);
                break;
            }
        }
        break;
    case 2:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            ActorBase_804A5770* manager = getActorManager_801F856C()->findActorBySerial_801F666C(mTargetSerial);
            if (manager != NULL) {
                static_cast<cls_804A5BD8*>(manager)->fn_80207ECC();
            }
            fn_8020AA78(7);
        }
        break;
    case 3:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            fn_8020AA78(4);
        }
        break;
    case 4:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            fn_8020AA78(5);
        }
        break;
    case 5:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            switch (mAnim.mAnim) {
            case 1:
                mAnim.startAnim_801D22DC(2, 0, false);
                break;
            case 2:
                mAnim.startAnim_801D22DC(3, 0, false);
                break;
            case 3: {
                mBox.i.set(-8.0f, -8.0f);
                mBox.f.set(8.0f, 8.0f);
                mBox2 = mBox;
                ActorBase_804A5770* manager = getActorManager_801F856C()->findActorBySerial_801F666C(mTargetSerial);
                if (manager != NULL) {
                    static_cast<cls_804A5BD8*>(manager)->fn_80207ECC();
                }
                fn_8020AA78(7);
                break;
            }
            }
        }
        break;
    case 6:
        if (mAnim.mAnim == 5 && mAnim.fn_801D217C()) {
            if (mFlags & 0x4000) {
                fn_8020AA78(7);
            } else if (mFlags & 0x8000) {
                fn_8020AA78(9);
            }
        }
        break;
    case 7:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            switch (mAnim.mAnim) {
            case 1:
                mAnim.startAnim_801D22DC(2, 0, false);
                startSound_801F0E34(0x50036, 0);
                break;
            case 2:
                mAnim.startAnim_801D22DC(3, 0, false);
                break;
            case 3:
                mAnim.startAnim_801D22DC(4, 0, false);
                break;
            case 4:
                field_0x2b1--;
                if (field_0x2b1 == 0) {
                    ActorBase_804A5770* manager = getActorManager_801F856C()->findActorBySerial_801F666C(mTargetSerial);
                    if (manager != NULL) {
                        if (static_cast<cls_804A5BD8*>(manager)->fn_80207E58()) {
                            fn_8020AA78(11);
                        } else {
                            fn_8020AA78(6);
                        }
                    } else {
                        fn_8020AA78(11);
                    }
                } else {
                    mAnim.startAnim_801D22DC(1, 0, false);
                }
                break;
            }
        }
        break;
    case 8:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            switch (mAnim.mAnim) {
            case 1:
                mAnim.startAnim_801D22DC(2, 0, false);
                startSound_801F0E34(0x50036, 0);
                break;
            case 2:
                mAnim.startAnim_801D22DC(3, 0, false);
                break;
            case 3:
                mAnim.startAnim_801D22DC(4, 0, false);
                break;
            case 4:
                field_0x2b1--;
                if (field_0x2b1 == 0) {
                    fn_8020AA78(11);
                } else {
                    mAnim.startAnim_801D22DC(1, 0, false);
                }
                break;
            }
        }
        break;
    case 9:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            switch (mAnim.mAnim) {
            case 1:
                mAnim.startAnim_801D22DC(6, 0, false);
                startSound_801F0E34(0x50039, 0);
                break;
            case 6:
                mAnim.startAnim_801D22DC(3, 0, false);
                break;
            case 3:
                fn_8020AA78(10);
                break;
            }
        }
        break;
    case 10:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            ActorBase_804A5770* manager = getActorManager_801F856C()->findActorBySerial_801F666C(mTargetSerial);
            if (manager != NULL) {
                static_cast<cls_804A5BD8*>(manager)->fn_80207E24();
            }
            fn_8020AA78(6);
        }
        break;
    case 11:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            fn_8020AA78(12);
        }
        break;
    case 12:
        if (mStateTimer >= 12) {
            ActorBase_804A5770* manager = getActorManager_801F856C()->findActorBySerial_801F666C(mTargetSerial);
            if (manager != NULL) {
                manager->mAlive = false;
            }
            mAlive = false;
        } else {
            f32 left = (f32)(12 - mStateTimer);
            if (left < JGeometry::TUtilf::epsilon()) {
                field_0x54.x = 0.0f;
            } else {
                field_0x54.x = (1.0f / left) * (0.0f - field_0x54.x) + field_0x54.x;
            }
            field_0x54.y = field_0x54.x;
            field_0x54.z = field_0x54.x;
        }
        break;
    }
}

// FIXME(shotgun): 95.10 %, missing/extra + reloc-name
void cls_804A5D60::vfunc_0x88() {
    switch (mState) {
    case 3:
        field_0x3c.z += 0.3f * lbl_8053EFB8;
        break;
    case 1:
    case 5:
    case 7:
    case 8:
    case 9:
        if (mAnim.mAnim == 6 || mAnim.mAnim == 2) {
            field_0x3c.x = field_0x2b4;
            field_0x3c.y = field_0x2b8;
        } else {
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.0f;
        }
        field_0x3c.z += lbl_8053EFB8;
        break;
    default:
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        vfunc_0x84();
        break;
    }
}

// FIXME(shotgun): 97.38 %, missing/extra + reloc-name
bool cls_804A5D60::vfunc_0x58() {
    if (mBox2.f.x - mBox2.i.x <= 0.0f || mBox2.f.y - mBox2.i.y <= 0.0f) {
        return false;
    }
    s32 id = -1;
    bool arrow = false;
    bool blocked = false;
    JGeometry::TBox2f box;
    JGeometry::TVec2f probe;
    fn_801F317C(&box);
    s32 player = fn_8023AE34(&box, field_0x4, &probe);
    if (player >= 0) {
        arrow = fn_801CC820(player, NULL);
    } else {
        player = fn_8023AA18(&box, field_0x4, &probe);
        if (player >= 0) {
            blocked = true;
        } else {
            player = fn_8023AC74(&box, field_0x4, &probe);
            if (player >= 0) {
                fn_801CC538(player, &probe, NULL, &id);
            } else {
                player = fn_8023AD54(&box, field_0x4, &probe);
                if (player >= 0) {
                    fn_801CC640(player, &id, NULL);
                }
            }
        }
    }
    bool hit = false;
    if (player >= 0) {
        if (blocked) {
            if (field_0x1dc.fn_801EAC20(player)) {
                hit = true;
            }
        } else if (!arrow) {
            if (id >= 0 && field_0x1dc.fn_801EAC54(id)) {
                hit = true;
            }
        } else {
            hit = true;
        }
        if (hit) {
            JGeometry::TVec3f at(probe.x, probe.y, 0.1f);
            fn_801CB538(field_0x4, &at, 0, 0);
        }
    }
    return hit;
}

u8 cls_804A5D60::vfunc_0x60() {
    fn_801F2DB4(mActorId, 'NULL', NULL);
    return 0;
}

// FIXME(shotgun): 97.21 %, missing/extra + reloc-name
u8 cls_804A5D60::vfunc_0x5C() {
    if (mBox.f.x - mBox.i.x > 0.0f && mBox.f.y - mBox.i.y > 0.0f) {
        s32 flag;
        s32 player = fn_801F285C(NULL, &flag, true, -1);
        if (player >= 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(mTargetSerial);
            if (actor != NULL) {
                const JGeometry::TVec3f* pos = getPlayerPos_8023E724(player);
                s32 type = fn_8023B6CC(player);
                static_cast<cls_804A5B28*>(actor)->fn_80207830(field_0x94, player, type, pos, flag);
            }
        }
    }
    return 0;
}

void cls_804A5D60::vfunc_0x74() {
    mFlags |= ENEMY_FLAG_NO_HURT;
}

void cls_804A5D60::vfunc_0x24() {
    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(mTargetSerial);
    if (actor != NULL) {
        static_cast<cls_804A5B28*>(actor)->fn_802075D8(field_0x94);
    }
    ActorBase_804A5770::vfunc_0x24();
}

// FIXME(shotgun): 73.03 %, missing/extra + reloc-name
void cls_804A5D60::draw() {
    bool tiny = field_0x54.squared() <= 32.0f * JGeometry::TUtilf::epsilon();
    if (!tiny) {
        SpriteParams_801F5AE0 params;
        Mtx mtx;
        mtx[0][0] = field_0x54.x;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[0][3] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = field_0x54.y;
        mtx[1][2] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = field_0x54.z;
        mtx[2][3] = 0.0f;
        params.mMtx = mtx;
        params.field_0x1c = 0;
        params.field_0x1d = 0;
        s32 frame = mAnim.mCel + 0x2C9;
        if (mFlags & ENEMY_FLAG_NO_HURT) {
            if (mSpriteParams.mMtx == NULL) {
                fn_801EF830(field_0x4, &mPos, &params, 7, "enemy_gc_sob.bin", frame, NULL);
            }
            if (field_0x4 < 8) {
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
            }
        } else {
            drawSprite_801F06F0("enemy_gc_sob.bin", frame, &params, 7, 1.0f, 1.0f);
        }
    }
}

void cls_804A5D60::fn_80209D44() {
    mFlags |= 0x2000;
}

// FIXME(shotgun): 87.22 %, missing/extra (the original re-reads mFlags)
void cls_804A5D60::fn_80209D20(s32 dir, u8 steps) {
    if (!(mFlags & 0x2000)) {
        mFlags |= 0x4000;
        field_0x2ac = dir;
        field_0x2b0 = steps;
    }
}

// FIXME(shotgun): 70.82 %, missing/extra (the original re-reads mFlags)
void cls_804A5D60::fn_80209CF4(const JGeometry::TVec2f* target) {
    if (!(mFlags & 0x2000)) {
        mFlags |= 0x8000;
        field_0x2c8 = *target;
    }
}

void cls_804A5D60::fn_80209CA4(s32 player, s32 attack, const JGeometry::TVec3f* from, s32 type) {
    if (fn_801F37AC(attack, from, 0, type, player)) {
        mFlags &= ~ENEMY_FLAG_NO_HURT;
    }
}

cls_804A5D60::~cls_804A5D60() {}
