#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/act_light_environment_control_802EF76C.h"
#include "game/act/act_goal_warp_801B6C50.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "game/Camera.h"
#include "game/ActorManager.h"
#include "game/GameView.h"
#include "game/Message.h"
#include "game/Map.h"
#include "std/math.h"

// The item argument of fn_802372F8 by mode (field_0x258).
static s32 lbl_804A39A0[15] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
extern u8 lbl_8053AB98;
extern u8 lbl_8053ABE8;

// The scripted-camera request many actors make: take the camera for the player getRepresentativePlayer_8022EA0C(0)
// names unless a move is already running.
static inline void takeCamera() {
    s32 player = getRepresentativePlayer_8022EA0C(0);
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    if (camera->mShowWholeArea_0x7f5 == 0) {
        camera->mFocusOverride_0x7f4 = 1;
        camera->mFocusMode_0x7f8 = player;
        camera->mOverrideBlend_0x7f6 = 0;
    }
}

// auto_inline off: the original's GoalWarp_804A39DC::init calls fn_801B7550 out of line (bl at 0x801B7430).
#pragma auto_inline off
// 0x801B7550.
u8 fn_801B7550(void) {
    return gGameRoot2_8053AB10->field_0x2c->field_0xc;
}
#pragma auto_inline reset

static inline void callForAllPlayers_8023EE28() {
    for (s32 i = 0; i < 4; i++) {
        fn_8023EE28(i);
    }
}

// 0x801B73B8.
void GoalWarp_804A39DC::init() {
    mBox.set(-128.0f, -128.0f, 128.0f, 128.0f);
    field_0x25c = fn_801B7550();
    field_0x260 = field_0x25c;
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    mMapBox_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x234[0] = 0;
    field_0x234[1] = 0;
    field_0x234[2] = 0;
    for (s32 i = 0; i < 4; i++) {
        const JGeometry::TVec3f* p = getPlayerPos_8023E724(i);
        field_0x264[i].x = p->x;
        field_0x264[i].y = p->y;
        field_0x264[i].z = p->z;
    }
    field_0x250 = mSpawnParam_0x90 & 0xF;
    field_0x254 = (mSpawnParam_0x90 >> 4) & 0xF;
    field_0x24c = (mSpawnParam_0x90 >> 8) & 0xF;
    field_0x258 = (mSpawnParam_0x90 >> 12) & 0xF;
    if (field_0x258 > 10) {
        field_0x258 = 0;
    }
    lbl_8053AC00 = 0;
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
    mEmitters[2] = NULL;
    mEmitters[3] = NULL;
    if (field_0x24c == 0) {
        field_0x230 = 0;
    } else {
        fn_80119DD0()->fn_80119468(0, 0);
        callForAllPlayers_8023EE28();
        field_0x230 = 6;
    }
}

