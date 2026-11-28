#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/ViewLayer.h"
#include "game/act/act_zol_small_slime_8020C0A8.h"
#include "JSystem/JMath/JMath.h"
#include <float.h>

// The animation table: pairs of frame and duration, ended by -1.
static const s16 lbl_8046A398[] = { 4, 6, 7, 4, 8, 6, 7, 4, -1, -1 };

// The actor's animations (one), ended by NULL.
static const s16* lbl_8053A178[2] = { lbl_8046A398, NULL };

// The actor ids isBlocked_0x4C ignores, ended by 'NULL'.
static const u32 lbl_8053F9E0[2] = { 'TOMI', 'NULL' };

ZolSmallSlime_804A5F40::ZolSmallSlime_804A5F40() {
    field_0x2a8 = -1;
    field_0x2ac = 0;
    field_0x2ae = 0;
}

void ZolSmallSlime_804A5F40::init() {
    EnemyBase_804A4808::init();
    if (mSpawnParam_0x90 & 1) {
        mFlags |= 0x2000;
    }
    field_0xb0 = 0x31101;
    if (mDir_0x98 == 3) {
        if (fn_801CF144()) {
            mDir_0x98 = pickNewDir_801CF720(3);
        }
    }
    field_0x2ae = dirToAngle_801F6874(mDir_0x98);
    field_0x194 = 1;
    mState = 0;
    mStateTimer = 0;
    field_0x250 = randInt_801CD664(0x3D) + 0x28;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mAnim.setKeyTables_801D2608(lbl_8053A178, 1);
    mAnim.startAnim_801D22DC(0, 2, false);
    mAnim.setRandomFrame_801D1D60();
    if (mFlags & 0x2000) {
        if (isBlocked_0x4C(mLayer_0x4, &mPos)) {
            markDead_801F3668();
        } else {
            JGeometry::TBox2f* box = layerBox(mLayer_0x4);
            JGeometry::TBox2f bounds(mMapBox_0x80.i.x + mPos.x, mMapBox_0x80.i.y + mPos.y,
                                     mMapBox_0x80.f.x + mPos.x, mMapBox_0x80.f.y + mPos.y);
            bool inside = false;
            if (bounds.i.x >= box->i.x && bounds.i.y >= box->i.y &&
                bounds.f.x < box->f.x && bounds.f.y < box->f.y) {
                inside = true;
            }
            if (!inside) {
                markDead_801F3668();
            }
        }
    }
}

// FIXME: 99.40 %, regalloc-only; behaviour clean.
void ZolSmallSlime_804A5F40::fn_8020C6E8() {
    s32 nearest = -1;
    if (mFlags & 0x2000) {
        f32 best = FLT_MAX;
        for (s32 i = 0; i < 4; i++) {
            if (!isPlayerActive_8023F264(i)) {
                continue;
            }
            if (fn_8022D75C(i)) {
                continue;
            }
            if (fn_8022F478(i)) {
                continue;
            }
            if (getPlayerLayer_8023DE58(i) != getField_0x4()) {
                continue;
            }
            f32 dist = getPlayerDistSq_801D0DA0(i, &mPos);
            if (dist < best) {
                best = dist;
                nearest = i;
            }
        }
    }
    if (mState == 2) {
        if (nearest < 0) {
            field_0x250 = randInt_801CD664(0x3D) + 0x28;
            mTargetSpeed = 0.0f;
            field_0x2a8 = -1;
            field_0x2ac = 0;
            mStateTimer = 0;
            mState = 0;
        } else if (field_0x2ac == 0) {
            if (field_0x2a8 != nearest) {
                field_0x2a8 = nearest;
                field_0x2ac = 90;
            }
        } else {
            field_0x2ac--;
        }
    } else {
        if (nearest >= 0) {
            field_0x2a8 = nearest;
            field_0x2ac = 90;
            mTargetSpeed = 0.125f;
            mStateTimer = 0;
            mState = 2;
        } else {
            field_0x2ac = 0;
        }
    }
}

