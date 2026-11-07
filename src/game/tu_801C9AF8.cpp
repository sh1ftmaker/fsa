#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Map.h"
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
