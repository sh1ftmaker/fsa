#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/ViewLayer.h"
#include "game/act/tu_801DE714.h"

extern u8 lbl_8053AC30;
// The reaction both hit tests run once the actor was hit (inlined in the original): back to the
// chase state from the grab, else the first hit lets go of the actor it carries.
static inline void releaseTarget(cls_804A51E4* self) {
    if (self->mState == 0xC && self->mStateTimer >= 6 && !self->fn_801E1DE8()) {
        self->fn_801E1594(0xD);
    } else if (self->fn_801E1CF8()) {
        if (!(self->mFlags & 0x10000)) {
            self->mFlags |= 0x10000;
            self->startSound_801F0E34(0x4003D, 0);
            lbl_8053AC30++;
            if (lbl_8053AC30 >= 5) {
                lbl_8053AC30 = 0;
            }
            self->mFlags &= ~0x100000;
            if (!(self->mFlags & 0x80000)) {
                if (self->mTargetSerial >= 0) {
                    self->startSound_801F0E34(0x5001A, 0);
                    ActorBase_804A5770* target = getActorManager_801F856C()->findActorBySerial_801F666C(self->mTargetSerial);
                    if (target != NULL) {
                        target->field_0x94 = 0;
                        target->field_0x194 = 0;
                    }
                    self->mTargetSerial = -1;
                    self->field_0x2c0 = 0;
                    self->mFlags &= ~0x8000;
                }
            }
        }
    }
}

// FIXME(shotgun): 77.83 %, missing/extra; behaviour clean.
bool cls_804A51E4::fn_801E2F40() {
    bool result = false;
    s32 code = fn_8022461C(&mPos, field_0x4);
    if (code < 13) {
        if (code == 3) {
            result = true;
        }
    } else if (code < 15) {
        result = true;
    }
    return result;
}

// FIXME(shotgun): 44.70 %, scheduling; behaviour clean.
void cls_804A51E4::fn_801E2EE4() {
    JGeometry::TBox2f box(mPos.x - 8.0f, mPos.y - 8.0f, mPos.x + 8.0f, mPos.y + 8.0f);
    fn_802397B4(&box, field_0x4, mSerial);
}

void cls_804A51E4::fn_801E2D88() {
    switch (mState) {
    case 0:
    case 14:
        mBox.i.x = 1.0f;
        mBox.i.y = 1.0f;
        mBox.f.x = -1.0f;
        mBox.f.y = -1.0f;
        mBox2.i.x = mBox.i.x;
        mBox2.i.y = mBox.i.y;
        mBox2.f.x = mBox.f.x;
        mBox2.f.y = mBox.f.y;
        break;
    case 12:
        mBox.i.x = -8.0f;
        mBox.i.y = -8.0f;
        mBox.f.x = 8.0f;
        mBox.f.y = 8.0f;
        mBox2.i.x = -8.0f;
        mBox2.i.y = -20.0f;
        mBox2.f.x = 8.0f;
        mBox2.f.y = 8.0f;
        break;
    default:
        if (fn_801E1DB4()) {
            mBox.i.x = -12.0f;
            mBox.i.y = -40.0f;
            mBox.f.x = 12.0f;
            mBox.f.y = 4.0f;
        } else {
            mBox.i.x = -8.0f;
            mBox.i.y = -24.0f;
            mBox.f.x = 8.0f;
            mBox.f.y = 4.0f;
        }
        if (fn_801D0DD4()) {
            mBox.i.y += 4.0f;
            mBox.f.y += 4.0f;
        }
        mBox2.i.x = mBox.i.x;
        mBox2.i.y = mBox.i.y;
        mBox2.f.x = mBox.f.x;
        mBox2.f.y = mBox.f.y;
        break;
    }
}

