#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/Camera.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/act_liftable_wood.h"

static const f32 lbl_8046CAD0[4][2] = {
    {16.0f, 0.0f},
    {-16.0f, 0.0f},
    {0.0f, 16.0f},
    {0.0f, -16.0f},
};

static const u32 lbl_8046CAF0[4] = {4, 3, 3, 3};

void LiftableWood_804A87A8::init() {
    field_0x238 = (field_0x90 >> 22) & 0x1F;
    mPos.x += 8.0f;
    mBox.i.x = -48.0f;
    mBox.i.y = -16.0f;
    mBox.f.x = 48.0f;
    mBox.f.y = 40.0f;
    field_0x1a4 = 1;
    if (field_0x4 < 8) {
        field_0x244 = 1;
    } else {
        field_0x244 = 8;
    }
    mTimer23c = 2;
    mState = 0;
    field_0x248 = getGameView_802D6A9C()->fn_802D9C4C();
    mEmitter230 = NULL;
    mTimer240 = 0;
    if (getGameView_802D6A9C()->fn_802D7544(field_0x16c)) {
        // FIXME: 89.58% (missing/extra) - inline TVec3f(mPos.x - 8.0f, mPos.y, mPos.z) temporary loads mPos.y/z before field_0x4/field_0x244
        gBgRewrite_8053AC98->fn_8029D8F0(&JGeometry::TVec3f(mPos.x - 8.0f, mPos.y, mPos.z), field_0x4, field_0x244, 0);
        mAlive = false;
    }
}

LiftableWood_804A87A8::~LiftableWood_804A87A8() {
    if (mEmitter230 != NULL) {
        releaseEmitter_801EE434(mEmitter230);
    }
}

void LiftableWood_804A87A8::update() {
    switch (mState) {
    case 0: {
        mBox.i.x = -30.0f;
        mBox.i.y = -20.0f;
        mBox.f.x = 30.0f;
        mBox.f.y = 20.0f;
        JGeometry::TBox2f box = mBox;
        box.addPos(mPos.x, mPos.y);
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i) && fn_80236E3C(i) && fn_80239C00(i, &box, field_0x4)) {
                JGeometry::TVec3f* playerPos = getPlayerPos_8023E724(i);
                f32 px = playerPos->x;
                f32 py = playerPos->y;
                s32 dir = fn_80230CFC(i);
                px += lbl_8046CAD0[dir][0];
                py += lbl_8046CAD0[dir][1];
                // Hand-expanded point-in-box test; this local and `== true` match the original (a contains() helper, with or without `== true`: 97.34 %).
                bool inside = false;
                if (px >= box.i.x && py >= box.i.y && px < box.f.x && py < box.f.y) {
                    inside = true;
                }
                if (inside == true && fn_8022F9E4(i, mSerial, true, false)) {
                    getGameView_802D6A9C()->fn_802D7584(field_0x16c, 1);
                    mTimer23c = 50;
                    mState = 2;
                    break;
                }
            }
        }
        if (mState == 0 && mTimer23c != 0) {
            mTimer23c--;
        }
        break;
    }
    case 1:
        mState = 0;
        mTimer23c = 20;
        break;
    case 2:
        mTimer23c--;
        if (mTimer23c == 0) {
            // FIXME: 97.70% (missing/extra) - inline TVec3f(mPos.x - 8.0f, mPos.y, mPos.z) temporary load order and setGlobalTranslation load order
            gBgRewrite_8053AC98->fn_8029D8F0(&JGeometry::TVec3f(mPos.x - 8.0f, mPos.y, mPos.z), field_0x4, field_0x244, 0);
            mState = 3;
            if (field_0x248[0] == 'u') {
                mEmitter230 = spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(mPos.x, mPos.y, mPos.z), 0x280, 0, 0, NULL, NULL, true, 1.0f);
            } else {
                mEmitter230 = spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(mPos.x, mPos.y, mPos.z), 0x27F, 0, 0, NULL, NULL, true, 1.0f);
            }
            mTimer240 = 50;
        }
        break;
    case 3:
        if (field_0xc4) {
            if ((fn_8022CAF8(field_0xc8) == 4 || fn_8022CAF8(field_0xc8) == 5) && fn_8022F478(field_0xc8)) {
                fn_80230C6C(field_0xc8);
            } else if (getPlayerLayer_8023DE58(field_0xc8) < 8) {
                mPos.set(*fn_8023E7B0(field_0xc8));
                if (mEmitter230 != NULL) {
                    if (mTimer240 == 0) {
                        mEmitter230->becomeInvalidEmitter();
                        mEmitter230 = NULL;
                    } else {
                        mEmitter230->setGlobalTranslation(mPos.x, mPos.y, mPos.z);
                        mTimer240--;
                    }
                }
            } else {
                fn_80230C6C(field_0xc8);
            }
        } else {
            field_0x3c.z += -0.15f;
            mPos += field_0x3c;
            if (mPos.z < 0.0f) {
                vfunc_0x24();
            } else if (fn_801F2B7C(0, field_0xc8, 6) || fn_801F2B20(0)) {
                vfunc_0x24();
            }
        }
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        break;
    }
}

