#include "game/GameAudio.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/RotMatrix.h"
#include "game/act/act_force_gem_wooden.h"

// The ids fn_801F26CC searches for: 'FIRE', 'BLZE', 'DDGO', then the 'NULL' end.
static const u32 lbl_80471708[4] = {'FIRE', 'BLZE', 'DDGO', 'NULL'};

// FIXME: 98.60 %. Register allocation and scheduling only: the scale (field_0x240,
// or field_0x238 * field_0x240 in the branch) and the sine results sit in other
// saved registers than the original's, and the products take another operand order.
void WoodenForceGem_804AB1F0::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtxA;
    Mtx mtxB;
    JGeometry::TVec3f saved;
    const char* name = "enemy_both2_sob.bin";
    s32 frame = 0x89;
    if (field_0x248 == 1) {
        name = "enemy_gc_sob.bin";
        frame = field_0x250 + 0x471;
    }
    params.field_0x2d = 1;
    params.field_0x1d = -1;
    if (field_0x230 == 2 || field_0x230 == 3) {
        f32 scale = field_0x238 * field_0x240;
        PSMTXIdentity(mtxA);
        setRotScaleMtx(mtxA, JGeometry::TVec3f(0.0f, field_0x23c, 0.0f), scale);
        params.mMtx = mtxA;
        if (field_0x4 < 8) {
            void* res = lbl_8053AAF8->field_0xa4->getResource('SOB ', name);
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
        }
        return;
    }
    params.field_0x1d = 0;
    PSMTXIdentity(mtxB);
    setRotScaleMtx(mtxB, JGeometry::TVec3f(0.0f, field_0x23c, 0.0f), field_0x240);
    params.mMtx = mtxB;
    if (mPos.z != 0.0f) {
        params.field_0x1c = 1;
    }
    if (field_0x258 == 4 || field_0x258 == 5) {
        drawSprite_801F06F0(name, frame, &params, -1, 1.0f, 1.0f);
        saved = mPos;
        mPos.y -= 1.0f;
        if (mSpriteParams.mMtx == NULL) {
            fn_801EF830(field_0x4, &mPos, &params, 7, name, frame, NULL);
        }
        mPos = saved;
    } else {
        drawSprite_801F06F0(name, frame, &params, 7, 1.0f, 1.0f);
    }
    fn_803075AC(mSerial, field_0x4, &mPos, frame, false, 2, -1, 0, 0, 0);
}

