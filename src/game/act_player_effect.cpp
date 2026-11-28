#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/act/act_player_effect.h"
#include "game/Map.h"
#include "game/SpriteParams.h"
#include "game/BgRewrite.h"
#include "game/GameState.h"
#include "game/Message.h"
#include "game/Map.h"

// The (x, y) offsets of the tiles state 5 rewrites: 9 pairs, 12 and 16.
static const f32 lbl_8046ACC8[9][2] = {
    {0.0f, 0.0f}, {-16.0f, -16.0f}, {0.0f, -16.0f}, {16.0f, -16.0f},
    {-16.0f, 0.0f}, {16.0f, 0.0f}, {-16.0f, 16.0f}, {0.0f, 16.0f},
    {16.0f, 16.0f},
};

static const f32 lbl_8046AD10[12][2] = {
    {-16.0f, -32.0f}, {0.0f, -32.0f}, {16.0f, -32.0f}, {-32.0f, -16.0f},
    {-32.0f, 0.0f}, {-32.0f, 16.0f}, {32.0f, -16.0f}, {32.0f, 0.0f},
    {32.0f, 16.0f}, {-16.0f, 32.0f}, {0.0f, 32.0f}, {16.0f, 32.0f},
};

static const f32 lbl_8046AD70[16][2] = {
    {-16.0f, -48.0f}, {0.0f, -48.0f}, {16.0f, -48.0f}, {-48.0f, -16.0f},
    {-48.0f, 0.0f}, {-48.0f, 16.0f}, {48.0f, -16.0f}, {48.0f, 0.0f},
    {48.0f, 16.0f}, {-16.0f, 48.0f}, {0.0f, 48.0f}, {16.0f, 48.0f},
    {32.0f, 32.0f}, {32.0f, -32.0f}, {-32.0f, 32.0f}, {-32.0f, -32.0f},
};

// 0x8022A83C.
// r4/r5 swapped in inlined mRandom.get_ufloat_1().
// FIXME: 99.77 %, regalloc-only; behaviour drift.
void PlayerEffect_804A6E20::init() {
    if (getKind() != 5) {
        if (getKind() == 1) {
            if (hasPlayerLeader_8022F514(mSpawnParam_0x90 & 3)) {
                mAlive = false;
                return;
            }
        } else if (getKind() != 6 && getKind() != 7 && getKind() != 8 && getKind() != 9 && mLayer_0x4 < 8) {
            mAlive = false;
            return;
        }
    }
    if (getKind() == 3) {
        mPos.x += (f32)(s32)(4.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) - 2.0f;
        mPos.y += (f32)(s32)(4.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) - 5.0f;
    } else if (getKind() == 4) {
        mPos.z += 16.0f;
    } else if (getKind() == 0xE) {
        mPos.x -= 4.0f;
        mPos.z += 16.0f;
    }
    mLife = 0;
    field_0x230 = 0;
    field_0x234 = 0;
    switch (getKind()) {
    case 0:
        field_0x234 = 0x85;
        break;
    case 1:
        field_0x234 = 0x7D;
        break;
    case 2:
        field_0x234 = 0x2AB;
        break;
    case 3:
        field_0x234 = 0x1AA;
        break;
    case 4:
    case 14:
        field_0x234 = 0x1C3;
        break;
    case 6:
    case 8:
        mPos = *getPlayerPos_8023E724(mSpawnParam_0x90 & 3);
        mPos.x += 10.0f;
        mPos.z += 20.0f;
        field_0x234 = 0x2A3;
        break;
    case 7:
    case 9:
        mPos = *getPlayerPos_8023E724(mSpawnParam_0x90 & 3);
        mPos.x -= 10.0f;
        mPos.z += 20.0f;
        field_0x234 = 0x2A3;
        break;
    case 10:
        field_0x234 = 0x2F5;
        break;
    case 11:
        field_0x234 = 0x306;
        break;
    case 12:
        field_0x234 = 0x30C;
        break;
    case 13:
        field_0x234 = 0x300;
        break;
    }
    mUpdateWhilePaused_0x11f = 1;
}

