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

void GiantBomb_804AC588::update() {
    JGeometry::TBox2f box = mBox;
    JGeometry::TVec2f pos(mPos.x, mPos.y);
    box.addPos(pos);

    mLife = 10000;

    if (mState == 4) {
        mPos += field_0x3c;
    }

    if (mTimer > 0) {
        mTimer--;
    }

    if (mState != 2) {
        JGeometry::TVec3f v1 = mPos;
        v1.x += mBox.i.x;
        v1.y += mBox.i.y;
        JGeometry::TVec3f v2 = mPos;
        v2.x += mBox.f.x;
        v2.y += mBox.f.y;
        lbl_8053AED0->fn_8043E398(v1, v2);
    }

    if (mState == 6 && mTimer > 0) {
        fn_802397B4(&box, field_0x4, mSerial);
        if (field_0x238 > 0) {
            field_0x238--;
        }
        if (!fn_801F3D94(0) && field_0x238 <= 0) {
            field_0xb0 = 0x41304;
            s32 r27 = fn_801A4D1C(mSerial, &box, &pos);
            s32 r28 = fn_801A6108(mSerial, &box, &pos);
            s32 r29 = fn_801A4F5C(mSerial, &box, &pos);
            s32 r30 = fn_801A46FC(mSerial, &box, &pos);
            if (fn_801F305C() != 0 || r27 > 0 || r28 > 0 || r29 > 0 || r30 > 0) {
                mTimer -= 60;
                field_0x238 = 15;
                goto end;
            } else {
                field_0xb0 = 0;
            }
        }
    } else if (mState == 5) {
        s32 t = (15 - mTimer) / 15;
        JGeometry::TBox2f box2;
        box2.set(mBox.i.x * (f32)t, mBox.i.y * (f32)t, mBox.f.x * (f32)t, mBox.f.y * (f32)t);
        fn_802397B4(&box2, field_0x4, mSerial);
    }

    switch (mState) {
    case 0:
        field_0x254.x += 0.75f / (f32)field_0x244;
        field_0x254.y = field_0x254.x;
        field_0x254.z = field_0x254.x;
        mBox.i.x = -32.0f * field_0x254.x;
        mBox.i.y = -32.0f * field_0x254.y;
        mBox.f.x = 32.0f * field_0x254.x;
        mBox.f.y = 32.0f * field_0x254.y;
        if (field_0x254.x >= 0.75f) {
            field_0x254.x = 0.75f;
            field_0x254.y = 0.75f;
            field_0x254.z = 0.75f;
            mBox.set(-24.0f, -24.0f, 24.0f, 24.0f);
            mTimer = 8;
            mState = 1;
        }
        field_0x80 = mBox;
        mBox2 = mBox;
        break;

    case 1:
        if (mTimer <= 0) {
            field_0x3c.z = 3.0f;
            mState = 4;
        }
        break;

    case 2:
        if (vfunc_0x34() == true || vfunc_0x3C() == 0) {
            if (field_0x24c == 1) {
                if (getGameView_802D6A9C()->field_0x4 == 1 && getGameView_802D6A9C()->field_0x8 == 0) {
                    fn_801F3604();
                }
            }
            mState = 4;
            field_0x250 = gAudioMgr_8053AF80->field_0x58;
        }
        break;

    case 4:
        if (mPos.z <= 0.0f) {
            getGameView_802D6A9C()->fn_802D79E8(field_0x4, 8.0f, 0.0f, false);
            JGeometry::TVec3f emitterPos = mPos;
            emitterPos.y += 16.0f;
            spawnEmitter_8013CC50(field_0x4, &emitterPos, 0x3cf, 0, -1, NULL, NULL, true, 2.5f);
            startSound_801F0E34(0x3000f, 0);
            if (field_0x261 == 1) {
                gAudioMgr_8053AF80->fn_80458FF0(0x1000021, false, 0);
            }
            field_0x3c.z = 0.0f;
            mTimer = 15;
            if (field_0x248 == 0) {
                lbl_8053AED0->fn_8043D67C(0x1a8, 0, 0);
            } else if (field_0x248 == 1) {
                lbl_8053AED0->fn_8043D67C(0x1c1, 0, 0);
            }
            mState = 5;
        } else {
            field_0x3c.z -= 0.2f;
            if (field_0x24c != 2) {
                fn_801F2B7C(1, -1, 2);
                fn_801F2B20(0);
            }
        }
        break;

    case 5:
        if (mTimer <= 0) {
            mTimer = 600;
            JGeometry::TVec3f pos = mPos;
            pos.y -= 14.0f;
            if (mEmitter264 == NULL) {
                mEmitter264 = spawnEmitter_8013CC50(field_0x4, &pos, 0x39, 1, 1, NULL, NULL, true, 1.0f);
            }
            if (mEmitter268 == NULL) {
                mEmitter268 = spawnEmitter_8013CC50(field_0x4, &pos, 0x3a, 1, 1, NULL, NULL, true, 1.0f);
            }
            field_0x254.set(0.75f, 0.75f, 0.75f);
            mState = 6;
        } else {
            fn_801F2B7C(1, -1, 2);
            fn_801F2B20(0);
            if (mTimer > 10) {
                f32 t = (f32)(15 - mTimer) * 0.2f / 5.0f;
                field_0x254.x = 0.75f * (1.0f + t);
                field_0x254.y = 0.75f * (1.0f - t);
                field_0x254.z = 0.75f * (1.0f - t);
            } else if (mTimer > 0) {
                f32 t = (f32)mTimer * 0.2f / 10.0f;
                field_0x254.x = 0.75f * (1.0f + t);
                field_0x254.y = 0.75f * (1.0f - t);
                field_0x254.z = 0.75f * (1.0f - t);
            }
        }
        break;

    case 6:
        fn_8020D58C()->addBox(mSerial);
        startSound_801F0E34(0x50026, 0);
        if (mTimer <= 0) {
            releaseEmitter_801EE434(mEmitter264);
            releaseEmitter_801EE434(mEmitter268);
            fn_8034335C(field_0x4, &mPos);
            mState = 7;
            mTimer = 3;
        } else {
            JGeometry::TVec3f pos = JGeometry::TVec3f(mPos.x + 4.0f, mPos.y - (6.0f + 8.0f * (f32)mTimer / 600.0f), mPos.z);
            if (mEmitter264 != NULL) {
                setEmitterPos_8013CB44(mEmitter264, &pos, 1, 1, true);
            }
            if (mEmitter268 != NULL) {
                setEmitterPos_8013CB44(mEmitter268, &pos, 1, 1, true);
            }
        }
        break;

    case 7: {
        f32 f30 = -mPos.y;
        f32 f31 = -mPos.x;
        GameView_804A9950* view = getGameView_802D6A9C();
        mBox = view->mVisibleBox;
        mBox.i.x += f31;
        mBox.i.y += f30;
        mBox.f.x += f31;
        mBox.f.y += f30;
        fn_801F2B20(5);
        if (mTimer <= 0) {
            if (field_0x24c == 1 && getGameView_802D6A9C()->field_0x4 == 1 && getGameView_802D6A9C()->field_0x8 == 0) {
                fn_801F3668();
            } else {
                mAlive = false;
            }
        }
        break;
    }
    }

    if (mState != 2) {
        JGeometry::TVec3f focusPoint(mPos.x, mPos.y - mPos.z, 0.0f);
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&focusPoint);
    }

    if (mTimer < 300 && mState == 6) {
        field_0x240++;
        if (mTimer < 120) {
            if (field_0x240 & 1) {
                field_0x240++;
            }
        }
        field_0x240 &= 7;
        if (field_0x240 == 0) {
            field_0x23c ^= 1;
        }
    }

