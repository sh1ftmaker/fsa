#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/RotMatrix.h"
#include "game/ActorManager.h"
#include "game/Player.h"
#include "game/GameView.h"
#include "game/Effect.h"
#include "JSystem/JMath/JMath.h"
#include "std/math.h"
#include "game/act/act_hardhat_beetle.h"

// FIXME(shotgun): 89.88 %, missing/extra + reloc-name
void HardhatBeetle_804A06C0::init() {
    mBox.set(-6.0f, -6.0f, 6.0f, 6.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x234[4] = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x234[0] = 0;
    field_0x234[1] = 0;
    field_0x234[2] = 0;
    field_0x234[3] = 0;
    mFrame = 0;
    field_0x278 = 0.2f;
    field_0x27c = 0.2f;
    field_0x280 = 0.2f;
    field_0x26c.set(0.0f, 0.0f, 0.0f);
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    field_0x25c = 0;
    mScale = 1.0f;
    mHome = mPos;
    field_0xb0 = 0x11101;
    field_0xb8 = 1;
    mState = 0;
}

// FIXME(shotgun): 92.67 %, missing/extra
void HardhatBeetle_804A06C0::update() {
    u16 angle = 0;
    s32 outDir = 0;
    f32 outDist = 0.0f;
    if (mState != 2) {
        if (field_0x234[2] != 0) {
            mHome.x = mPos.x;
            mHome.y = mPos.y;
            mHome.z = mPos.z;
            mHome.x = (f32)((s32)mHome.x & 0xFFF8);
            mHome.y = (f32)((s32)mHome.y & 0xFFF8);
            mHome.x += -16.0f * JMath::sincosTable_.sinShort(field_0x24c);
            mHome.y += -16.0f * JMath::sincosTable_.cosShort(field_0x24c);
            u16 code = fn_8022461C(&mHome, field_0x4);
            if ((code >= 2 && code <= 3) || code == 0xE) {
                if (fn_802DE9A0() == true && code == 0xE) {
                    field_0x25c = 1;
                }
                field_0xd8 = 0;
                field_0xdc = 0;
                field_0x3c.set(0.0f, 0.0f, 0.0f);
                field_0x26c.set(0.0f, 0.0f, 0.0f);
                mAngle = 0.0f;
                field_0x234[0] = 0;
                field_0x234[1] = 30;
                mState = 2;
            }
        }
        if (fn_801F3D94(true) != 0 || fn_801F2DB4('NULL', 'NULL', NULL)) {
            field_0x234[2] = 20;
            fn_801F2618(NULL, false);
            return;
        }
        addCalc2_801F71A4(&field_0x3c.x, field_0x26c.x, 0.1f, field_0x278);
        addCalc2_801F71A4(&field_0x3c.y, field_0x26c.y, 0.1f, field_0x27c);
        mPos += field_0x3c;
        fn_801EEC98(mSerial, &angle, &outDir, &outDist, NULL, 0, 0);
        if (fn_801F2B7C(-100, -1, 1) && mState == 0) {
            field_0x3c.x = -2.0f * JMath::sincosTable_.sinShort(angle);
            field_0x3c.y = -2.0f * JMath::sincosTable_.cosShort(angle);
            field_0x26c.set(0.0f, 0.0f, 0.0f);
            field_0x248 = 0;
            mState = 1;
        }
        fn_801F2618(NULL, false);
    }
    for (u8 i = 0; i < 5; i++) {
        if (field_0x234[i] > 0) {
            field_0x234[i]--;
        }
    }
    switch (mState) {
    case 0: {
        if (field_0x234[1] == 0) {
            field_0x26c.x = 0.5f * JMath::sincosTable_.sinShort(angle);
            field_0x26c.y = 0.5f * JMath::sincosTable_.cosShort(angle);
            if (outDir < 0) {
                field_0x26c.set(0.0f, 0.0f, 0.0f);
            }
            field_0x234[1] = 30;
        }
        s32 target = field_0x9c;
        ActorBase_804A5770* other = NULL;
        if (target >= 0) {
            other = getActorManager_801F856C()->findActorBySerial_801F666C(target);
        }
        if (other != NULL) {
            angle = (s32)(JMath::atanTable_.atan2_(other->mPos.x - mPos.x, other->mPos.y - mPos.y) * 10430.378f);
            field_0x3c.x = -2.0f * JMath::sincosTable_.sinShort(angle);
            field_0x3c.y = -2.0f * JMath::sincosTable_.cosShort(angle);
            field_0x26c.set(0.0f, 0.0f, 0.0f);
            field_0x248 = 0;
            if (field_0x234[3] == 0) {
                startSound_801F0E34(0x400D4, 0);
                field_0x234[3] = 8;
            }
            mState = 1;
            break;
        }
        for (s32 i = 0; i < 4; i++) {
            s32 player = fn_801F285C(NULL, NULL, true, i);
            if (player < 0) {
                continue;
            }
            JGeometry::TVec3f p = mPos;
            f32 x0 = mBox.i.x + p.x;
            f32 x1 = mBox.f.x + p.x;
            f32 y0 = mBox.i.y + p.y;
            f32 y1 = mBox.f.y + p.y;
            JGeometry::TVec2f mid(0.5f * (x0 - x1) + x1, 0.5f * (y0 - y1) + y1);
            if (fn_80237948(player, &mid) && field_0x234[3] == 0) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x117, 0, 1, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x118, 0, 1, NULL, NULL, true, 1.0f);
                startSound_801F0E34(0x400D4, 0);
                field_0x234[3] = 8;
            }
            angle += (s32)(4096.0f - 8192.0f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f));
            field_0x3c.x = -6.0f * JMath::sincosTable_.sinShort(angle);
            field_0x3c.y = -6.0f * JMath::sincosTable_.cosShort(angle);
            field_0x26c.set(0.0f, 0.0f, 0.0f);
            field_0x248 = player + 1;
            field_0x24c = angle;
            mState = 1;
        }
        break;
    }
    case 1:
        if (field_0x248 != 0) {
            field_0x234[2] = 10;
        }
        if (field_0x3c.x < 0.2f && field_0x3c.y < 0.2f) {
            mState = 0;
        }
        break;
    case 2: {
        addCalc2_801F71A4(&mPos.x, mHome.x, 0.2f, 1.0f);
        addCalc2_801F71A4(&mPos.y, mHome.y, 0.2f, 1.0f);
        f32 dx = mPos.x - mHome.x;
        f32 dy = mPos.y - mHome.y;
        if (field_0x234[1] == 0 && std::sqrtf(dx * dx + dy * dy) < 2.0f) {
            if (mFrame != 3) {
                startSound_801F0E34(0x4001A, 0);
            }
            mFrame = 3;
            mAngle += 0.1f;
            mScale -= 0.02f;
            if (mScale < 0.1f) {
                if (field_0x25c == 1) {
                    spawnEmitter_8013CC50(field_0x4, &mPos, 0x2A9, -1, 1, NULL, NULL, true, 1.0f);
                    spawnEmitter_8013CC50(field_0x4, &mPos, 0x2AA, -1, 0, NULL, NULL, true, 1.0f);
                    startSound_801F0E34(0x5005F, 0);
                }
                fn_801F3668();
            }
        } else if (field_0x234[0] == 0) {
            field_0x234[0] = 3;
            mFrame++;
            if (mFrame > 2) {
                mFrame = 0;
            }
        }
        break;
    }
    }
}

// FIXME: 99.0 %. regalloc-only: the original keeps mScale in f29 and cos x
// in f31, ours the other way round, and sin x / sin z get f28 / f26 there,
// f26 / f28 here.
void HardhatBeetle_804A06C0::draw() {
    if (mState == 2) {
        SpriteParams_801F5AE0 params;
        Mtx mtx;
        mtx[0][3] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][3] = 0.0f;
        // The original stores &mtx to params.mMtx twice, here (0x80167AEC)
        // and after the rotation (0x80167B44).
        params.mMtx = mtx;
        JGeometry::TVec3f rot(0.0f, mAngle, 0.0f);
        setRotScaleMtx(mtx, rot, mScale);
        params.mMtx = mtx;
        s32 frame = mFrame + 0x92;
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
        }
    } else {
        drawSprite_801F06F0("enemy_gc_sob.bin", mFrame + 0x92, NULL, 0, 1.0f, 1.0f);
    }
}

HardhatBeetle_804A06C0::~HardhatBeetle_804A06C0() {}
