#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/ViewLayer.h"
#include "game/act/tu_8020C0A8.h"
#include "JSystem/JMath/JMath.h"
#include <float.h>

// Sprite frame tables of the actor (.data 0x8053A178, not written as source here).
extern const s16* lbl_8053A178[];
extern const u32 lbl_8053F9E0[];

cls_804A5F40::cls_804A5F40() {
    field_0x2a8 = -1;
    field_0x2ac = 0;
    field_0x2ae = 0;
}

// FIXME(shotgun): 84.42 %, missing/extra + reloc-name.
void cls_804A5F40::init() {
    EnemyBase_804A4808::init();
    if (field_0x90 & 1) {
        mFlags |= 0x2000;
    }
    field_0xb0 = 0x31101;
    if (field_0x98 == 3) {
        if (fn_801CF144()) {
            field_0x98 = fn_801CF720(3);
        }
    }
    field_0x2ae = fn_801F6874(field_0x98);
    field_0x194 = 1;
    mState = 0;
    mStateTimer = 0;
    field_0x250 = fn_801CD664(0x3D) + 0x28;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mAnim.fn_801D2608(lbl_8053A178, 1);
    mAnim.startAnim_801D22DC(0, 2, false);
    mAnim.fn_801D1D60();
    if (mFlags & 0x2000) {
        if (vfunc_0x4C(field_0x4, &mPos)) {
            fn_801F3668();
        } else {
            JGeometry::TBox2f* box = layerBox(field_0x4);
            bool inside = false;
            if (field_0x80.i.x + mPos.x >= box->i.x && field_0x80.i.y + mPos.y >= box->i.y &&
                field_0x80.f.x + mPos.x < box->f.x && field_0x80.f.y + mPos.y < box->f.y) {
                inside = true;
            }
            if (!inside) {
                fn_801F3668();
            }
        }
    }
}

// FIXME(shotgun): 94.95 %, missing/extra + reloc-name.
void cls_804A5F40::fn_8020C6E8() {
    s32 nearest = -1;
    if (mFlags & 0x2000) {
        f32 best = FLT_MAX;
        for (s32 i = 0; i < 4; i++) {
            if (!fn_8023F264(i)) {
                continue;
            }
            if (fn_8022D75C(i)) {
                continue;
            }
            if (fn_8022F478(i)) {
                continue;
            }
            if (getPlayerLayer_8023DE58(i) != field_0x4) {
                continue;
            }
            f32 dist = fn_801D0DA0(i, &mPos);
            if (dist < best) {
                best = dist;
                nearest = i;
            }
        }
    }
    if (mState == 2) {
        if (nearest < 0) {
            field_0x250 = fn_801CD664(0x3D) + 0x28;
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

// FIXME(shotgun): 99.27 %, missing/extra + reloc-name.
void cls_804A5F40::vfunc_0x90(u8 mapHit) {
    fn_8020C6E8();
    switch (mState) {
    case 0:
        if (mStateTimer >= field_0x250) {
            field_0x250 = fn_801CD664(0x29) + 0x3C;
            mTargetSpeed = 0.4f;
            field_0x2a8 = -1;
            field_0x2ac = 0;
            if (fn_801CF144()) {
                field_0x98 = fn_801CF720(3);
            } else {
                field_0x98 = fn_801CF6B8();
            }
            field_0x2ae = fn_801F6874(field_0x98);
            mStateTimer = 0;
            mState = 1;
        }
        break;
    case 1:
        if (mapHit) {
            if (fn_801CF144()) {
                if (field_0x98 == 3) {
                    field_0x98 = fn_801CF720(3);
                } else {
                    field_0x98 = fn_801CF798(3, field_0x98);
                }
            } else {
                field_0x98 = fn_801CF720(field_0x98);
            }
            field_0x250 += 10;
        } else if (mStateTimer >= field_0x250) {
            if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f < 0.35f) {
                field_0x250 = fn_801CD664(0x3D) + 0x28;
                mTargetSpeed = 0.0f;
                field_0x2a8 = -1;
                field_0x2ac = 0;
                mStateTimer = 0;
                mState = 0;
            } else {
                field_0x250 = fn_801CD664(0x29) + 0x3C;
                mTargetSpeed = 0.4f;
                field_0x2a8 = -1;
                field_0x2ac = 0;
                if (fn_801CF144()) {
                    field_0x98 = fn_801CF720(3);
                } else {
                    field_0x98 = fn_801CF6B8();
                }
                field_0x2ae = fn_801F6874(field_0x98);
                mStateTimer = 0;
                mState = 1;
            }
        }
        field_0x2ae = fn_801F6874(field_0x98);
        break;
    case 2:
        field_0x2ae = fn_801D0280(field_0x2a8) - 0x8000;
        if (viewLayerFlag(field_0x4)) {
            if (!(fn_801D0DD4() && fn_801CC9F8(&mPos, field_0x4))) {
                if (field_0x2ae > 0) {
                    field_0x2ae = 0x4000;
                } else {
                    field_0x2ae = -0x4000;
                }
            }
        }
        break;
    }
    if (fn_801D0DD4() && mState != 0) {
        if (mAnim.fn_801D217C()) {
            startSound_801F0E34(0x5004F, 0);
        }
    }
}

// FIXME(shotgun): 68.48 %, missing/extra.
void cls_804A5F40::vfunc_0x88() {
    field_0x3c.x = JMath::sincosTable_.sinShort(field_0x2ae) * mSpeed;
    field_0x3c.y = JMath::sincosTable_.cosShort(field_0x2ae) * mSpeed;
    vfunc_0x84();
}

void cls_804A5F40::vfunc_0x98() {
    if (!(mFlags & 7)) {
        fn_801CE32C();
    } else if (viewLayerFlag(field_0x4)) {
        fn_801CE5D4();
    }
}

// FIXME(shotgun): 89.12 %, missing/extra.
bool cls_804A5F40::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    const u32* ignoreIds = NULL;
    if (mFlags & 7) {
        ignoreIds = lbl_8053F9E0;
    }
    bool result = true;
    if (!fn_802265DC(pos, layer, false)) {
        s32 serial = mSerial;
        CollisionBoxes_8020D58C* boxes = fn_8020D58C();
        if (!boxes->mLists[boxes->field_0x1].fn_8020D6AC(layer, pos, serial, ignoreIds)) {
            result = false;
        }
    }
    return result;
}

// FIXME(shotgun): 99.81 %, reloc-name.
void cls_804A5F40::draw() {
    SpriteParams_801F5AE0 params;
    params.field_0x0[2] = 4;
    drawSprite_801F06F0("enemy_gc_sob.bin", mAnim.mCel + 0x2C9, &params, 0, 1.0f, 1.0f);
}

cls_804A5F40::~cls_804A5F40() {}
