#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameMode.h"
#include "game/GameRootSub20.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include <stdio.h>

GameRootSub20_801902C0* fn_801902C0() {
    return lbl_8053AAF8->field_0x4->field_0x20;
}

// Each frame: when none of the pause / fade / view conditions hold, clears all
// requests (the active flags and counters, as fn_8019169C does for the same
// fields); then counts the frame.
void GameRootSub20_801902C0::fn_801914C4() {
    // The pause test is a materialised bool (li 1 / compare) in the original.
    if ((!isGameModeSet() || !getGameView_802D6A9C()->fn_802DCD0C()) && lbl_8053AAF8->field_0x94 == 0 &&
        lbl_8053AAF8->field_0x96 == 0 &&
        (!lbl_8053AAF8->field_0x7f ||
         (!fn_80239D38() && !(lbl_8053AAF8->field_0x4c == 11 ? true : lbl_8053AAF8->field_0x88 == 2))) &&
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
