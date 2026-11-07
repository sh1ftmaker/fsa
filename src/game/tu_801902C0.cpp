#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameMode.h"
#include "game/GameRootSub20.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/SpriteParams.h"
#include "JSystem/J3D/J3DAnmTextureSRTKey.h"
#include "JSystem/J3D/J3DAnmTevRegKey.h"
#include <math.h>
#include <stdio.h>

// FIXME: 99.43. Only the .rodata offsets of the string literals: the original
// has "rock6_smp.bin" (used by the unwritten draw fn_801902D0) before them.
GameRootSub20_801902C0::GameRootSub20_801902C0(JKRHeap* parent) {
    mHeap = JKRSolidHeap::create(-1, parent, false);
    mHeap->becomeCurrentHeap();
    for (s32 i = 0; i < 7; i++) {
        char bmd[0x20];
        char btk[0x20];
        char brk[0x20];
        sprintf(bmd, "suisyo_%d.bmd", i + 1);
        sprintf(btk, "suisyo_%d.btk", i + 1);
        sprintf(brk, "suisyo_%d.brk", i + 1);
        if (lbl_80529DEC.field_0x10->getResource('J3D ', bmd) == NULL) {
            sprintf(bmd, "suisyo_op.bmd");
            sprintf(btk, "suisyo_op.btk");
            sprintf(brk, "suisyo_op.brk");
        }
        fn_8016A3BC(lbl_80529DEC.field_0x10, bmd, btk, NULL, brk, NULL, 0x20240000, mHeap,
                    &mCrystals[i].mModel, &mCrystals[i].mBtk, NULL, &mCrystals[i].mBrk, NULL);
    }
    fn_8016A3BC(lbl_80529DEC.field_0x10, "taki_a.bmd", "taki_a.btk", NULL, NULL, NULL, 0x20240000, mHeap,
                &mSmallRequests[0].field_0xc, &mSmallRequests[0].field_0x14, NULL, NULL, NULL);
    fn_8016A3BC(lbl_80529DEC.field_0x10, "niji_a.bmd", NULL, NULL, NULL, NULL, 0x20240000, mHeap,
                &mSmallRequests[0].field_0x10, NULL, NULL, NULL, NULL);
    fn_8016A3BC(lbl_80529DEC.field_0x10, "taki_b.bmd", "taki_b.btk", NULL, NULL, NULL, 0x20240000, mHeap,
                &mSmallRequests[1].field_0xc, &mSmallRequests[1].field_0x14, NULL, NULL, NULL);
    fn_8016A3BC(lbl_80529DEC.field_0x10, "osiro_mon.bmd", "osiro_mon.btk", NULL, NULL, NULL, 0x240000, mHeap,
                &mRequests[0].field_0xc, &mRequests[0].field_0x10, NULL, NULL, NULL);
    fn_8016A3BC(lbl_80529DEC.field_0x10, "osiro_tobira.bmd", "osiro_tobira.btk", NULL, NULL, NULL, 0x240000, mHeap,
                &mRequests[1].field_0xc, &mRequests[1].field_0x10, NULL, NULL, NULL);
    fn_8016A3BC(lbl_80529DEC.field_0x10, "osiro_mado.bmd", "osiro_mado.btk", NULL, NULL, NULL, 0x240000, mHeap,
                &mRequests[2].field_0xc, &mRequests[2].field_0x10, NULL, NULL, NULL);
    for (s32 i = 0; i < 10; i++) {
        fn_8016A3BC(lbl_80529DEC.field_0x10, "suna.bmd", "suna.btk", NULL, "suna.brk", NULL, 0x240000, mHeap,
                    &mQuicksandEntries[i].mModel, &mQuicksandEntries[i].mBtk, NULL,
                    &mQuicksandEntries[i].mBrk, NULL);
    }
    for (s32 i = 0; i < 8; i++) {
        fn_8016A3BC(lbl_80529DEC.field_0x14, "ak_magicCircle.bmd", "ak_magicCircle.btk",
                    "ak_magicCircleApp.btk", "ak_magicCircle.brk", "ak_magicCircleColor.brk", 0x20240000,
                    mHeap, &mMagicCircles[i].mModel, &mMagicCircles[i].mBtk, &mMagicCircles[i].mBtkApp,
                    &mMagicCircles[i].mBrk, &mMagicCircles[i].mBrkColor);
    }
    for (s32 i = 0; i < 2; i++) {
        fn_8016A3BC(lbl_80529DEC.field_0x14, "mekakusi.bmd", "mekakusi.btk", NULL, NULL, NULL, 0x1240000, mHeap,
                    &mQueuedPos[i].mModel, &mQueuedPos[i].mBtk, NULL, NULL, NULL);
    }
    for (s32 i = 0; i < 16; i++) {
        fn_8016A3BC(lbl_80529DEC.field_0x10, "SandFall00.bmd", "SandFall00.btk", NULL, NULL, NULL, 0x240000, mHeap,
                    &mSandFalls[i].mModel, &mSandFalls[i].mBtk, NULL, NULL, NULL);
    }
    fn_8016A3BC(lbl_80529DEC.field_0x10, "saidanDark00.bmd", "saidanDark00.btk", NULL, "saidanDark00.brk", NULL,
                0x240000, mHeap, &field_0x7ac, &field_0x7b0, NULL, &field_0x7b4, NULL);
    fn_8016A3BC(lbl_80529DEC.field_0x10, "saidanFlash00.bmd", NULL, NULL, "saidanFlash00.brk", NULL, 0x10220000,
                mHeap, &field_0x7c4, NULL, NULL, &field_0x7c8, NULL);
    fn_8016A3BC(lbl_80529DEC.field_0x10, "snowWall00.bmd", "snowWall00.btk", NULL, "snowWall00.brk", NULL,
                0x10240000, mHeap, &mIceRequest.field_0x10, &mIceRequest.field_0x14, NULL,
                &mIceRequest.field_0x18, NULL);
    mHeap->adjustSize();
    parent->becomeCurrentHeap();
    mSound7b8 = new ActorSoundObj_804578F4(NULL, NULL, 8);
    mSound7cc = new ActorSoundObj_804578F4(NULL, NULL, 8);
}