end:;
}

static const char* lbl_804AC558[] = {
    "kk_rupynum_00.bti", "kk_rupynum_01.bti", "kk_rupynum_02.bti",
    "kk_rupynum_03.bti", "kk_rupynum_04.bti", "kk_rupynum_05.bti",
    "kk_rupynum_06.bti", "kk_rupynum_07.bti", "kk_rupynum_08.bti",
    "kk_rupynum_09.bti", "rupy_num_exclamation.bti", "kk_rupynum_00.bti",
};

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

void GiantBomb_804AC588::vfunc_0x54(void* res, SpriteParams_801F5AE0* params, s32 flags, f32 offsetX,
                                    f32 offsetY, f32 offsetZ) {
    if (field_0x4 != 0) {
        return;
    }
    if (mSpriteParams.mMtx == NULL) {
        fn_801EF830(field_0x4, &mPos, params, flags, NULL, 0, res);
    }
    SpriteParams_801F5AE0 localParams;
    if (field_0xd8 > 0) {
        if (params != NULL) {
            localParams = *params;
        }
        s32 r3 = field_0xdc;
        s32 r0 = field_0xd8;
        s32 r5 = r3 - 25;
        if (r0 > r5 || r3 < 25) {
            if (r3 < 25) {
                localParams.field_0x14.r += (r0 * 150) / r3;
            } else {
                localParams.field_0x14.r += ((r0 - r5) * 150) / 25;
            }
        }
        if (field_0x3c.squared() > 1.0f) {
            localParams.field_0x30 = 8;
        }
    } else if (field_0x238 > 0) {
        if (params != NULL) {
            localParams = *params;
        }
        localParams.field_0x14.r += (field_0x238 * 150) / 15;
        if (field_0x3c.squared() > 1.0f) {
            localParams.field_0x30 = 8;
        }
    } else {
        if (params != NULL) {
            localParams = *params;
        } else {
            localParams.field_0x30 = mSpriteParams.field_0x30;
        }
        if (mSpriteParams.mMtx != NULL) {
            localParams.mMtx = mSpriteParams.mMtx;
        }
    }
    JGeometry::TVec3f drawPos = mPos;
    drawPos.x += offsetX;
    drawPos.y += offsetY;
    drawPos.z += offsetZ;
    if (field_0x4 < 8) {
        gGameRoot2_8053AB10->field_0x20->fn_802F745C(&drawPos, res, &localParams);
    }
}

void fn_80341CB0(s32 layer, const JGeometry::TVec3f* pos) {
    getActorManager_801F856C()->spawnActor_801F9484('BBMB', layer, pos, 0x30000, -1, -1);
}
