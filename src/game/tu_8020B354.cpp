#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Camera.h"
#include "game/Player.h"
#include "game/ViewLayer.h"
#include "JSystem/JMath/JMath.h"
#include "game/act/tu_80207548.h"
#include "game/act/tu_8020B354.h"

static const s16 lbl_8046A368[14] = {0, 3, 1, 3, 2, 3, 3, 3, 4, 3, 5, 3, -1, -1};
static const s16* lbl_8053A170[2] = {lbl_8046A368, NULL};

cls_804A5E98::~cls_804A5E98() {}

void cls_804A5E98::fn_8020B3E4(s32 player, s32 attack, const JGeometry::TVec3f* from, s32 type) {
    if (fn_801F37AC(attack, from, 0, type, player)) {
        mFlags &= ~ENEMY_FLAG_NO_HURT;
    }
}

// FIXME: 98.14. Regalloc only: the -1.0f and 2.4f literals and the two products are in other
// float registers (f2, f1 and f3, f1 in the target).
void cls_804A5E98::fn_8020B434() {
    if (mState == 1) {
        fn_801F0E34(0x400D8, 0);
        mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
        mBox2.set(mBox);
        s16 angle = (field_0x94 & 3) * 0x4000 - 0x6000;
        field_0x3c.set(JMath::sincosTable_.sinShort(angle) * 2.4f,
                       JMath::sincosTable_.cosShort(angle) * 2.4f, 0.0f);
        mStateTimer = 0;
        mState = 2;
    }
}

void cls_804A5E98::fn_8020B500() {
    if (mState == 0) {
        mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
        mBox2.set(mBox);
        mStateTimer = 0;
        mState = 1;
    }
}

void cls_804A5E98::draw() {
    if (mState == 0) {
        return;
    }
    SpriteParams_801F5AE0 params;
    params.field_0x1c = 0;
    params.field_0x1d = 0;
    s32 frame = mAnim.mCel + 0x2BF;
    if (!(mFlags & ENEMY_FLAG_NO_HURT)) {
        fn_801F06F0("enemy_gc_sob.bin", frame, &params, 7, 1.0f, 1.0f);
        return;
    }
    if (mSpriteParams.mMtx == NULL) {
        fn_801EF830(field_0x4, &mPos, &params, 7, "enemy_gc_sob.bin", frame, NULL);
    }
    if (field_0x4 < 8) {
        void* res = lbl_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
        lbl_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
    }
}

void cls_804A5E98::vfunc_0x24() {
    ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(mTargetSerial);
    if (actor != NULL) {
        static_cast<cls_804A5B28*>(actor)->fn_802075D8(field_0x94);
    }
    ActorBase_804A5770::vfunc_0x24();
}

// FIXME: 79.4. Same work as EnemyBase::fn_801CE010 but with 0.001f; the target computes the box
// pointer into r5 and the stack position's address into r4 before loading mPos (as if a member
// taking the out pointer were inlined), ours loads the camera pointer last and has 3 more moves.
void cls_804A5E98::vfunc_0xA0() {
    if (mState != 0) {
        s32 layer = field_0x4;
        GameView_804A9950* view = fn_802D6A9C();
        JGeometry::TBox2f* box = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
        JGeometry::TVec3f pos(mPos.x, mPos.y - mPos.z, 0.0f);
        f32 top = 0.001f + box->i.y;
        pos.y = top - pos.y >= 0.0f ? top : pos.y;
        lbl_8053AB10->field_0x24->fn_8030C210(&pos);
    }
}

void cls_804A5E98::vfunc_0x74() {
    mFlags |= ENEMY_FLAG_NO_HURT;
}

bool cls_804A5E98::vfunc_0x5C() {
    if (mBox.getWidth() > 0.0f && mBox.getHeight() > 0.0f) {
        s32 flag;
        s32 player = fn_801F285C(NULL, &flag, true, -1);
        if (player >= 0) {
            ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(mTargetSerial);
            if (actor != NULL) {
                static_cast<cls_804A5B28*>(actor)->fn_80207830(field_0x94, player, fn_8023B6CC(player),
                                                               fn_8023E724(player), flag);
            }
        }
    }
    return false;
}