// FIXME: 98.27%. The emitter block in case 3 still differs: the original also stores 1
// at offset 0x24 of the emitter (after the status-bit store, which stopCreateParticle
// covers) and orders the pointer and status work differently, so the registers differ.
// include/JSystem/JParticle/JPAEmitter.h has no field at 0x24 (mEmitterRot is an s16
// vector there), so it needs a header decision outside include/game/. The original also
// stores and reloads field_0x250 near the end of the function (the field_0x248 == 1
// block), which ours does not.
void WoodenForceGem_804AB1F0::update() {
    JGeometry::TBox2f box(mBox.i, mBox.f);
    JGeometry::TVec2f p(mPos.x, mPos.y);
    box.addPos(p);
    switch (field_0x230) {
    case 0: {
        fn_80239914(&box, field_0x4, mSerial);
        s32 serial = mSerial;
        s32 layer = field_0x4;
        CollisionBoxes_8020D58C* boxes = fn_8020D58C();
        boxes->mLists[boxes->field_0x0].fn_8020D8C0(&box, layer, serial);
        if (fn_8023AB94(&box, field_0x4, &p) >= 0 || fn_801F26CC(lbl_80471708) >= 0) {
            if (field_0x248 == 0) {
                startSound_801F0E34(0x30005, 0);
                fn_801F856C()->fn_801F9484('BLZE', field_0x4, &mPos, 0, -1, -1);
            } else {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x28D, 0, 0, NULL, NULL, true, 1.25f * field_0x240);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x28E, 0, 0, NULL, NULL, true, 1.25f * field_0x240);
                startSound_801F0E34(0x30061, 0);
            }
            field_0x230 = 1;
            field_0x25c = 0x28;
        }
        break;
    }
    case 1: {
        fn_80239914(&box, field_0x4, mSerial);
        field_0x25c--;
        if (field_0x25c <= 0) {
            s32 kind = field_0x258;
            s32 arg;
            if (kind == 4 || kind == 5) {
                arg = kind - 4;
            } else {
                arg = kind + 4;
            }
            s32 serial = fn_801F856C()->fn_801F8544('RUPY', field_0x4, &mPos, arg);
            s32 bit = field_0x260;
            if (bit > 0) {
                getGameView_802D6A9C()->fn_802D8050(bit, true, 0xFF);
            }
            if (serial >= 0) {
                ActorBase_804A5770* actor = fn_801F856C()->findActorBySerial_801F666C(serial);
                if (actor != NULL) {
                    JGeometry::TVec3f v(0.0f, 0.0f, 3.0f);
                    actor->field_0x3c = v;
                }
            }
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x19D, 0, 0, NULL, NULL, true, 0.1f);
            fn_801F3668();
        }
        break;
    }
    case 3:
        if (field_0x230 == 3 && mEmitter268 != NULL) {
            mEmitter268->stopCreateParticle();
            mEmitter268 = NULL;
        }
        // fall through
    case 2:
        field_0x23c += 0.1f;
        field_0x238 -= 0.02f;
        if (field_0x238 < 0.1f) {
            fn_801F3668();
        }
        break;
    }
    if (field_0x248 == 1) {
        field_0x24c--;
        if (field_0x24c <= 0) {
            field_0x250 = (field_0x250 + 1) & 3;
            if (field_0x250 == 0) {
                field_0x24c = (s32)(lbl_8053AAF8->mRandom.get_ufloat_1() * 60.0f) + 60;
            } else {
                field_0x24c = 2;
            }
        }
    }
}

bool WoodenForceGem_804AB1F0::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    s32 tile = fn_8022461C(pos, layer);
    if (tile == 0xC || tile == 3 || tile == 0xE) {
        field_0x3c.z = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.x = 0.0f;
        field_0x230 = 2;
        return false;
    }
    if (tile == 0xD) {
        field_0x3c.z = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.x = 0.0f;
        if (mEmitter268 == NULL) {
            mEmitter268 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x3D0, 0, 0, NULL, NULL, true, 1.0f);
            startSound_801F0E34(0x30022, 0);
        }
        field_0x230 = 3;
        return false;
    }
    return tile != 0;
}

void WoodenForceGem_804AB1F0::init() {
    field_0x258 = field_0x90 & 0xF;
    field_0x248 = (field_0x90 >> 16) & 0xF;
    mEmitter268 = NULL;
    field_0x260 = (field_0x90 >> 22) & 0x1F;
    field_0xb0 = 0x2300;
    field_0x230 = 0;
    field_0x264 = 0;
    mLife = 10000;
    field_0x244 = 0;
    field_0x254 = -1;
    field_0x98 = 0;
    field_0x238 = 1.0f;
    field_0x23c = 0.0f;
    field_0x25c = 0;
    field_0x24c = (s32)(lbl_8053AAF8->mRandom.get_ufloat_1() * 60.0f) + 60;
    field_0x250 = 0;
    if (field_0x258 == 4 || field_0x258 == 5) {
        field_0x240 = 0.6f;
    } else {
        field_0x240 = 1.0f;
    }
    if (vfunc_0x34()) {
        fn_801F3668();
        return;
    }
    field_0x234 = 30.0f;
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x80.i.x = -6.0f;
    field_0x80.i.y = -6.0f;
    field_0x80.f.x = 6.0f;
    field_0x80.f.y = 6.0f;
    mBox.i.x = -7.0f;
    mBox.i.y = -7.0f;
    mBox.f.x = 7.0f;
    mBox.f.y = 7.0f;
    if (lbl_8053AAF8->field_0x7f == 0) {
        fn_801F3668();
    }
}

WoodenForceGem_804AB1F0::~WoodenForceGem_804AB1F0() {
    fn_801EE434(mEmitter268);
}