void LiftableWood_804A87A8::draw() {
    if (mState == 1 || mState >= 3) {
        SpriteParams_801F5AE0 params;
        params.field_0x1c = 0;
        params.field_0x1d = 1;
        if (field_0x248[0] == 'r' || field_0x248[0] == 's' || field_0x248[0] == 'y') {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', "wood_ura.bti");
            fn_801EFD34(res, &params, 7, 0.0f, -18.0f, 0.0f);
        } else if (field_0x248[0] == 'l' || field_0x248[0] == 'x' || field_0x248[0] == 'p') {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', "wood_yasi.bti");
            fn_801EFD34(res, &params, 7, 0.0f, -18.0f, 0.0f);
        } else if (field_0x248[0] == 'u') {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', "wood_yuki.bti");
            fn_801EFD34(res, &params, 7, 0.0f, -18.0f, 0.0f);
        } else {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', "wood_omote.bti");
            fn_801EFD34(res, &params, 7, 0.0f, -18.0f, 0.0f);
        }
    }
}

void LiftableWood_804A87A8::vfunc_0x24() {
    if (field_0x248[0] == 'r' || field_0x248[0] == 's' || field_0x248[0] == 'y') {
        spawnEmitter_8013CC50(field_0x4, &mPos, 0xAD, 1, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0xB1, 1, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0xB2, 1, 0, NULL, NULL, true, 1.0f);
    } else if (field_0x248[0] == 'u') {
        spawnEmitter_8013CC50(field_0x4, &mPos, 0xAE, 1, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0xB3, 1, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0xB4, 1, 0, NULL, NULL, true, 1.0f);
    } else {
        spawnEmitter_8013CC50(field_0x4, &mPos, 0xAD, 1, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0xAF, 1, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0xB0, 1, 0, NULL, NULL, true, 1.0f);
    }
    startSound_801F0E34(0x30003, 0);
    if (field_0x238 == 0) {
        for (s32 i = 0; i < 4; i++) {
            s32 serial = getActorManager_801F856C()->fn_801F8544(ACTOR_RUPY, field_0x4, &mPos, lbl_8046CAF0[i]);
            if (serial >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (actor != NULL) {
                    // FIXME: 99.32% (regalloc-only) - target->ours r6->r7, f4->f3, r7->r6, f1->f0 in random velocity construction
                    actor->field_0x3c = JGeometry::TVec3f(
                        2.0f - 4.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1(),
                        2.0f - 4.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1(),
                        3.0f + 3.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()
                    );
                }
            }
        }
    } else if (field_0x238 == 2) {
        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_FARY, field_0x4, &mPos, 0, -1, -1);
    }
    mAlive = false;
}