// Destructor-shaped (null check on this, flag > 0 frees, returns this) but
// called directly with 1, so it is a plain member.
GameRootSub20_801902C0* GameRootSub20_801902C0::fn_80191744(s16 flag) {
    if (this != NULL) {
        if (mSound7cc != NULL) {
            delete mSound7cc;
        }
        if (mSound7b8 != NULL) {
            delete mSound7b8;
        }
        mHeap->destroy();
        if (flag > 0) {
            delete this;
        }
    }
    return this;
}

// FIXME: 99.88. Only the 0.0f literal: .sdata2 is not in the original order
// until the unwritten fn_801902D0 / fn_80190DC8 put their literals before it.
void GameRootSub20_801902C0::fn_8019169C() {
    for (s32 i = 0; i < 7; i++) {
        mCrystals[i].mActive = 0;
        mCrystals[i].field_0x48 = 0.0f;
        mCrystals[i].field_0x50 = 0;
    }
    mSmallRequests[0].mActive = 0;
    mSmallRequests[1].mActive = 0;
    mRequests[0].mActive = 0;
    mRequests[1].mActive = 0;
    mRequests[2].mActive = 0;
    mRequests[2].field_0x18 = 0;
    mQuicksandCount = 0;
    field_0x5ec = 0;
    mQueuedPosCount = 0;
    field_0x79c = 0;
    field_0x7a8 = 0;
    field_0x7a4 = 300;
    field_0x7a0 = 0;
    field_0x7c0 = 0;
    field_0x7bc = 0;
    mIceRequest.mActive = 0;
    field_0x4 = 0;
}