// 0x80229F1C.
void PlayerEffect_804A6E20::update() {
    field_0x230++;
    switch (getKind()) {
    case 0:
        if (field_0x230 <= 2) {
            field_0x234 = 0x85;
        } else if (field_0x230 <= 4) {
            field_0x234 = 0x86;
        } else if (field_0x230 <= 6) {
            field_0x234 = 0x87;
        } else if (field_0x230 <= 8) {
            field_0x234 = 0x88;
        } else {
            mAlive = false;
        }
        break;
    case 1: {
        mPos = *getPlayerPos_8023E724(mSpawnParam_0x90 & 3);
        field_0x234 = ((field_0x230 >> 1) & 7) + 0x7D;
        s32 player = mSpawnParam_0x90 & 3;
        s32 life = getPlayerLife_80237774(player);
        if (life == lbl_80529DEC.field_0xd8[player]) {
            mAlive = false;
        }
        break;
    }
    case 2:
        if (field_0x230 <= 2) {
            field_0x234 = 0x2AB;
        } else if (field_0x230 <= 4) {
            field_0x234 = 0x2AC;
        } else if (field_0x230 <= 6) {
            field_0x234 = 0x2AD;
        } else if (field_0x230 <= 8) {
            field_0x234 = 0x2AE;
        } else if (field_0x230 <= 10) {
            field_0x234 = 0x2AB;
        } else {
            mAlive = false;
        }
        break;
    case 3:
        mPos.y -= 0.5f;
        field_0x234 = 0x1AA;
        if ((s32)(2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) == 0) {
            mPos.x -= 0.5f;
        } else {
            mPos.x += 0.5f;
        }
        if (mPos.y < mSpawnPos.y - 50.0f || getTileCollisionCode_8022461C(&mPos, mLayer_0x4) != 0x3B) {
            mAlive = false;
        }
        break;
    case 4:
    case 14:
        mPos = *getPlayerPos_8023E724(mSpawnParam_0x90 & 3);
        mPos.z += 16.0f;
        if (field_0x230 >= 12) {
            field_0x230 = 0;
        }
        field_0x234 = (u16)(field_0x230 / 3) + 0x1C3;
        if (getKind() == 14) {
            mPos.x -= 4.0f;
            if (fn_80239D38() || !isPlayerDead_80236B50(mSpawnParam_0x90 & 3) || fn_80236AB4(mSpawnParam_0x90 & 3)) {
                mAlive = false;
            }
        } else if (!fn_80235274(mSpawnParam_0x90 & 3)) {
            mAlive = false;
        }
        break;
    case 5:
        if (field_0x230 == 1) {
            for (s32 i = 0; i < 9; i++) {
                JGeometry::TVec3f pos(mPos);
                pos.x += lbl_8046ACC8[i][0];
                pos.y += lbl_8046ACC8[i][1];
                gBgRewrite_8053AC98->fn_802A20F0(&pos, mSpawnParam_0x90 & 3, mLayer_0x4, 0, 1);
            }
        } else if (field_0x230 == 11) {
            for (s32 i = 0; i < 12; i++) {
                JGeometry::TVec3f pos(mPos);
                pos.x += lbl_8046AD10[i][0];
                pos.y += lbl_8046AD10[i][1];
                gBgRewrite_8053AC98->fn_802A20F0(&pos, mSpawnParam_0x90 & 3, mLayer_0x4, 0, 1);
            }
        } else if (field_0x230 == 21) {
            for (s32 i = 0; i < 16; i++) {
                JGeometry::TVec3f pos(mPos);
                pos.x += lbl_8046AD70[i][0];
                pos.y += lbl_8046AD70[i][1];
                gBgRewrite_8053AC98->fn_802A20F0(&pos, mSpawnParam_0x90 & 3, mLayer_0x4, 0, 1);
            }
            mAlive = false;
        }
        break;
    case 6:
    case 8:
        mPos = *getPlayerPos_8023E724(mSpawnParam_0x90 & 3);
        mPos.x += 10.0f;
        mPos.z += 20.0f;
        if (fn_8022FA80(mSpawnParam_0x90 & 3) == 1 || getKind() == 8) {
            if (field_0x230 >= 2 && field_0x234 == 0x2A3) {
                field_0x230 = 2;
                s32 slot = fn_8022F340(mSpawnParam_0x90 & 3);
                u32 mode = (gGameRoot_8053AAF8->field_0x83 >> (slot * 2)) & 3;
                if (mode == 2) {
                    field_0x234 = 0x2A5;
                } else if (fn_8037E694(mSpawnParam_0x90 & 3) == 1) {
                    if (mLayer_0x4 < 8) {
                        field_0x234 = 0x2A5;
                    } else {
                        field_0x234 = 0x2A8;
                    }
                } else if (fn_8037E694(mSpawnParam_0x90 & 3) == 2) {
                    if (mLayer_0x4 < 8) {
                        field_0x234 = 0x2A4;
                    } else {
                        field_0x234 = 0x2A5;
                    }
                } else {
                    if (mLayer_0x4 < 8) {
                        field_0x234 = 0x2A5;
                    } else {
                        field_0x234 = 0x2A8;
                    }
                }
            }
        } else {
            mAlive = false;
        }
        break;
    case 7:
    case 9:
        mPos = *getPlayerPos_8023E724(mSpawnParam_0x90 & 3);
        mPos.x -= 10.0f;
        mPos.z += 20.0f;
        if (fn_8022FA80(mSpawnParam_0x90 & 3) == 1 || getKind() == 9) {
            if (field_0x230 > 2 && field_0x234 == 0x2A3) {
                field_0x230 = 2;
                s32 slot = fn_8022F340(mSpawnParam_0x90 & 3);
                u32 mode = (gGameRoot_8053AAF8->field_0x83 >> (slot * 2)) & 3;
                if (mode == 2) {
                    field_0x234 = 0x2A6;
                } else if (fn_8037E694(mSpawnParam_0x90 & 3) == 1) {
                    if (mLayer_0x4 < 8) {
                        field_0x234 = 0x2A6;
                    } else {
                        field_0x234 = 0x2A9;
                    }
                } else if (fn_8037E694(mSpawnParam_0x90 & 3) == 2) {
                    if (mLayer_0x4 < 8) {
                        field_0x234 = 0x2A7;
                    } else {
                        field_0x234 = 0x2A6;
                    }
                } else {
                    if (mLayer_0x4 < 8) {
                        field_0x234 = 0x2A6;
                    } else {
                        field_0x234 = 0x2A9;
                    }
                }
            }
        } else {
            mAlive = false;
        }
        break;
    case 10:
        mPos = *getPlayerPos_8023E724(mSpawnParam_0x90 & 3);
        if (field_0x230 <= 2) {
            field_0x234 = 0x2F5;
        } else {
            field_0x234 = 0x2F6;
        }
        if (field_0x230 > 4) {
            mAlive = false;
        }
        break;
    case 11:
        if (field_0x230 <= 2) {
            field_0x234 = 0x306;
        } else if (field_0x230 <= 4) {
            field_0x234 = 0x307;
        } else if (field_0x230 <= 6) {
            field_0x234 = 0x308;
        } else if (field_0x230 <= 8) {
            field_0x234 = 0x309;
        } else if (field_0x230 <= 10) {
            field_0x234 = 0x30A;
        } else if (field_0x230 <= 12) {
            field_0x234 = 0x30B;
        } else {
            mAlive = false;
        }
        break;
    case 12:
        if (field_0x230 <= 2) {
            field_0x234 = 0x30C;
        } else if (field_0x230 <= 4) {
            field_0x234 = 0x30D;
        } else if (field_0x230 <= 6) {
            field_0x234 = 0x30E;
        } else {
            mAlive = false;
        }
        break;
    case 13:
        if (field_0x230 <= 2) {
            field_0x234 = 0x300;
        } else if (field_0x230 <= 4) {
            field_0x234 = 0x301;
        } else if (field_0x230 <= 6) {
            field_0x234 = 0x302;
        } else if (field_0x230 <= 8) {
            field_0x234 = 0x303;
        } else if (field_0x230 <= 10) {
            field_0x234 = 0x304;
        } else if (field_0x230 <= 12) {
            field_0x234 = 0x305;
        } else {
            mAlive = false;
        }
        break;
    }
}

