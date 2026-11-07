#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/ViewLayer.h"
#include "game/act/act_shadow_link_2.h"

static const u32 lbl_8053EF18[2] = {'FLME', 'NULL'};

// FIXME(shotgun): 90 %, missing/extra + reloc-name
bool ShadowLink2_804A43E8::fn_801CAEF4(const JGeometry::TVec3f* pos, s32 layer) {
    bool hit = false;
    if (ActorBase_804A5770::vfunc_0x4C(layer, pos)) {
        hit = true;
    } else {
        JGeometry::TVec3f p(pos->x - 7.0f, pos->y - 7.0f, pos->z);
        if (ActorBase_804A5770::vfunc_0x4C(layer, &p)) {
            hit = true;
        } else {
            JGeometry::TVec3f q(7.0f + pos->x, pos->y - 7.0f, pos->z);
            if (ActorBase_804A5770::vfunc_0x4C(layer, &q)) {
                hit = true;
            } else {
                JGeometry::TVec3f r(pos->x - 7.0f, 7.0f + pos->y, pos->z);
                if (ActorBase_804A5770::vfunc_0x4C(layer, &r)) {
                    hit = true;
                } else {
                    JGeometry::TVec3f t(7.0f + pos->x, 7.0f + pos->y, pos->z);
                    if (ActorBase_804A5770::vfunc_0x4C(layer, &t)) {
                        hit = true;
                    }
                }
            }
        }
    }
    return hit;
}

// FIXME(shotgun): 100 %, matches
bool ShadowLink2_804A43E8::fn_801CAED4(int player) {
    bool ok = false;
    if (field_0x560 != 4 && field_0x560 != player) {
        ok = true;
    }
    return ok;
}

// Whether something hurts the actor through its hit box: the hit box of player attacks.
// FIXME(shotgun): 78 %, missing/extra + reloc-name
u8 ShadowLink2_804A43E8::fn_801CA274() {
    if (field_0x564 == 3) {
        return 0;
    }
    u8 mask;
    if (field_0x560 == 4) {
        mask = 0xF;
    } else {
        mask = 1 << field_0x560;
    }
    if (mHitBox.mBox.f.x - mHitBox.mBox.i.x <= 0.0f || mHitBox.mBox.f.y - mHitBox.mBox.i.y <= 0.0f) {
        return 0;
    }
    u8 hits = mHitBox.fn_8021693C(mask);
    if (hits != 0) {
        bool done = false;
        for (s32 i = 0; i < 4; i++) {
            u32 bit = 1 << i;
            if (hits & bit) {
                fn_802379E4(i, &mPos);
                if (!done) {
                    done = true;
                    f32 dy = mPos.y - getPlayerPos_8023E724(i)->y;
                    f32 dx = mPos.x - getPlayerPos_8023E724(i)->x;
                    f32 sq = dx * dx + dy * dy;
                    if (sq > 32.0f * FLT_EPSILON) {
                        f32 norm = JGeometry::TUtil<f32>::inv_sqrt(sq);
                        dx *= norm;
                        dy *= norm;
                    }
                    dx *= 3.0f;
                    dy *= 3.0f;
                    field_0x3c.x = dx;
                    field_0x3c.y = dy;
                    fn_801C3B7C(0x20);
                    mFlags |= 0x80000;
                }
                mask &= ~bit;
            }
        }
    }
    JGeometry::TBox2f saved = mBox;
    mBox = mHitBox.mBox;
    u8 result;
    if (mask == 0) {
        result = 0;
    } else {
        result = fn_801F2740(-1);
        if (result != 0) {
            JGeometry::TVec2f out;
            fn_801F3128(&out);
            for (s32 j = 0; j < 4; j++) {
                u32 bit = 1 << j;
                if (result & bit) {
                    if (mask & bit) {
                        JGeometry::TVec2f v;
                        if (fn_801F329C(j)) {
                            JGeometry::TVec3f in(out.x, out.y, 0.0f);
                            JGeometry::TVec3f res;
                            fn_801CD0D0(&in, &res);
                            v.x = res.x;
                            v.y = res.y;
                        } else {
                            v.x = out.x;
                            v.y = out.y;
                        }
                        fn_80238548(j, field_0x114, &v, 2);
                    } else {
                        result &= ~bit;
                    }
                }
            }
        }
    }
    mBox = saved;
    return result;
}

// FIXME(shotgun): 86 %, missing/extra + reloc-name
bool ShadowLink2_804A43E8::fn_801CA0F8() {
    if (mBox.f.x - mBox.i.x <= 0.0f || mBox.f.y - mBox.i.y <= 0.0f || fn_801C53C4()) {
        return false;
    }
    bool hit = false;
    s32 serial = getActorManager_801F856C()->fn_801F82CC(this, field_0x195, 32.0f);
    if (serial >= 0 && serial != mSerial) {
        ActorBase_804A5770* other = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (other != NULL) {
            if (other->mActorId == ACTOR_SWBM || other->mActorId == ACTOR_FLME) {
                return false;
            }
            JGeometry::TVec3f from(mPos);
            from -= other->field_0x3c;
            hit = other->fn_801F37AC(0, &from, 0, other->field_0x168, other->field_0x110);
            if (hit) {
                other->field_0x164 = 1;
                if (mState == 16) {
                    mFlags |= 0x800000;
                } else {
                    fn_801C3B7C(16);
                }
            }
        }
    }
    return hit;
}

