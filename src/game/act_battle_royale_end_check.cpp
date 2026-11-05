#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/act/act_battle_royale_end_check.h"
#include "game/act/tu_803DA6B4.h"

BattleRoyaleEndCheck_804B0380::~BattleRoyaleEndCheck_804B0380() {}

// The state table: update() calls the function for mState. The original's copy
// of it is zero in .data and the global constructor fn_803D6DC0 fills it at run
// time from a constant table; the initialised table here leaves .data short.
static void (BattleRoyaleEndCheck_804B0380::*lbl_804B032C[7])() = {
    &BattleRoyaleEndCheck_804B0380::fn_803D6BDC,
    &BattleRoyaleEndCheck_804B0380::fn_803D6AE8,
    &BattleRoyaleEndCheck_804B0380::fn_803D69AC,
    &BattleRoyaleEndCheck_804B0380::fn_803D6730,
    &BattleRoyaleEndCheck_804B0380::fn_803D66A8,
    &BattleRoyaleEndCheck_804B0380::fn_803D6618,
    &BattleRoyaleEndCheck_804B0380::fn_803D6494,
};

// The scripted camera move (Camera.h): the target goes in 0x7F8 while 0x7F5 is clear.
static inline void startCameraMove(Camera_804AA9E4* camera, s32 target) {
    if (camera->field_0x7f5 == 0) {
        camera->field_0x7f4 = 1;
        camera->field_0x7f8 = target;
        camera->field_0x7f6 = 0;
    }
}

void BattleRoyaleEndCheck_804B0380::fn_803D6494() {
    s32 winner = mWinner;
    if (winner != -1) {
        startCameraMove(lbl_8053AB10->field_0x24, winner);
    } else {
        startCameraMove(lbl_8053AB10->field_0x24, -1);
    }
    if (lbl_8053AD30->fn_80309C4C(0x1100, -1) != 0) {
        lbl_8053AAF8->fn_801367E0(0xA);
        if (mWinner != -1) {
            lbl_80529DEC.field_0x140[mWinner]++;
            lbl_80529DEC.field_0x243[0] = 0;
            lbl_80529DEC.field_0x64 = mWinner;
            lbl_80529DEC.field_0x243[0] = 1 << mWinner;
        } else {
            s32 pick = (s32)(3.0f * lbl_8053AAF8->mRandom.get_ufloat_1());
            if (pick >= 3) {
                pick = 2;
            }
            lbl_80529DEC.field_0x64 = pick;
        }
        lbl_8053AD78->field_0xe4 = 0;
        gAudioMgr_8053AF80->fn_80458880(0xB, -1);
        mAlive = false;
    }
}

void BattleRoyaleEndCheck_804B0380::fn_803D6618() {
    s32 winner = mWinner;
    if (winner != -1) {
        startCameraMove(lbl_8053AB10->field_0x24, winner);
    } else {
        startCameraMove(lbl_8053AB10->field_0x24, -1);
    }
    if (mTimer <= 0) {
        return;
    }
    mTimer--;
    if (mTimer != 0) {
        return;
    }
    mState = 6;
}

void BattleRoyaleEndCheck_804B0380::fn_803D66A8() {
    mTimer--;
    startCameraMove(lbl_8053AB10->field_0x24, -1);
    if (mTimer <= 0) {
        mFocus = 4;
        mTimer = 0x82;
        mState = 5;
        gAudioMgr_8053AF80->fn_80458880(0x2D, -1);
    }
}

void BattleRoyaleEndCheck_804B0380::fn_803D6730() {
    // FIXME: 99.78 %. Regalloc only: the original loads mWinner into r4 and the
    // camera into r5 before the move; this puts them the other way round.
    mTimer--;
    startCameraMove(lbl_8053AB10->field_0x24, mWinner);
    if (mTimer <= 0) {
        field_0x238++;
        mTimer = 2;
        for (s32 i = 0; i < 4; i++) {
            if (mPlayerFlag[i] != 0) {
                if (i == mWinner || fn_8022F3DC(i) == mWinner) {
                    fn_80230498(i, getPlayerPos_8023E724(i), field_0x238 + 0xDB, 0xFF, -0x80, -0x80, true);
                    if (field_0x238 == 1) {
                        fn_80230FD0(mWinner, 0x10001);
                    } else if (field_0x238 == 0xD) {
                        JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
                        JGeometry::TVec3f p(pos.x + 7.0f, pos.y - 27.0f, pos.z);
                        spawnEmitter_8013CC50(field_0x4, &p, 0xCB, 1, 0, NULL, NULL, true, 1.0f);
                        JGeometry::TVec3f q(pos.x + 7.0f, pos.y - 27.0f, pos.z);
                        spawnEmitter_8013CC50(field_0x4, &q, 0xCC, 1, 0, NULL, NULL, true, 1.0f);
                        fn_80230FD0(mWinner, 0x2000E);
                    }
                }
            }
        }
        if (field_0x238 == 0xD) {
            mTimer = 0x82;
            mState = 5;
            switch (mWinner) {
            case 0:
                mFocus = 0;
                break;
            case 1:
                mFocus = 1;
                break;
            case 2:
                mFocus = 2;
                break;
            case 3:
                mFocus = 3;
                break;
            }
            gAudioMgr_8053AF80->fn_80458880(0x2A, -1);
            gAudioMgr_8053AF80->fn_80458FF0(0x1000027, true, false);
        }
    }
}