void ZolSmallSlime_804A5F40::updateState_0x90(u8 mapHit) {
    fn_8020C6E8();
    switch (mState) {
    case 0:
        if (mStateTimer >= field_0x250) {
            field_0x250 = randInt_801CD664(0x29) + 0x3C;
            mTargetSpeed = 0.4f;
            field_0x2a8 = -1;
            field_0x2ac = 0;
            if (fn_801CF144()) {
                mDir_0x98 = pickNewDir_801CF720(3);
            } else {
                mDir_0x98 = fn_801CF6B8();
            }
            field_0x2ae = dirToAngle_801F6874(mDir_0x98);
            mStateTimer = 0;
            mState = 1;
        }
        break;
    case 1:
        if (mapHit) {
            if (fn_801CF144()) {
                if (mDir_0x98 == 3) {
                    mDir_0x98 = pickNewDir_801CF720(3);
                } else {
                    mDir_0x98 = fn_801CF798(3, mDir_0x98);
                }
            } else {
                mDir_0x98 = pickNewDir_801CF720(mDir_0x98);
            }
            field_0x250 += 10;
        } else if (mStateTimer >= field_0x250) {
            if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.35f) {
                field_0x250 = randInt_801CD664(0x3D) + 0x28;
                mTargetSpeed = 0.0f;
                field_0x2a8 = -1;
                field_0x2ac = 0;
                mStateTimer = 0;
                mState = 0;
            } else {
                field_0x250 = randInt_801CD664(0x29) + 0x3C;
                mTargetSpeed = 0.4f;
                field_0x2a8 = -1;
                field_0x2ac = 0;
                if (fn_801CF144()) {
                    mDir_0x98 = pickNewDir_801CF720(3);
                } else {
                    mDir_0x98 = fn_801CF6B8();
                }
                field_0x2ae = dirToAngle_801F6874(mDir_0x98);
                mStateTimer = 0;
                mState = 1;
            }
        }
        field_0x2ae = dirToAngle_801F6874(mDir_0x98);
        break;
    case 2:
        field_0x2ae = getAngleToPlayer_801D0280(field_0x2a8) - 0x8000;
        if (viewLayerFlag(mLayer_0x4)) {
            if (!(isOnGround_801D0DD4() && fn_801CC9F8(&mPos, mLayer_0x4))) {
                if (field_0x2ae > 0) {
                    field_0x2ae = 0x4000;
                } else {
                    field_0x2ae = -0x4000;
                }
            }
        }
        break;
    }
    if (isOnGround_801D0DD4() && mState != 0) {
        if (mAnim.isLoopEnd_801D217C()) {
            startSound_801F0E34(0x5004F, 0);
        }
    }
}

// FIXME: 98.04 %, regalloc-only; behaviour clean.
// FIXME(shotgun): register allocation differs (f0/f1/f2 assignment).
void ZolSmallSlime_804A5F40::calcVelocity_0x88() {
    f32 sinV = JMath::sincosTable_.sinShort(field_0x2ae);
    f32 cosV = JMath::sincosTable_.cosShort(field_0x2ae);
    f32 x = sinV * mSpeed;
    f32 y = cosV * mSpeed;
    mVel_0x3c.x = x;
    mVel_0x3c.y = y;
    applyGravity_0x84();
}

void ZolSmallSlime_804A5F40::move_0x98() {
    if (!(mFlags & 7)) {
        fn_801CE32C();
    } else if (viewLayerFlag(mLayer_0x4)) {
        fn_801CE5D4();
    }
}

bool ZolSmallSlime_804A5F40::isBlocked_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    bool result;
    s32 serial;
    const u32* ignoreIds = NULL;
    if (mFlags & 7) {
        ignoreIds = lbl_8053F9E0;
    }
    result = true;
    if (!fn_802265DC(pos, layer, false)) {
        serial = mSerial;
        CollisionBoxes_8020D58C* boxes = fn_8020D58C();
        if (!boxes->mLists[boxes->field_0x1].containsPointOfOther_8020D6AC(layer, pos, serial, ignoreIds)) {
            result = false;
        }
    }
    return result;
}

void ZolSmallSlime_804A5F40::draw() {
    SpriteParams_801F5AE0 params;
    params.field_0x0[2] = 4;
    drawSprite_801F06F0("enemy_gc_sob.bin", mAnim.mCel + 0x2C9, &params, 0, 1.0f, 1.0f);
}

ZolSmallSlime_804A5F40::~ZolSmallSlime_804A5F40() {}
