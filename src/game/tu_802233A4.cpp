#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/EmitterDelete.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/act/act_leaf_effect_npc_generator.h"
#include "game/act/tu_802233A4.h"

cls_804A6D00::cls_804A6D00() {
    field_0x2b8 = NULL;
    field_0x2bc = NULL;
    field_0x2c0 = NULL;
}

cls_804A6D00::~cls_804A6D00() {
    deleteEmitter(&field_0x2b8);
    fn_801CDC8C(0x4000E);
    releaseEmitter_801EE434(field_0x2bc);
    releaseEmitter_801EE434(field_0x2c0);
    field_0x2bc = NULL;
    field_0x2c0 = NULL;
}

// FIXME(shotgun): 99.84 %, reloc-name
void cls_804A6D00::init() {
    ShadowLinkBase_804A6A00::init();
    field_0x98 = field_0x90 & 3;
    mFlags &= ~0x100;
    mState = 4;
    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mAnim.startAnim_801D22DC(2, 0, false);
}

// Not auto-inlined: vfunc_0x90 calls it with a plain bl in the original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// FIXME(shotgun): 94.84 %, missing/extra + reloc-name
void cls_804A6D00::fn_80223AD8(s32 state) {
    switch (state) {
    case 5:
        mTargetSpeed = 2.5f;
        mAnim.fn_801D22A0(field_0x98 + 0x14, 2);
        field_0x2bc = spawnEmitter_8013CC50(field_0x4, &mPos, 0x116, 0, 0, NULL, NULL, true, 1.0f);
        field_0x2c0 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x115, 0, 0, NULL, NULL, true, 1.0f);
        break;
    case 6: {
        mTargetSpeed = 0.0f;
        field_0x3c.x = -field_0x3c.x;
        field_0x3c.y = -field_0x3c.y;
        field_0x3c.z = 0.0f;
        field_0x3c.normalize();
        field_0x3c.scale(1.2f);
        field_0x3c.z = 4.0f;
        mAnim.startAnim_801D22DC(field_0x98 + 0xC, 0, false);
        if (!fn_801CD2F8(field_0x4)) {
            s32 serial = getActorManager_801F856C()->fn_801F7884('LFGN', field_0x4);
            if (serial >= 0) {
                ActorBase_804A5770* gen = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (gen != NULL) {
                    ((LeafEffectNPCGenerator_804AE288*)gen)->fn_8036D858(mPos);
                }
            }
        }
        startSound_801F0E34(0x400EB, 0);
        JGeometry::TVec3f off(0.0f, 0.0f, 0.0f);
        if (field_0x98 < 2 && field_0x98 >= 0) {
            off.set(0.0f, -8.0f, 0.0f);
            getGameView_802D6A9C()->fn_802D79E8(field_0x4, 0.0f, 4.0f, false);
        } else {
            getGameView_802D6A9C()->fn_802D79E8(field_0x4, 4.0f, 0.0f, false);
        }
        gGameRoot_8053AAF8->fn_80134DE4(1, 0);
        JGeometry::TVec3f n(-field_0x3c.y, -field_0x3c.x, 0.0f);
        n.normalize();
        JGeometry::TVec3f at = mPos + off;
        if (field_0x4 < 8) {
            gGameRoot2_8053AB10->field_0x20->fn_802F601C(&at, 3, &n, 1.0f, 1.0f);
        }
        break;
    }
    case 7:
    case 8:
        mSpeed = 0.0f;
        mTargetSpeed = 0.0f;
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        mAnim.fn_801D22A0(field_0x98, 0);
        if (state == 7) {
            field_0x2b8 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x565, 0, 1, NULL, NULL, true, 0.5f);
            startSound_801F0E34(0x4000E, 0);
        }
        break;
    }
    mStateTimer = 0;
    mState = state;
}
#pragma auto_inline reset

// The rectangle (in front of the actor while it charges in state 5) as (x0, y0, x1, y1);
// empty (1, 1, -1, -1) in the other states.
static inline void attackBox(cls_804A6D00* a, f32& x0, f32& y0, f32& x1, f32& y1) {
    x0 = 1.0f;
    x1 = -1.0f;
    y0 = x0;
    y1 = x1;
    if (a->mState == 5) {
        x0 = -8.0f;
        x1 = 8.0f;
        y0 = x0;
        y1 = x1;
        switch (a->field_0x98) {
        case 0:
            x0 += 16.0f;
            y0 += 0.0f;
            x1 += 16.0f;
            y1 += 0.0f;
            break;
        case 1:
            x0 += -16.0f;
            y0 += 0.0f;
            x1 += -16.0f;
            y1 += 0.0f;
            break;
        case 2:
            x0 += 0.0f;
            y0 += 16.0f;
            x1 += 0.0f;
            y1 += 16.0f;
            break;
        case 3:
            x0 += 0.0f;
            y0 += -16.0f;
            x1 += 0.0f;
            y1 += -16.0f;
            break;
        }
    }
}

