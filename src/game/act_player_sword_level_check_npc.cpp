#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/StageFlag.h"
#include "game/tu_802EF76C.h"
#include "game/act/tu_801F9484.h"

static const s32 lbl_8046F628_0[9] = {1, 1, 1, 2, 1, 1, 2, 2, 0x64};
static const s32 lbl_8046F628_24[14] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 3, 5};
static const s32 lbl_8046F628_5C[8][4] = {
    {0, 4, 0, 0},
    {8, 4, 0, 2},
    {3, 6, 0, 1},
    {9, 1, 0, 2},
    {4, 6, 0, 0},
    {0, 0, 0, 5},
    {2, 0, 0, 1},
    {9, 0, 6, 3},
};
static const s32 lbl_8046F628_DC[8][4] = {
    {2, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {2, 0, 0, 0},
    {7, 0, 0, 0},
    {1, 0, 1, 0},
    {0, 0, 3, 3},
    {0, 1, 3, 0},
};

// FIXME(shotgun): 100 %, matches
void PlayerSwordLevelCheckNPC_804AA590::init() {
    field_0x258 = fn_801B7550();
    field_0x25c = field_0x258;
    if (fn_80237060(0) == 2) {
        mAlive = false;
    } else {
        field_0x244 = 0;
    }
    mEmitters[0] = NULL;
    field_0x264[0] = 2;
    mEmitters[1] = NULL;
    field_0x264[1] = 2;
    mEmitters[2] = NULL;
    field_0x264[2] = 2;
    mEmitters[3] = NULL;
    field_0x264[3] = 2;
    mSoundObj = new ActorSoundObj_804578F4(NULL, NULL, 8);
}

// FIXME(shotgun): 97.92 %, missing/extra
PlayerSwordLevelCheckNPC_804AA590::~PlayerSwordLevelCheckNPC_804AA590() {
    for (s32 i = 0; i < 4; i++) {
        if (mEmitters[i] != NULL) {
            releaseEmitter_801EE434(mEmitters[i]);
        }
    }
    delete mSoundObj;
}

// FIXME(shotgun): 96.88 %, PlayerSwordLevelCheckNPC_804AA590
void PlayerSwordLevelCheckNPC_804AA590::update() {
    if (field_0x244 >= 1) {
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
            gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&pos);
        }
    }
    switch (field_0x244) {
    case 0: {
        s32 total = 0;
        for (s32 i = 0; i < 4; i++) {
            if ((u8)(gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) != 0) {
                total += fn_80236228(i);
            }
        }
        if (total >= 2000 && fn_8023077C() == 1) {
            for (s32 i = 0; i < 4; i++) {
                JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
                fn_80230498(i, &pos, 0x2E, 0xFF, 2, -0x80, true);
            }
            field_0x24c = 0;
            field_0x250 = 1;
            gGameRoot_8053AAF8->fn_80134D14(2);
            field_0x258 = fn_801B7550();
            field_0x254 = field_0x258 / 30;
            if (field_0x254 == 0) {
                field_0x254 = 1;
            }
            field_0x25c = field_0x258;
            field_0x244 = 1;
            mSoundObj->startSound(0x50076);
            gAudioMgr_8053AF80->fn_8045873C(1);
            if (gGameRoot_8053AAF8->field_0x7e == 0 && (gGameRoot_8053AAF8->field_0x83 & 3) == 2 &&
                getPlayerLayer_8023DE58(0) >= 8) {
                field_0x264[0] = 1;
                field_0x264[1] = 1;
                field_0x264[2] = 1;
                field_0x264[3] = 1;
            }
        }
        break;
    }
    case 1:
        field_0x25c -= field_0x254;
        if (field_0x25c <= 0) {
            field_0x25c = 0;
            if (field_0x24c == 8) {
                field_0x244 = 2;
            }
        }
        lbl_8053AEC8->field_0x5c = 0xFF - field_0x25c;
        field_0x250--;
        if (field_0x250 == 0) {
            field_0x24c++;
            field_0x250 = lbl_8046F628_0[field_0x24c];
            for (s32 i = 0; i < 4; i++) {
                JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
                if (field_0x24c < 6) {
                    fn_80230498(i, &pos, field_0x24c + 0x2E, 0xFF, 2, -0x80, true);
                } else if (field_0x24c == 8) {
                    fn_80230498(i, &pos, 0x3B9, 0xFF, 2, -0x80, true);
                    spawnEmitter_8013CC50(field_0x4, &pos, 0x277, 2, 3, NULL, NULL, true, 1.0f);
                    spawnEmitter_8013CC50(field_0x4, &pos, 0x278, 2, 3, NULL, NULL, true, 1.0f);
                    spawnEmitter_8013CC50(field_0x4, &pos, 0x279, 2, 3, NULL, NULL, true, 1.0f);
                    spawnEmitter_8013CC50(field_0x4, &pos, 0x27A, 2, 3, NULL, NULL, true, 1.0f);
                    spawnEmitter_8013CC50(field_0x4, &pos, 0x27B, 2, 3, NULL, NULL, true, 1.0f);
                    spawnEmitter_8013CC50(field_0x4, &pos, 0x27C, 2, 3, NULL, NULL, true, 1.0f);
                    spawnEmitter_8013CC50(field_0x4, &pos, 0x27D, 2, 3, NULL, NULL, true, 1.0f);
                } else {
                    fn_80230498(i, &pos, field_0x24c + 0x3BA, 0xFF, 2, -0x80, true);
                }
            }
        }
        break;
    case 2:
        field_0x250--;
        if (field_0x250 == 0) {
            field_0x244 = 3;
            field_0x250 = 0x64;
        }
        break;
    case 3:
        field_0x250--;
        if (field_0x250 == 0) {
            field_0x250 = 4;
            field_0x244 = 4;
        }
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
            if (getPlayerLayer_8023DE58(i) >= 8) {
                if ((field_0x250 & field_0x264[i]) != 0) {
                    fn_80230498(i, &pos, 0x3BA, 0xFF, 2, -0x80, true);
                } else {
                    fn_80230498(i, &pos, 0x3BB, 0xFF, 2, -0x80, true);
                }
            } else {
                fn_80230498(i, &pos, 0x3BA, 0xFF, 2, -0x80, true);
            }
        }
        break;
    case 4:
        field_0x250--;
        if (field_0x250 == 0) {
            field_0x250 = 0x64;
            field_0x244 = 5;
        }
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
            if (getPlayerLayer_8023DE58(i) >= 8) {
                if ((field_0x250 & field_0x264[i]) != 0) {
                    fn_80230498(i, &pos, 0x3BC, 0xFF, 2, -0x80, true);
                } else {
                    fn_80230498(i, &pos, 0x3BD, 0xFF, 2, -0x80, true);
                }
            } else {
                fn_80230498(i, &pos, 0x3BC, 0xFF, 2, -0x80, true);
            }
        }
        break;
    case 5:
        field_0x250--;
        if (field_0x250 == 0) {
            field_0x244 = 6;
            field_0x250 = 0x3C;
        }
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
            if (getPlayerLayer_8023DE58(i) >= 8) {
                if ((field_0x250 & field_0x264[i]) != 0) {
                    fn_80230498(i, &pos, 0x3BE, 0xFF, 2, -0x80, true);
                } else {
                    fn_80230498(i, &pos, 0x3BF, 0xFF, 2, -0x80, true);
                }
            } else {
                fn_80230498(i, &pos, 0x3BE, 0xFF, 2, -0x80, true);
            }
        }
        if (field_0x250 == 0x5A) {
            lbl_8053AED0->fn_8043D67C(0x23F, 0, 0);
            fn_80230F44(0, 0x100002D);
            for (s32 i = 0; i < 4; i++) {
                if ((u8)(gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) != 0) {
                    fn_80236ED8(i, 2);
                }
                JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
                mEmitters[i] = spawnEmitter_8013CC50(field_0x4, &pos, 0x26A, 1, 3, NULL, NULL, true, 1.0f);
            }
        }
        break;
    case 6:
        field_0x250--;
        if (field_0x250 == 0) {
            field_0x244 = 7;
            field_0x24c = 0;
            field_0x250 = 2;
            field_0x260 = 0;
            for (s32 i = 0; i < 4; i++) {
                JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
                fn_80230498(i, &pos, 0xDB, 0xFF, 2, -0x80, true);
            }
        } else {
            for (s32 i = 0; i < 4; i++) {
                JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
                if (getPlayerLayer_8023DE58(i) >= 8) {
                    if ((field_0x250 & field_0x264[i]) != 0) {
                        fn_80230498(i, &pos, 0x3BE, 0xFF, 2, -0x80, true);
                    } else {
                        fn_80230498(i, &pos, 0x3BF, 0xFF, 2, -0x80, true);
                    }
                } else {
                    fn_80230498(i, &pos, 0x3BE, 0xFF, 2, -0x80, true);
                }
            }
        }
        break;
    case 7:
        field_0x250--;
        if (field_0x250 == 0) {
            field_0x260 += 0x1000;
            for (s32 i = 0; i < 4; i++) {
                JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
                if (field_0x24c < 11) {
                    if (mEmitters[i] != NULL) {
                        JGeometry::TVec3f emPos = *getPlayerPos_8023E724(i);
                        emPos.x += 20.0f * JMath::sincosTable_.cosShort(field_0x260);
                        emPos.y += 20.0f * JMath::sincosTable_.sinShort(field_0x260);
                        setEmitterPos_8013CB44(mEmitters[i], &emPos, 1, 3, true);
                    }
                    fn_80230498(i, &pos, lbl_8046F628_24[field_0x24c] + 0xDB, 0xFF, 2, -0x80, true);
                } else {
                    if (getPlayerLayer_8023DE58(i) >= 8) {
                        if ((field_0x250 & field_0x264[i]) != 0) {
                            fn_80230498(i, &pos, lbl_8046F628_24[field_0x24c] + 0x3B9, 0xFF, 2, -0x80, true);
                        } else {
                            fn_80230498(i, &pos, lbl_8046F628_24[field_0x24c] + 0x3BA, 0xFF, 2, -0x80, true);
                        }
                    } else {
                        fn_80230498(i, &pos, lbl_8046F628_24[field_0x24c] + 0x3B9, 0xFF, 2, -0x80, true);
                    }
                    if (field_0x24c == 11) {
                        if (mEmitters[i] != NULL) {
                            releaseEmitter_801EE434(mEmitters[i]);
                            mEmitters[i] = NULL;
                        }
                        fn_80230FD0(i, 0x10001);
                        if ((gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) != 0) {
                            fn_80236554(i, fn_80237810(i), false);
                        }
                    }
                }
            }
            field_0x24c++;
            field_0x250 = 2;
            if (field_0x24c >= 14) {
                if (getGameView_802D6A9C()->field_0x131f9 != 0) {
                    field_0x244 = 10;
                    field_0x250 = 1;
                } else {
                    field_0x244 = 8;
                    field_0x250 = 1;
                }
            }
        }
        break;
    case 8:
        if (field_0x250 == 0) {
            field_0x25c += field_0x254;
            if (field_0x25c >= field_0x258) {
                field_0x25c = field_0x258;
                if (fn_802DE7CC() && !isStageFlagSet(lbl_80529DEC.field_0x118, lbl_80529DEC.field_0x11c)) {
                    fn_8038BD08(0, 0x265, field_0x4, mPos, 0x21);
                    field_0x244 = 15;
                } else {
                    field_0x244 = 9;
                }
                gGameRoot_8053AAF8->fn_80134D14(0);
            }
            lbl_8053AEC8->field_0x5c = 0xFF - field_0x25c;
        } else {
            field_0x250--;
        }
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
            fn_80230498(i, &pos, 0x3BE, 0xFF, 2, -0x80, true);
        }
        break;
    case 9:
        if (fn_80230534(false)) {
            mAlive = false;
            gAudioMgr_8053AF80->fn_8045873C(0);
        }
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
            if (getPlayerLayer_8023DE58(i) >= 8) {
                if ((field_0x250 & field_0x264[i]) != 0) {
                    fn_80230498(i, &pos, 0x3BE, 0xFF, 2, -0x80, true);
                } else {
                    fn_80230498(i, &pos, 0x3BF, 0xFF, 2, -0x80, true);
                }
            } else {
                fn_80230498(i, &pos, 0x3BE, 0xFF, 2, -0x80, true);
            }
        }
        break;
    case 10:
        if (field_0x250 == 0) {
            field_0x25c += field_0x254;
            if (field_0x25c >= field_0x258) {
                field_0x25c = field_0x258;
                if (fn_802DE7CC() && !isStageFlagSet(lbl_80529DEC.field_0x118, lbl_80529DEC.field_0x11c)) {
                    fn_8038BD08(0, 0x265, field_0x4, mPos, 0x21);
                    field_0x244 = 16;
                } else {
                    field_0x244 = 11;
                }
                gGameRoot_8053AAF8->fn_80134D14(0);
            }
            lbl_8053AEC8->field_0x5c = 0xFF - field_0x25c;
        } else {
            field_0x250--;
        }
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
            if (getPlayerLayer_8023DE58(i) >= 8) {
                if ((field_0x250 & field_0x264[i]) != 0) {
                    fn_80230498(i, &pos, 0x3BE, 0xFF, 2, -0x80, true);
                } else {
                    fn_80230498(i, &pos, 0x3BF, 0xFF, 2, -0x80, true);
                }
            } else {
                fn_80230498(i, &pos, 0x3BE, 0xFF, 2, -0x80, true);
            }
        }
        break;
    case 11:
        field_0x244 = 12;
        gGameRoot2_8053AB10->field_0x20->field_0x216 = 1;
        break;
    case 12:
        gGameRoot2_8053AB10->field_0x20->field_0x216 = 1;
        if (gGameRoot2_8053AB10->field_0x20->field_0x214 != 1) {
            fn_80119DD0()->fn_80119468(0, 1);
            field_0x250 = 5;
            field_0x244 = 13;
        }
        break;
    case 13: {
        gGameRoot2_8053AB10->field_0x20->field_0x216 = 1;
        gGameRoot_8053AAF8->fn_80134D14(0);
        s32 b = lbl_8046F628_DC[getGameView_802D6A9C()->field_0x4][getGameView_802D6A9C()->field_0x8];
        getGameView_802D6A9C()->fn_802D9FF4(
            lbl_8046F628_5C[getGameView_802D6A9C()->field_0x4][getGameView_802D6A9C()->field_0x8], b);
        gGameRoot_8053AAF8->fn_80136798(0xD, 0);
        getGameView_802D6A9C()->fn_802D6AAC(3);
        gAudioMgr_8053AF80->fn_8045873C(0);
        field_0x244 = 14;
        break;
    }
    case 14:
        gGameRoot2_8053AB10->field_0x20->field_0x216 = 1;
        break;
    case 15:
        if (fn_80388EEC(0)) {
            field_0x244 = 9;
        }
        break;
    case 16:
        if (fn_80388EEC(0)) {
            field_0x244 = 11;
        }
        break;
    }
    if (field_0x244 >= 1 && field_0x244 <= 10) {
        fn_80128C58((u8)field_0x25c);
        gGameRoot2_8053AB10->field_0x2c->field_0xd[1] = field_0x25c;
    }
}

// FIXME(shotgun): 100 %, matches
void PlayerSwordLevelCheckNPC_804AA590::draw() {}
