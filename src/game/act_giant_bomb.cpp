#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/RotMatrix.h"
#include "game/ActorManager.h"
#include "game/act/act_giant_bomb.h"

static const char* lbl_804AC558[] = {
    "kk_rupynum_00.bti", "kk_rupynum_01.bti", "kk_rupynum_02.bti",
    "kk_rupynum_03.bti", "kk_rupynum_04.bti", "kk_rupynum_05.bti",
    "kk_rupynum_06.bti", "kk_rupynum_07.bti", "kk_rupynum_08.bti",
    "kk_rupynum_09.bti", "rupy_num_exclamation.bti", "kk_rupynum_00.bti",
};

GiantBomb_804AC588::~GiantBomb_804AC588() {
    releaseEmitter_801EE434(mEmitter264);
    releaseEmitter_801EE434(mEmitter268);
    if (field_0x261 == 1) {
        s32 mode = gGameRoot_8053AAF8->field_0x4c;
        if (mode != 9 && mode != 0xB) {
            gGameRoot_8053AAF8->fn_80138B7C();
        }
    }
}

void GiantBomb_804AC588::init() {
    field_0x244 = field_0x90 & 0xFFF;
    field_0x118 = 0x80;
    mState = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    mTimer = 0;
    field_0x260 = 0;
    field_0x238 = 0;
    field_0x248 = (field_0x90 >> 12) & 3;
    field_0x24c = (field_0x90 >> 16) & 0xF;
    field_0x261 = 0;
    field_0x250 = 0x0100000D;
    mLife = 10000;
    field_0x80.set(-24.0f, -24.0f, 24.0f, 24.0f);
    mBox.set(-24.0f, -24.0f, 24.0f, 24.0f);
    mBox2.set(-24.0f, -24.0f, 24.0f, 24.0f);
    field_0x254.set(0.75f, 0.75f, 0.75f);
    switch (field_0x24c) {
    case 0:
        field_0x80.set(0.0f, 0.0f, 0.0f, 0.0f);
        mBox.set(0.0f, 0.0f, 0.0f, 0.0f);
        mBox2.set(0.0f, 0.0f, 0.0f, 0.0f);
        field_0x254.z = 0.0f;
        field_0x254.y = 0.0f;
        field_0x254.x = 0.0f;
        break;
    case 1:
        mPos.z = 400.0f;
        mState = 2;
        if (gGameRoot_8053AAF8->field_0x7f == 1) {
            field_0x261 = 1;
        }
        break;
    case 2:
        mState = 3;
        if (gGameRoot_8053AAF8->field_0x7f == 1) {
            field_0x261 = 1;
        }
        break;
    case 3:
        fn_8034335C(field_0x4, &mPos);
        mState = 7;
        mTimer = 3;
        break;
    }
    mEmitter264 = NULL;
    mEmitter268 = NULL;
}

// FIXME: 99.40 %. Only the register of the loop counter differs (r26 in the original) and the .sdata2 reloc names.
void GiantBomb_804AC588::fn_8034335C(s32 layer, const JGeometry::TVec3f* pos) {
    getGameView_802D6A9C()->fn_802D79E8(layer, 16.0f, 0.0f, false);
    gGameRoot_8053AAF8->fn_80134DE4(3, 0);

    u8 unk[4];
    unk[0] = 0;
    unk[1] = 0;
    unk[2] = 0;
    unk[3] = 0;

    for (int i = 0; i < 4; i++) {
        if (fn_8022D75C(i) == true && !fn_80234ECC(i)) {
            unk[i] = 1;
            if (fn_8023ED8C(i) == true) {
                unk[fn_8023ECF0(i)] = 1;
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        if (fn_8023F264(i) && !fn_8022F478(i) && getPlayerLayer_8023DE58(i) < 8 &&
            !fn_80230188(i) && !fn_8022B7C4(i) && !fn_802349EC(i) && !unk[i]) {
            // fn_80238548 reads pos+0/+4 (TVec2f) only
            fn_80238548(i, 0x20, (const JGeometry::TVec2f*)pos, 0x13);
        }
    }

    if (layer < 8) {
        gGameRoot2_8053AB10->field_0x20->fn_802F601C(pos, 6, NULL, 1.0f, 1.0f);
    }

    spawnEmitter_8013CC50(layer, pos, 0x33, 1, 1, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(layer, pos, 0x34, 1, 1, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(layer, pos, 0x35, 1, 1, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(layer, pos, 0x36, 1, 1, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(layer, pos, 0x37, 1, 1, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(layer, pos, 0x38, 1, 1, NULL, NULL, true, 1.0f);

    startSound_801F0E34(0x30001, 0);
}

// FIXME: 98.45 %. Two extra `mr` around the getResource results and the float registers of the matrix scale (scheduling).
void GiantBomb_804AC588::draw() {
    if (mState == 2 || mState == 7) {
        return;
    }
    s32 count = mTimer / 60 + 1;
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    PSMTXIdentity(mtx);
    JGeometry::TVec3f savedPos = mPos;
    if (field_0x254.x == 0.0f) {
        return;
    }
    f32 scaleY = field_0x254.y;
    f32 scaleZ = field_0x254.z;
    mtx[0][0] = field_0x254.x;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = scaleY;
    mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = scaleZ;
    params.mMtx = mtx;
    if (mState >= 5) {
        params.field_0x1d = 0;
        params.field_0x1c = 0;
    } else {
        params.field_0x1d = 1;
        params.field_0x1c = 1;
    }
    params.field_0x2d = 1;
    s32 flags = 7;
    if (mState == 3) {
        flags = -1;
    }
    mPos.y += 10.0f;
    f32 f27;
    if (mSpawnPos.z != 0.0f && mPos.z != 0.0f && field_0x24c == 0) {
        f27 = (48.0f * (-field_0x254.z / 0.75f)) * (mPos.z / mSpawnPos.z) - 12.0f * ((mSpawnPos.z - mPos.z) / mSpawnPos.z);
    } else {
        f27 = -12.0f;
    }
    if (field_0x23c == 1) {
        params.field_0x14.r = 150;
    }
    if (mState == 6 && count < 10) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', "bomb_big_a.bti");
        vfunc_0x54(res, &params, flags, 0.0f, f27 - 10.0f, 0.0f);
    } else {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', "bomb_big.bti");
        vfunc_0x54(res, &params, flags, 0.0f, f27 - 10.0f, 0.0f);
    }
    mPos = savedPos;
    if (mState == 6 && count < 10) {
        mtx[0][3] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][3] = 0.0f;
        JGeometry::TVec3f rot(0.0f, 0.0f, 0.0f);
        setRotScaleMtx(mtx, rot, 0.5f);
        params.mMtx = mtx;
        params.field_0x2c = 0;
        void* numberRes = fn_804030C4('TIMG', lbl_804AC558[count]);
        if (field_0x4 < 8) {
            gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, numberRes, &params);
        }
    }
}

void fn_80341CB0(s32 layer, const JGeometry::TVec3f* pos) {
    getActorManager_801F856C()->spawnActor_801F9484('BBMB', layer, pos, 0x30000, -1, -1);
}