// 0x801B7324.
GoalWarp_804A39DC::~GoalWarp_804A39DC() {
    for (int i = 0; i < 4; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
}

// 0x801B6C54.
// FIXME(shotgun): 99.86 %, scheduling+regalloc; behaviour drift.
void GoalWarp_804A39DC::update() {
    if (field_0x24c == 0 && field_0x230 != 0) {
        fn_80128C58((s32)field_0x260);
    }
    switch (field_0x230) {
    case 0:
        if (field_0x234[2] != 0) {
            lbl_8053AC00 = 0;
            break;
        }
        if (lbl_8053AC00 == 0) {
            break;
        }
        field_0x25c = fn_801B7550();
        field_0x260 = field_0x25c;
        if (getActorManager_801F856C()->fn_801F68E0() == 0) {
            break;
        }
        {
            Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
            mPos.set(-camera->mViewMtx_0x4[0][3], -camera->mViewMtx_0x4[1][3], 0.0f);
        }
        for (s32 i = 0; i < 4; i++) {
            spawnEmitter_8013CC50(mLayer_0x4, getPlayerPos_8023E724(i), 0x3AE, 1, 1, NULL, NULL, true,
                                  1.0f);
            mEmitters[i] = spawnEmitter_8013CC50(mLayer_0x4, getPlayerPos_8023E724(i), 0x3AC, 1, 1,
                                                 NULL, NULL, true, 1.0f);
        }
        field_0x234[1] = 120;
        field_0x230 = 1;
        // fall through
    case 1:
        takeCamera();
        addCalc2_801F71A4(&field_0x260, 136.0f, 0.5f, 5.0f);
        if (field_0x234[1] == 0) {
            JGeometry::TVec3f pos;
            pos.zero();
            pos.y = -10.0f;
            fn_8022FE80(getRepresentativePlayer_8022EA0C(0), mLayer_0x4, mSerial, false, false);
            if (talk(0x240, mSerial, mLayer_0x4, pos, 0x4102)) {
                field_0x230 = 2;
            }
        }
        break;
    case 2:
        takeCamera();
        fn_8022FE80(getRepresentativePlayer_8022EA0C(0), mLayer_0x4, mSerial, false, false);
        if (stepMessageWindow_803865B8(getRepresentativePlayer_8022EA0C(0))) {
            for (s32 i = 0; i < 4; i++) {
                deleteEmitter(&mEmitters[i]);
            }
            switch (fn_8037EFB4(0)) {
            case 0:
                field_0x244 = 0;
                fn_80237524(0, 0x180);
                lbl_8053AAA0 = 1;
                lbl_8053AB98 = 1;
                lbl_8053ABE8 = 1;
                for (s32 i = 0; i < 4; i++) {
                    const JGeometry::TVec3f* p = getPlayerPos_8023E724(i);
                    field_0x264[i].x = p->x;
                    field_0x264[i].y = p->y;
                    field_0x264[i].z = p->z;
                }
                field_0x234[0] = 8;
                field_0x244 = 0x3F;
                for (s32 i = 0; i < 4; i++) {
                    spawnEmitter_8013CC50(mLayer_0x4, getPlayerPos_8023E724(i), 0x489, 0, 0, NULL, NULL,
                                          true, 1.0f);
                    spawnEmitter_8013CC50(mLayer_0x4, getPlayerPos_8023E724(i), 0x551, 0, 0, NULL, NULL,
                                          true, 1.0f);
                }
                field_0x234[1] = 240;
                field_0x230 = 4;
                break;
            case 1:
                for (s32 i = 0; i < 4; i++) {
                    spawnEmitter_8013CC50(mLayer_0x4, getPlayerPos_8023E724(i), 0x48A, 1, 1, NULL, NULL,
                                          true, 1.0f);
                }
                field_0x234[1] = 100;
                field_0x230 = 3;
                break;
            }
            fn_80384C58(getRepresentativePlayer_8022EA0C(0), mSerial);
        }
        break;
    case 3:
        takeCamera();
        if (field_0x234[1] == 0) {
            addCalc2_801F71A4(&field_0x260, field_0x25c, 0.5f, 5.0f);
            if (std::fabsf(field_0x260 - field_0x25c) < 1.0f) {
                field_0x260 = field_0x25c;
                getActorManager_801F856C()->fn_801F6884();
                lbl_8053AC00 = 0;
                field_0x234[2] = 40;
                field_0x230 = 0;
            }
        }
        break;
    case 4:
        takeCamera();
        startSound_801F0E34(0x3008E, 0);
        if (field_0x234[1] == 0) {
            getGameView_802D6A9C()->fn_802D9FF4(field_0x250, field_0x254);
            fn_80119DD0()->fn_80119468(0, 1);
            getGameView_802D6A9C()->fn_802D6AAC(2);
            for (s32 i = 0; i < 4; i++) {
                fn_802372F8(i, lbl_804A39A0[field_0x258], 1);
            }
            field_0x234[0] = 5;
            field_0x230 = 5;
            gGameRoot2_8053AB10->field_0x20->field_0x216 = 1;
        }
        break;
    case 5:
        takeCamera();
        if (field_0x234[0] == 0) {
            gGameRoot_8053AAF8->fn_80136798(0xD, 0);
            mAlive = false;
        }
        gGameRoot2_8053AB10->field_0x20->field_0x216 = 1;
        break;
    case 6:
        gGameRoot2_8053AB10->field_0x20->field_0x216 = 1;
        field_0x230 = 7;
        break;
    case 7:
        gGameRoot2_8053AB10->field_0x20->field_0x216 = 1;
        field_0x230 = 8;
        // fall through
    case 8:
        getActorManager_801F856C()->fn_801F6884();
        mAlive = false;
        break;
    }
    for (s32 i = 0; i < 3; i++) {
        if (field_0x234[i] > 0) {
            field_0x234[i]--;
        }
    }
}

// 0x801B6C50.
void GoalWarp_804A39DC::draw() {}