// Whether something hit by the actor's second box counts as a hit on the player/arrow/bomb.
// FIXME(shotgun): 93 %, missing/extra
bool ShadowLink2_804A43E8::fn_801C9EB8() {
    if (mBox2.f.x - mBox2.i.x <= 0.0f || mBox2.f.y - mBox2.i.y <= 0.0f || fn_801C53C4()) {
        return false;
    }
    s32 outPlayer = -1;
    JGeometry::TVec3f arrowVel(0.0f, 0.0f, 0.0f);
    bool arrow = false;
    bool direct = false;
    bool bomb = false;
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
                bomb = fn_801CC538(player, &pos2, &bombVel, &outPlayer);
            } else {
                player = fn_8023AD54(&box, field_0x4, &pos2);
                if (player >= 0) {
                    fn_801CC640(player, &outPlayer, &arrowVel);
                }
            }
        }
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
        hit = fn_801F37AC(0, &from, 0, type, player);
        if (hit) {
            if (mState == 16) {
                mFlags |= 0x800000;
            } else {
                fn_801C3B7C(16);
            }
        }
    }
    return hit;
}

// Moves the actor along the map; lifts the z-offset into the ground plane for the sweep when the
// actor is in the air (state 0x1C) and lands it when it touches ground.
// FIXME(shotgun): 83 %, missing/extra + reloc-name
bool ShadowLink2_804A43E8::fn_801C9CB0() {
    bool hit = false;
    if (!(mFlags & ENEMY_FLAG_NO_MAP_HIT) && field_0x80.f.x - field_0x80.i.x > 0.0f && field_0x80.f.y - field_0x80.i.y > 0.0f) {
        JGeometry::TVec3f savedPrev = mPrevPos;
        JGeometry::TVec3f savedPos = mPos;
        u8 floorLayer = viewLayerFlag(field_0x4);
        bool lifted = false;
        if (mState == 0x1C && !(mPos.z <= 0.0f) && floorLayer != 0) {
            lifted = true;
            mPrevPos.y = mPrevPos.y - mPrevPos.z;
            mPrevPos.z = 0.0f;
            mPos.y = mPos.y - mPos.z;
            mPos.z = 0.0f;
        }
        hit = fn_801F2618(&mMapHitDir, lbl_8053EFC0);
        if (lifted) {
            mPrevPos = savedPrev;
            if (!hit) {
                mPos = savedPos;
            } else {
                mPos.z = savedPos.z - (mPos.y - (savedPos.y - savedPos.z));
                mPos.y = savedPos.y;
                fn_801CE800();
            }
        } else if (hit && floorLayer != 0 && !(mFlags & ENEMY_FLAG_LANDED) && !(mPos.z <= 0.0f) &&
                   fabs(mMapHitDir.y - -1.0f) < 0.001f && fabs(mPos.y - mPrevPos.y) < 0.001f) {
            mPos.z = 0.0f;
            field_0x3c.z = 0.0f;
            mFlags |= 0x10;
        }
    }
    return hit;
}

// FIXME(shotgun): 86 %, missing/extra + reloc-name
bool ShadowLink2_804A43E8::fn_801C9AF8(s32 layer, const JGeometry::TVec3f* pos) {
    JGeometry::TVec3f p(pos->x, pos->y, pos->z);
    if (viewLayerFlag(layer)) {
        p.y = p.y - p.z;
        p.z = 0.0f;
    }
    bool special = false;
    if (p.z <= 0.0f) {
        switch (mState) {
        case 0x11:
        case 0x17:
            special = true;
            break;
        }
    }
    if (special) {
        switch (fn_8022461C(&p, layer)) {
        case 3:
            if (fn_80226850(&p, layer) == 0x7A) {
                return false;
            }
            break;
        case 1: {
            u16 tile = fn_80226850(&p, layer);
            if ((tile >= 0x9B && tile < 0x9D) || (tile >= 0x322 && tile < 0x324)) {
                return false;
            }
            break;
        }
        case 0x27:
        case 0x28:
        case 0x29:
        case 0x2A:
        case 0x2B:
            return false;
        }
    }
    bool ok = true;
    if (!fn_802265DC(&p, layer, false)) {
        s32 serial = mSerial;
        CollisionBoxes_8020D58C* boxes = fn_8020D58C();
        if (!boxes->mLists[boxes->field_0x1].fn_8020D6AC(layer, &p, serial, lbl_8053EF18)) {
            ok = false;
        }
    }
    return ok;
}