static inline void drawShadow_802297FC(s32 layer, const JGeometry::TVec3f* pos, s32 frame, SpriteParams_801F5AE0* params) {
    if (layer < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
        gGameRoot2_8053AB10->field_0x20->drawSobSprite_802F86CC(pos, res, frame, params, 1.0f, 1.0f);
    }
}

// 0x802297FC.
void PlayerEffect_804A6E20::draw() {
    SpriteParams_801F5AE0 params;
    switch ((mSpawnParam_0x90 >> 2) & 0x3F) {
    case 0:
        drawShadow_802297FC(mLayer_0x4, getPos(), field_0x234, NULL);
        showGbaSprite_803075AC(mSerial, mLayer_0x4, &mPos, field_0x234, 0, 1, -1, true, 0, 0);
        break;
    case 1:
        drawShadow_802297FC(mLayer_0x4, getPos(), field_0x234, NULL);
        showGbaSprite_803075AC(mSerial, mLayer_0x4, &mPos, field_0x234, 0, 1, -1, false, 0, 0);
        break;
    case 2:
        drawShadow_802297FC(mLayer_0x4, getPos(), field_0x234, NULL);
        showGbaSprite_803075AC(mSerial, mLayer_0x4, &mPos, field_0x234, 0, 1, -1, true, 0, 0);
        break;
    case 3:
        drawShadow_802297FC(mLayer_0x4, getPos(), field_0x234, NULL);
        showGbaSprite_803075AC(mSerial, mLayer_0x4, &mPos, field_0x234, 0, 1, -1, true, 0, 0);
        break;
    case 4:
    case 14:
        drawShadow_802297FC(mLayer_0x4, getPos(), field_0x234, NULL);
        showGbaSprite_803075AC(mSerial, mLayer_0x4, &mPos, field_0x234, 0, 1, -1, true, 0, 0);
        break;
    case 6:
    case 7:
    case 8:
    case 9:
        params.field_0x1c = 2;
        params.field_0x2d = 1;
        drawShadow_802297FC(mLayer_0x4, getPos(), field_0x234, &params);
        showGbaSprite_803075AC(mSerial, mLayer_0x4, &mPos, field_0x234, 0, 1, -1, true, 0, 0);
        break;
    case 10:
        drawShadow_802297FC(mLayer_0x4, getPos(), field_0x234, NULL);
        showGbaSprite_803075AC(mSerial, mLayer_0x4, &mPos, field_0x234, 0, 1, -1, true, 0, 0);
        break;
    case 11:
        drawShadow_802297FC(mLayer_0x4, getPos(), field_0x234, NULL);
        showGbaSprite_803075AC(mSerial, mLayer_0x4, &mPos, field_0x234, 0, 1, -1, true, 0, 0);
        break;
    case 12:
        drawShadow_802297FC(mLayer_0x4, getPos(), field_0x234, NULL);
        showGbaSprite_803075AC(mSerial, mLayer_0x4, &mPos, field_0x234, 0, 1, -1, true, 0, 0);
        break;
    case 13:
        drawShadow_802297FC(mLayer_0x4, getPos(), field_0x234, NULL);
        showGbaSprite_803075AC(mSerial, mLayer_0x4, &mPos, field_0x234, 0, 1, -1, true, 0, 0);
        break;
    }
}

// Destructor at 0x8022979C.
PlayerEffect_804A6E20::~PlayerEffect_804A6E20() {}