// Each frame: when none of the pause / fade / view conditions hold, clears all
// requests (the active flags and counters, as fn_8019169C does for the same
// fields); then counts the frame.
void GameRootSub20_801902C0::fn_801914C4() {
    // The pause test is a materialised bool (li 1 / compare) in the original.
    if ((!isGameModeSet() || !getGameView_802D6A9C()->fn_802DCD0C()) && gGameRoot_8053AAF8->field_0x94 == 0 &&
        gGameRoot_8053AAF8->field_0x96 == 0 &&
        (!gGameRoot_8053AAF8->field_0x7f ||
         (!fn_80239D38() && !(gGameRoot_8053AAF8->field_0x4c == 11 ? true : gGameRoot_8053AAF8->field_0x88 == 2))) &&
        lbl_80529DEC.field_0x240 == 0 && lbl_8053AEC8->field_0x5d == 0) {
        mCrystals[0].mActive = 0;
        mCrystals[1].mActive = 0;
        mCrystals[2].mActive = 0;
        mCrystals[3].mActive = 0;
        mCrystals[4].mActive = 0;
        mCrystals[5].mActive = 0;
        mCrystals[6].mActive = 0;
        mSmallRequests[0].mActive = 0;
        mSmallRequests[1].mActive = 0;
        mRequests[0].mActive = 0;
        mRequests[1].mActive = 0;
        mRequests[2].mActive = 0;
        mQuicksandCount = 0;
        field_0x5ec = 0;
        mQueuedPosCount = 0;
        field_0x79c = 0;
        field_0x7a8 = 0;
        field_0x7c0 = 0;
        mIceRequest.mActive = 0;
    }
    field_0x4++;
}