void BattleRoyaleEndCheck_804B0380::fn_803D69AC() {
    mTimer--;
    startCameraMove(lbl_8053AB10->field_0x24, -1);
    if (mTimer <= 0) {
        if (mWinner == -1) {
            mState = 4;
            gAudioMgr_8053AF80->fn_80458F9C(0x3C);
            lbl_8053AAF8->fn_801365C8(0);
            mTimer = 1;
            return;
        }
        for (s32 i = 0; i < 4; i++) {
            if (mPlayerFlag[i] != 0) {
                if (i == mWinner || fn_8022F3DC(i) == mWinner) {
                    fn_80230498(i, getPlayerPos_8023E724(i), 0x3E, 0xFF, -0x80, -0x80, true);
                }
            }
        }
        mState = 3;
        gAudioMgr_8053AF80->fn_80458F9C(0x3C);
        lbl_8053AAF8->fn_801365C8(0);
        mTimer = 1;
    }
}

void BattleRoyaleEndCheck_804B0380::fn_803D6AE8() {
    s32 count = 0;
    s32 matched = 0;
    for (s32 i = 0; i < 4; i++) {
        mPlayerFlag[i] = 0;
        if (!fn_80236B50(i)) {
            count++;
            if (fn_802300EC(i) == true) {
                mPlayerFlag[i] = 1;
                matched++;
            }
        }
    }
    if (matched == count) {
        for (s32 i = 0; i < 4; i++) {
            if (mPlayerFlag[i] != 0) {
                fn_80230AD4(i);
            }
        }
        startCameraMove(lbl_8053AB10->field_0x24, -1);
        lbl_8053AED0->field_0xdc |= 0x10;
        mState = 2;
    }
}

void BattleRoyaleEndCheck_804B0380::fn_803D6BDC() {
    if (fn_80239F10()) {
        for (s32 i = 0; i < 4; i++) {
            u8 bit = 1 << i;
            if (lbl_8053AAF8->field_0x82 & bit) {
                if (!fn_80236B50(i)) {
                    mWinner = i;
                }
            }
        }
        if (mWinner >= 0) {
            lbl_80529DEC.field_0x1a8 = mWinner;
        } else {
            lbl_80529DEC.field_0x1a8 = 4;
        }
        // FIXME: 99.59 %. Regalloc only: the original keeps the actor bound in
        // r30 and the byte offset of mActors[i] in r29; this puts them the other way.
        s32 actorEnd = getActorManager_801F856C()->mActorEnd;
        for (s32 i = 0; i < actorEnd; i++) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
            if (actor != NULL && actor->mActorId == 'SWEV') {
                fn_802B3A48(actor);
            }
        }
        getGameView_802D6A9C()->fn_802D7440(false);
        lbl_80529DEC.field_0x240 = 1;
        mState = 1;
    }
}

void BattleRoyaleEndCheck_804B0380::draw() {
    if (mFocus != 0xFF) {
        lbl_8053AED0->field_0x256 = mFocus;
    }
    if (mState == 6) {
        lbl_8053ADC8->field_0x20 = 1;
        lbl_8053ADC8->field_0x21 = 3;
    }
}

void BattleRoyaleEndCheck_804B0380::update() {
    (this->*lbl_804B032C[mState])();
}

void BattleRoyaleEndCheck_804B0380::init() {
    mState = 0;
    mWinner = -1;
    mTimer = 0x82;
    field_0x238 = 0;
    mFocus = 0xFF;
    for (s32 i = 3; i >= 0; i--) {
        mPlayerFlag[i] = 0;
    }
    field_0x11f = 1;
    lbl_80529DEC.field_0x1a8 = -1;
}