// Whether a player-side object (arrow, bomb, sword, ...) hits the actor's box.
bool cls_804A51E4::vfunc_0x58() {
    bool arrow = false;
    bool direct = false;
    bool bomb = false;
    bool other = false;
    JGeometry::TVec3f arrowVel(0.0f, 0.0f, 0.0f);
    JGeometry::TBox2f box;
    fn_801F317C(&box);
    JGeometry::TVec2f pos2;
    JGeometry::TVec3f bombVel;
    s32 player = fn_8023AE34(&box, field_0x4, &pos2);
    if (player >= 0) {
        arrow = fn_801CC820(player, &arrowVel);
    } else {
        player = fn_8023AA18(&box, field_0x4, &pos2);
        if (player >= 0) {
            direct = true;
        } else {
            player = fn_8023AC74(&box, field_0x4, &pos2);
            if (player >= 0) {
                bomb = fn_801CC538(player, &pos2, &bombVel, NULL);
            } else {
                player = fn_8023AD54(&box, field_0x4, &pos2);
                if (player >= 0) {
                    other = fn_801CC640(player, NULL, &arrowVel);
                }
            }
        }
    }
    if (arrow || bomb || other) {
        releaseTarget(this);
    }
    bool hit = false;
    if (player >= 0) {
        JGeometry::TVec3f from = *getPlayerPos_8023E724(player);
        s32 type = 0;
        if (direct) {
            type = 4;
        } else if (bomb) {
            from = bombVel;
        } else {
            from = mPos - arrowVel;
            if (arrow) {
                type = 3;
            }
        }
        if (mFlags & 0x10000) {
            hit = fn_801F37AC(0, &from, 0, type, player);
        }
    }
    return hit;
}

// FIXME(shotgun): 79.85 %, different; behaviour drift.
u8 cls_804A51E4::vfunc_0x60() {
    u8 hit = 0;
    s32 other = getActorManager_801F856C()->fn_801F82CC(this, field_0x195, 32.0f);
    if (other >= 0 && other != mSerial) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(other);
        if (actor != NULL) {
            u8 landed;
            if (!viewLayerFlag(field_0x4) && mPos.z > 16.0f && fn_801CC8E4(actor)) {
                landed = 0;
            } else if (!fn_801E1CF8()) {
                landed = 1;
            } else {
                JGeometry::TVec3f dir(mPos.x - actor->field_0x3c.x, mPos.y - actor->field_0x3c.y, mPos.z - actor->field_0x3c.z);
                landed = fn_801F37AC(actor->field_0x118, &dir, 0, actor->field_0x168, -1);
                if (landed) {
                    hit = 1;
                }
            }
            if (landed) {
                actor->field_0x164 = 1;
                releaseTarget(this);
            }
        }
    }
    return hit;
}

// FIXME(shotgun): 94.58 %, missing/extra; behaviour near.
u8 cls_804A51E4::vfunc_0x5C() {
    u8 hit = 0;
    s32 player = fn_801F285C(NULL, NULL, true, -1);
    if (player >= 0) {
        if (fn_801E1CF8() && (fn_80235448(player) || fn_80235310(player))) {
            fn_801CB688(field_0x4, &mPos, 0, 0);
            releaseTarget(this);
            hit = 1;
        } else if (fn_801E1D7C()) {
            for (s32 i = 0; i < 4; i++) {
                if (fn_801F285C(NULL, NULL, true, i) == i) {
                    fn_8023B6CC(i);
                    fn_802379E4(i, &mPos);
                }
            }
            if (mState == 0xC && mStateTimer >= 6 && !fn_801E1DE8()) {
                fn_801E1594(0xD);
            }
            hit = 1;
        }
    }
    return hit;
}

u8 cls_804A51E4::vfunc_0x64() {
    u8 hit = 0;
    if (!(mFlags & 0x1000)) {
        JGeometry::TBox2f* box = layerBox(field_0x4);
        f32 upper = box->f.y - 8.0f;
        if (mPos.y > upper) {
            mPos.y = upper;
            hit = 1;
            mMapHitDir.x = 0.0f;
            mMapHitDir.y = -1.0f;
            mMapHitDir.z = 0.0f;
        } else {
            f32 lower = 24.0f + box->i.y;
            if (mPos.y < lower) {
                mPos.y = lower;
                hit = 1;
                mMapHitDir.x = 0.0f;
                mMapHitDir.y = 1.0f;
                mMapHitDir.z = 0.0f;
            }
        }
    }
    return hit;
}