bool cls_804A5E98::vfunc_0x60() {
    if (mBox.getWidth() > 0.0f && mBox.getHeight() > 0.0f) {
        s32 other = fn_801F856C()->fn_801F82CC(this, field_0x195, 32.0f);
        if (other >= 0 && other != mSerial) {
            ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(other);
            if (actor != NULL && !(!viewLayerFlag(field_0x4) && mPos.z > 16.0f && fn_801CC8E4(actor))) {
                ActorBase_804A5770* target = fn_801F856C()->fn_801F666C(mTargetSerial);
                if (target != NULL) {
                    JGeometry::TVec3f from(mPos);
                    from.sub(actor->field_0x3c);
                    static_cast<cls_804A5B28*>(target)->fn_80207830(
                        field_0x94, actor->field_0x110, actor->field_0x118, &from, actor->field_0x168);
                }
                actor->field_0x164 = 1;
            }
        }
    }
    return false;
}

bool cls_804A5E98::vfunc_0x58() {
    if (mBox2.getWidth() <= 0.0f || mBox2.getHeight() <= 0.0f) {
        return false;
    }
    JGeometry::TVec3f vel54;
    JGeometry::TVec3f vel48;
    JGeometry::TBox2f box;
    JGeometry::TVec3f from;
    JGeometry::TVec2f probe;
    s32 player;
    bool arrow = false;
    bool actorC = false;
    vel48.set(0.0f, 0.0f, 0.0f);
    fn_801F317C(&box);
    player = fn_8023AE34(&box, field_0x4, &probe);
    if (player >= 0) {
        arrow = fn_801CC820(player, &vel48);
    } else {
        player = fn_8023AC74(&box, field_0x4, &probe);
        if (player >= 0) {
            actorC = fn_801CC538(player, &probe, &vel54, NULL);
        } else {
            player = fn_8023AD54(&box, field_0x4, &probe);
            if (player >= 0) {
                fn_801CC640(player, NULL, &vel48);
            }
        }
    }
    bool hit = false;
    if (player >= 0) {
        hit = true;
        ActorBase_804A5770* target = fn_801F856C()->fn_801F666C(mTargetSerial);
        if (target != NULL) {
            from = *fn_8023E724(player);
            s32 type = 0;
            if (actorC) {
                from.set(vel54.x, vel54.y, vel54.z);
            } else {
                from.set(mPos - vel48);
                if (arrow) {
                    type = 3;
                }
            }
            static_cast<cls_804A5B28*>(target)->fn_80207830(field_0x94, player, 4, &from, type);
        }
    }
    return hit;
}

void cls_804A5E98::vfunc_0x88() {}

void cls_804A5E98::vfunc_0x90(u8 mapHit) {
    switch (mState) {
    case 1: // state 1 does nothing here
        break;
    case 2: {
        s32 layer = field_0x4;
        GameView_804A9950* view = fn_802D6A9C();
        JGeometry::TBox2f* box = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
        if (mPos.x < box->i.x - 32.0f || mPos.y < box->i.y - 32.0f || mPos.x > 32.0f + box->f.x ||
            mPos.y > 32.0f + box->f.y) {
            mAlive = false;
        }
        break;
    }
    }
}

void cls_804A5E98::vfunc_0x54() {
    mPos.x = mSpawnPos.x;
    mPos.y = mSpawnPos.y;
    mPos.z = mSpawnPos.z;
}

void cls_804A5E98::init() {
    EnemyBase_804A4808::init();
    mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
    mBox2.set(mBox);
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0xb0 = 0x50003;
    field_0x98 = 2;
    mFlags |= ENEMY_FLAG_NO_HURT;
    field_0x194 = 1;
    mTargetSerial = field_0x90;
    mState = 0;
    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mAnim.fn_801D2608(lbl_8053A170, 1);
    mAnim.fn_801D22DC(0, 2, false);
    s32 layer = field_0x4;
    GameView_804A9950* view = fn_802D6A9C();
    JGeometry::TBox2f* box = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
    mPos.set(0.5f * (box->i.x + box->f.x), 0.5f * (box->i.y + box->f.y), 0.0f);
}

cls_804A5E98::cls_804A5E98() {
    mTargetSerial = -1;
}