// FIXME(shotgun): 97.9 %, missing/extra (the two small requests are unrolled in the target)
void GameRootSub20_801902C0::fn_80190DC8() {
    s32 frame = field_0x4;
    for (s32 i = 0; i < 7; i++) {
        Crystal& c = mCrystals[i];
        if (c.mBtk != NULL && c.mActive != 0) {
            c.mBtk->mCurrentFrame = (f32)(frame % c.mBtk->mTotalFrameCount);
            if (c.field_0x48 >= 0.0f) {
                if (c.field_0x48 == 0.0f) {
                    c.mBrk->mCurrentFrame = 201.0f;
                } else if (c.field_0x48 < 1.0f) {
                    c.mBrk->mCurrentFrame = 200.0f * c.field_0x48 + 201.0f;
                } else {
                    c.field_0x50++;
                    c.mBrk->mCurrentFrame = (f32)(c.field_0x50 % 200 + 401);
                }
            } else if (c.field_0x4c >= 0) {
                c.mBrk->mCurrentFrame = (f32)c.field_0x4c;
            }
        }
    }
    for (s32 i = 0; i < 2; i++) {
        SmallRequest& r = mSmallRequests[i];
        if (r.field_0x14 != NULL && r.mActive != 0) {
            r.field_0x14->mCurrentFrame = (f32)(frame % r.field_0x14->mTotalFrameCount);
        }
    }
    if (mRequests[0].field_0x10 != NULL && mRequests[0].mActive != 0) {
        mRequests[0].field_0x10->mCurrentFrame = 121.0f * mRequests[0].mValue;
    }
    if (mRequests[1].field_0x10 != NULL && mRequests[1].mActive != 0) {
        mRequests[1].field_0x10->mCurrentFrame = (f32)(s32)(37.0f * mRequests[1].mValue);
    }
    if (mRequests[2].field_0x10 != NULL && mRequests[2].mActive != 0) {
        if (mRequests[2].mValue < 1.0f) {
            mRequests[2].field_0x10->mCurrentFrame = 121.0f * mRequests[2].mValue;
            mRequests[2].field_0x18 = 0;
        } else {
            mRequests[2].field_0x10->mCurrentFrame = (f32)(mRequests[2].field_0x18 % 120 + 121);
            mRequests[2].field_0x18++;
        }
    }
    for (s32 i = 0; i < mQuicksandCount; i++) {
        QuicksandEntry& e = mQuicksandEntries[i];
        if (e.mBtk != NULL) {
            e.mBtk->mCurrentFrame = (f32)((s32)((f32)frame * e.field_0x14) % e.mBtk->mTotalFrameCount);
            e.mBrk->mCurrentFrame = (f32)(s32)(60.0f - 59.0f * e.field_0x10);
        }
    }
    for (s32 i = 0; i < field_0x5ec; i++) {
        MagicCircle& m = mMagicCircles[i];
        if (m.mBtk != NULL) {
            m.mBtk->mCurrentFrame = (f32)(frame % m.mBtk->mTotalFrameCount);
            m.mBtkApp->mCurrentFrame = m.field_0x10;
            m.mBrk->mCurrentFrame = (f32)(frame % m.mBrk->mTotalFrameCount);
            m.mBrkColor->mCurrentFrame = (f32)m.field_0xc;
        }
    }
    for (s32 i = 0; i < mQueuedPosCount; i++) {
        if (mQueuedPos[i].mBtk != NULL) {
            mQueuedPos[i].mBtk->mCurrentFrame = (f32)(frame % mQueuedPos[i].mBtk->mTotalFrameCount);
        }
    }
    for (s32 i = 0; i < field_0x79c; i++) {
        if (mSandFalls[i].mBtk != NULL) {
            mSandFalls[i].mBtk->mCurrentFrame = (f32)(frame % mSandFalls[i].mBtk->mTotalFrameCount);
        }
    }
    if (field_0x7b0 != NULL && field_0x7a8 != 0) {
        field_0x7b0->mCurrentFrame = (f32)(frame % field_0x7b0->mTotalFrameCount);
        if (field_0x7a4 > 0) {
            field_0x7a4--;
        } else if (field_0x7a0 < field_0x7b4->mTotalFrameCount) {
            field_0x7a0++;
        } else {
            field_0x7a0 = 0;
            field_0x7a4 = (s32)(120.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
        }
        if (field_0x7a0 == 0) {
            field_0x7b4->mCurrentFrame = (f32)field_0x7b4->mTotalFrameCount;
        } else {
            field_0x7b4->mCurrentFrame = (f32)(field_0x7a0 - 1);
        }
    }
    if (field_0x7c8 != NULL) {
        if (field_0x7c0 != 0 || (field_0x7bc > 0 && field_0x7bc < field_0x7c8->mTotalFrameCount)) {
            field_0x7bc++;
        }
        field_0x7c8->mCurrentFrame = (f32)field_0x7bc;
    }
    if (mIceRequest.field_0x14 != NULL && mIceRequest.mActive != 0) {
        mIceRequest.field_0x14->mCurrentFrame = (f32)mIceRequest.mPercent;
        mIceRequest.field_0x18->mCurrentFrame = (f32)mIceRequest.mPercent;
    }
}

void GameRootSub20_801902C0::fn_801902D0() {
    for (s32 i = 0; i < 7; i++) {
        Crystal& c = mCrystals[i];
        if (c.mActive != 0) {
            JGeometry::TVec3f scale = c.mPos;
            f32 z = c.mMtx[2][3];
            f32 x = c.mMtx[0][3];
            f32 y = c.mMtx[1][3];
            s32 a = c.field_0x54;
            s32 b = c.field_0x58;
            if (z > 0.0f && a < 1) {
                a++;
                b--;
            }
            f32 offset = (f32)((a << 10) + (b << 7));
            Mtx mtx1;
            mtx1[0][3] = 0.0f;
            mtx1[1][3] = 0.0f;
            mtx1[2][3] = 0.0f;
            PSMTXScale(mtx1, 1.0f, 0.70710677f, 0.70710677f);
            PSMTXConcat(mtx1, c.mMtx, mtx1);
            Mtx mtx2;
            mtx2[0][3] = 0.0f;
            mtx2[1][3] = 0.0f;
            mtx2[2][3] = 0.0f;
            f32 sn = sin(1.5707964f);
            f32 cs = cos(1.5707964f);
            mtx2[0][0] = 1.0f;
            mtx2[1][1] = cs;
            mtx2[1][2] = -sn;
            mtx2[2][1] = sn;
            mtx2[2][2] = cs;
            mtx2[2][0] = 0.0f;
            mtx2[0][2] = 0.0f;
            mtx2[1][0] = 0.0f;
            mtx2[0][1] = 0.0f;
            PSMTXConcat(mtx1, mtx2, mtx1);
            mtx1[0][3] = x;
            mtx1[1][3] = y + offset;
            mtx1[2][3] = z + offset;
            scale.x = -scale.x;
            fn_8013ABCC(c.mModel, scale, mtx1);
        }
    }
    if (mSmallRequests[0].field_0xc != NULL && mSmallRequests[0].mActive != 0) {
        JGeometry::TVec3f scale(1.0f, 1.0f, -1.0f);
        Mtx mtx;
        PSMTXTrans(mtx, mSmallRequests[0].mPos.x, mSmallRequests[0].mPos.y, mSmallRequests[0].mPos.z);
        scale.scale(10.24f);
        mtx[1][3] += 2048.0f;
        mtx[2][3] += 2048.0f;
        fn_8013ABCC(mSmallRequests[0].field_0x10, scale, mtx);
        mtx[1][3] -= 2048.0f;
        mtx[2][3] -= 2048.0f;
        fn_8013ABCC(mSmallRequests[0].field_0xc, scale, mtx);
    }
    if (mSmallRequests[1].field_0xc != NULL && mSmallRequests[1].mActive != 0) {
        JGeometry::TVec3f scale(1.0f, 1.0f, -1.0f);
        Mtx mtx;
        PSMTXTrans(mtx, mSmallRequests[1].mPos.x, mSmallRequests[1].mPos.y, mSmallRequests[1].mPos.z);
        if (!(lbl_80529DEC.field_0x118 == 9 && lbl_80529DEC.field_0x11c == 3) &&
            !(lbl_80529DEC.field_0x118 == 8 && lbl_80529DEC.field_0x11c == 2)) {
            mtx[1][3] += 1024.0f;
            mtx[2][3] += 1024.0f;
        }
        scale.scale(10.24f);
        fn_8013ABCC(mSmallRequests[1].field_0xc, scale, mtx);
    }
    if (mRequests[0].field_0xc != NULL && mRequests[0].mActive != 0) {
        JGeometry::TVec3f scale(1.0f, 1.0f, -1.0f);
        Mtx mtx;
        PSMTXTrans(mtx, mRequests[0].mPos.x, mRequests[0].mPos.y, mRequests[0].mPos.z);
        mtx[1][3] -= 256.0f;
        mtx[2][3] -= 256.0f;
        scale.scale(10.24f);
        fn_8013ABCC(mRequests[0].field_0xc, scale, mtx);
    }
    if (mRequests[1].field_0xc != NULL && mRequests[1].mActive != 0) {
        JGeometry::TVec3f scale(1.0f, 1.0f, -1.0f);
        Mtx mtx;
        PSMTXTrans(mtx, mRequests[1].mPos.x, mRequests[1].mPos.y, mRequests[1].mPos.z);
        mtx[1][3] -= 256.0f;
        mtx[2][3] -= 256.0f;
        scale.scale(10.24f);
        fn_8013ABCC(mRequests[1].field_0xc, scale, mtx);
    }
    if (mRequests[2].field_0xc != NULL && mRequests[2].mActive != 0) {
        JGeometry::TVec3f scale(1.0f, 1.0f, -1.0f);
        Mtx mtx;
        PSMTXTrans(mtx, mRequests[2].mPos.x, mRequests[2].mPos.y, mRequests[2].mPos.z);
        mtx[1][3] -= 256.0f;
        mtx[2][3] -= 256.0f;
        scale.scale(10.24f);
        fn_8013ABCC(mRequests[2].field_0xc, scale, mtx);
    }
    for (s32 i = 0; i < mQuicksandCount; i++) {
        QuicksandEntry& e = mQuicksandEntries[i];
        if (e.mModel != NULL) {
            Mtx mtx;
            PSMTXTrans(mtx, e.mPos.x, e.mPos.y, e.mPos.z);
            mtx[1][3] -= 256.0f;
            mtx[2][3] -= 256.0f;
            f32 s = (16.0f * e.field_0xc) / 10.0f;
            JGeometry::TVec3f scale(s, s, s);
            scale.z = -s;
            fn_8013ABCC(e.mModel, scale, mtx);
        }
    }
    for (s32 i = 0; i < field_0x5ec; i++) {
        MagicCircle& m = mMagicCircles[i];
        if (m.mModel != NULL) {
            JGeometry::TVec3f scale(1.0f, 0.7f, -1.0f);
            Mtx mtx;
            PSMTXTrans(mtx, m.mPos.x, m.mPos.y, m.mPos.z);
            f32 sn = sin(1.5707964f);
            f32 cs = cos(1.5707964f);
            mtx[0][0] = 1.0f;
            mtx[1][1] = cs;
            mtx[1][2] = -sn;
            mtx[2][1] = sn;
            mtx[2][2] = cs;
            mtx[2][0] = 0.0f;
            mtx[0][2] = 0.0f;
            mtx[1][0] = 0.0f;
            mtx[0][1] = 0.0f;
            scale.scale(1.5f);
            fn_8013ABCC(m.mModel, scale, mtx);
        }
    }
    for (s32 i = 0; i < mQueuedPosCount; i++) {
        QueuedPos& q = mQueuedPos[i];
        if (q.mModel != NULL) {
            JGeometry::TVec3f scale(1.0f, 1.0f, -1.0f);
            Mtx mtx;
            PSMTXTrans(mtx, q.mPos.x, q.mPos.y, q.mPos.z);
            mtx[0][3] -= 256.0f;
            mtx[1][3] += 768.0f;
            mtx[2][3] += 1024.0f;
            scale.scale(10.24f);
            fn_8013ABCC(q.mModel, scale, mtx);
        }
    }
    for (s32 i = 0; i < field_0x79c; i++) {
        SandFall& s = mSandFalls[i];
        if (s.mModel != NULL) {
            JGeometry::TVec3f scale(1.0f, 1.0f, -1.0f);
            SpriteParams_801F5AE0 params;
            Mtx mtx;
            PSMTXTrans(mtx, s.mPos.x, s.mPos.y, s.mPos.z);
            mtx[1][3] -= 1024.0f;
            mtx[2][3] -= 1024.0f;
            if (s.field_0xc[0] != 0) {
                scale.scale(0.33333334f);
            }
            JGeometry::TVec3f pos = s.mPos;
            if (s.field_0xc[0] != 0) {
                pos.x -= 16.0f;
                pos.y -= 16.0f;
            } else {
                pos.x -= 48.0f;
                pos.y -= 48.0f;
            }
            params.field_0x1c = 0xFF;
            params.field_0x1d = 0xFF;
            fn_8013ABCC(s.mModel, scale, mtx);
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SMP ', "rock6_smp.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F6CF4(&pos, res, &params);
        }
    }
    if (field_0x7ac != NULL && field_0x7a8 != 0) {
        GameView_804A9950* view = getGameView_802D6A9C();
        Mtx mtx;
        PSMTXTrans(mtx, 0.5f * (view->mVisibleBox.i.x + view->mVisibleBox.f.x),
                   0.5f * (view->mVisibleBox.i.y + view->mVisibleBox.f.y) + 1024.0f, 1024.0f);
        JGeometry::TVec3f scale(1.0f, 1.0f, -1.0f);
        fn_8013ABCC(field_0x7ac, scale, mtx);
    }
    if (field_0x7c4 != NULL && field_0x7bc > 0 && field_0x7bc < field_0x7c8->mTotalFrameCount) {
        GameView_804A9950* view = getGameView_802D6A9C();
        Mtx mtx;
        PSMTXTrans(mtx, 0.5f * (view->mVisibleBox.i.x + view->mVisibleBox.f.x),
                   0.5f * (view->mVisibleBox.i.y + view->mVisibleBox.f.y) + 1024.0f, 1024.0f);
        JGeometry::TVec3f scale(1.0f, 1.0f, -1.0f);
        fn_8013ABCC(field_0x7c4, scale, mtx);
    }
    if (mIceRequest.field_0x10 != NULL && mIceRequest.mActive != 0) {
        JGeometry::TVec3f scale(1.0f, 1.0f, -1.0f);
        Mtx mtx;
        PSMTXTrans(mtx, mIceRequest.mPos.x, mIceRequest.mPos.y, mIceRequest.mPos.z);
        fn_8013ABCC(mIceRequest.field_0x10, scale, mtx);
    }
}

GameRootSub20_801902C0* fn_801902C0() {
    return gGameRoot_8053AAF8->field_0x4->field_0x20;
}