// Not auto-inlined: vfunc_0x90 calls it with a plain bl in the original.
#pragma auto_inline off
// FIXME(shotgun): 75.89 %, missing/extra + reloc-name
void cls_804A6D00::fn_802238CC() {
    f32 x0, y0, x1, y1;
    attackBox(this, x0, y0, x1, y1);
    if (x1 - x0 > 0.0f && y1 - y0 > 0.0f) {
        x0 += mPos.x;
        y0 += mPos.y;
        x1 += mPos.x;
        y1 += mPos.y;
        JGeometry::TVec3f p(x0, y0, 0.0f);
        gBgRewrite_8053AC98->fn_802A20F0(&p, -1, field_0x4, 0, 0);
        JGeometry::TVec3f q(x1, y0, 0.0f);
        gBgRewrite_8053AC98->fn_802A20F0(&q, -1, field_0x4, 0, 0);
        JGeometry::TVec3f r(x1, y1, 0.0f);
        gBgRewrite_8053AC98->fn_802A20F0(&r, -1, field_0x4, 0, 0);
        JGeometry::TVec3f t(x0, y1, 0.0f);
        gBgRewrite_8053AC98->fn_802A20F0(&t, -1, field_0x4, 0, 0);
    }
}
#pragma auto_inline reset

// FIXME(shotgun): 99.94 %, reloc-name
void cls_804A6D00::vfunc_0x90(u8 mapHit) {
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
        if (mStateTimer >= 0x78) {
            fn_80223AD8(5);
        }
        break;
    case 4:
        fn_802216E8(0x78);
        if (mPos.z <= 0.0f) {
            if (mFlags & CLS_804A6A00_FLAG_0x2000) {
                fn_80223AD8(5);
            } else {
                field_0x3c.z = lbl_8053FE70;
            }
        }
        break;
    case 5: {
        u32 sound = 0x400E7;
        if (fn_8022461C(&mPos, field_0x4) == 0x39) {
            sound = 0x400E8;
        } else if (fn_80224B88(&mPos, field_0x4)) {
            sound = 0x400E9;
        } else if (fn_80224E88(&mPos, field_0x4)) {
            sound = 0x400EA;
        }
        startSound_801F0E34(sound, 0);
        fn_802238CC();
        if (mStateTimer > 1 && mapHit) {
            releaseEmitter_801EE434(field_0x2bc);
            releaseEmitter_801EE434(field_0x2c0);
            field_0x2bc = NULL;
            field_0x2c0 = NULL;
            fn_80223AD8(6);
        }
        break;
    }
    case 6:
        if (mStateTimer > 1 && mPos.z <= 0.0f) {
            fn_80223AD8(7);
        }
        break;
    case 7:
        if (mStateTimer >= 0x7B) {
            deleteEmitter(&field_0x2b8);
            fn_801CDC8C(0x4000E);
            fn_80223AD8(8);
        } else if (mStateTimer % 6 == 0) {
            field_0x98 = fn_801CD30C(field_0x98);
            mAnim.startAnim_801D22DC(field_0x98, 0, false);
        }
        break;
    case 8:
        if (mStateTimer >= 0x1E) {
            fn_8022149C(1);
        }
        break;
    }
}

void cls_804A6D00::vfunc_0x9C(u8 mapHit) {
    ShadowLinkBase_804A6A00::vfunc_0x9C(mapHit);
    setEmitterPos_8013CB44(field_0x2bc, &mPos, 0, 0, true);
    setEmitterPos_8013CB44(field_0x2c0, &mPos, 0, 0, true);
}

void cls_804A6D00::vfunc_0x88() {
    switch (mState) {
    case 2:
        fn_80221250();
        break;
    case 6:
        field_0x3c.z += lbl_8053EFB8;
        break;
    default:
        EnemyBase_804A4808::vfunc_0x88();
        break;
    }
}

void cls_804A6D00::vfunc_0x6C() {
    ShadowLinkBase_804A6A00::vfunc_0x6C();
    deleteEmitter(&field_0x2b8);
    fn_801CDC8C(0x4000E);
    releaseEmitter_801EE434(field_0x2bc);
    releaseEmitter_801EE434(field_0x2c0);
    field_0x2bc = NULL;
    field_0x2c0 = NULL;
}

// FIXME(shotgun): 84.65 %, missing/extra + reloc-name
u8 cls_804A6D00::vfunc_0x68() {
    f32 x0, y0, x1, y1;
    attackBox(this, x0, y0, x1, y1);
    if (x1 - x0 > 0.0f && y1 - y0 > 0.0f) {
        f32 s0 = mBox.i.x;
        f32 s1 = mBox.i.y;
        f32 s2 = mBox.f.x;
        f32 s3 = mBox.f.y;
        mBox.i.x = x0;
        mBox.i.y = y0;
        mBox.f.x = x1;
        mBox.f.y = y1;
        fn_801F2B7C(field_0x114, -1, 1);
        mBox.i.x = s0;
        mBox.i.y = s1;
        mBox.f.x = s2;
        mBox.f.y = s3;
    }
    return 0;
}
