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

// FIXME(shotgun): table not written out; the (x, y) offsets of the tiles state 5 rewrites:
// 9 pairs, then 12, then 16. Extern-declared, size from symbols.txt.
extern const f32 lbl_8046ACC8[37][2];

// 0x8022A83C.
// FIXME(shotgun): 96.87 %, different
void PlayerEffect_804A6E20::init() {
    u32 kind = (field_0x90 >> 2) & 0x3F;
    if (kind != 5) {
        if (kind == 1) {
            if (fn_8022F514(field_0x90 & 3)) {
                mAlive = false;
                return;
            }
        } else if (kind != 6 && kind != 7 && kind != 8 && kind != 9 && field_0x4 < 8) {
            mAlive = false;
            return;
        }
    }
    kind = (field_0x90 >> 2) & 0x3F;
    if (kind == 3) {
        mPos.x += (f32)(s32)(4.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) - 2.0f;
        mPos.y += (f32)(s32)(4.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) - 5.0f;
    } else if (kind == 4) {
        mPos.z += 16.0f;
    } else if (kind == 0xE) {
        mPos.x -= 4.0f;
        mPos.z += 16.0f;
    }
    mLife = 0;
    field_0x230 = 0;
    field_0x234 = 0;
    kind = (field_0x90 >> 2) & 0x3F;
    switch (kind) {
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
        mPos = *getPlayerPos_8023E724(field_0x90 & 3);
        mPos.x += 10.0f;
        mPos.z += 20.0f;
        field_0x234 = 0x2A3;
        break;
    case 7:
    case 9:
        mPos = *getPlayerPos_8023E724(field_0x90 & 3);
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
    field_0x11f = 1;
}

// 0x80229F1C.
// FIXME(shotgun): 97.27 %, missing/extra + reloc-name
void PlayerEffect_804A6E20::update() {
    field_0x230++;
    switch ((field_0x90 >> 2) & 0x3F) {
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
    case 1:
        mPos = *getPlayerPos_8023E724(field_0x90 & 3);
        field_0x234 = ((field_0x230 >> 1) & 7) + 0x7D;
        if (fn_80237774(field_0x90 & 3) == lbl_80529DEC.field_0xd8[field_0x90 & 3]) {
            mAlive = false;
        }
        break;
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
        if (mPos.y < mSpawnPos.y - 50.0f || fn_8022461C(&mPos, field_0x4) != 0x3B) {
            mAlive = false;
        }
        break;
    case 4:
    case 14:
        mPos = *getPlayerPos_8023E724(field_0x90 & 3);
        mPos.z += 16.0f;
        if (field_0x230 >= 12) {
            field_0x230 = 0;
        }
        // FIXME(shotgun): the original masks the quotient to 16 bits (extrwi 16,15).
        field_0x234 = field_0x230 / 3 + 0x1C3;
        if (((field_0x90 >> 2) & 0x3F) == 14) {
            mPos.x -= 4.0f;
            if (fn_80239D38() || !fn_80236B50(field_0x90 & 3) || fn_80236AB4(field_0x90 & 3)) {
                mAlive = false;
            }
        } else if (!fn_80235274(field_0x90 & 3)) {
            mAlive = false;
        }
        break;
    case 5:
        if (field_0x230 == 1) {
            for (s32 i = 0; i < 9; i++) {
                JGeometry::TVec3f pos(mPos);
                pos.x += lbl_8046ACC8[0 + i][0];
                pos.y += lbl_8046ACC8[0 + i][1];
                gBgRewrite_8053AC98->fn_802A20F0(&pos, field_0x90 & 3, field_0x4, 0, 1);
            }
        } else if (field_0x230 == 11) {
            for (s32 i = 0; i < 12; i++) {
                JGeometry::TVec3f pos(mPos);
                pos.x += lbl_8046ACC8[9 + i][0];
                pos.y += lbl_8046ACC8[9 + i][1];
                gBgRewrite_8053AC98->fn_802A20F0(&pos, field_0x90 & 3, field_0x4, 0, 1);
            }
        } else if (field_0x230 == 21) {
            for (s32 i = 0; i < 16; i++) {
                JGeometry::TVec3f pos(mPos);
                pos.x += lbl_8046ACC8[21 + i][0];
                pos.y += lbl_8046ACC8[21 + i][1];
                gBgRewrite_8053AC98->fn_802A20F0(&pos, field_0x90 & 3, field_0x4, 0, 1);
            }
            mAlive = false;
        }
        break;
    case 6:
    case 8:
        mPos = *getPlayerPos_8023E724(field_0x90 & 3);
        mPos.x += 10.0f;
        mPos.z += 20.0f;
        if (fn_8022FA80(field_0x90 & 3) == 1 || ((field_0x90 >> 2) & 0x3F) == 8) {
            if (field_0x230 >= 2 && field_0x234 == 0x2A3) {
                field_0x230 = 2;
                s32 slot = fn_8022F340(field_0x90 & 3);
                if (((gGameRoot_8053AAF8->field_0x83 >> (slot * 2)) & 3) == 2) {
                    field_0x234 = 0x2A5;
                } else if (fn_8037E694(field_0x90 & 3) == 1) {
                    if (field_0x4 < 8) {
                        field_0x234 = 0x2A5;
                    } else {
                        field_0x234 = 0x2A8;
                    }
                } else if (fn_8037E694(field_0x90 & 3) == 2) {
                    if (field_0x4 < 8) {
                        field_0x234 = 0x2A4;
                    } else {
                        field_0x234 = 0x2A5;
                    }
                } else {
                    if (field_0x4 < 8) {
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
        mPos = *getPlayerPos_8023E724(field_0x90 & 3);
        mPos.x -= 10.0f;
        mPos.z += 20.0f;
        if (fn_8022FA80(field_0x90 & 3) == 1 || ((field_0x90 >> 2) & 0x3F) == 9) {
            if (field_0x230 > 2 && field_0x234 == 0x2A3) {
                field_0x230 = 2;
                s32 slot = fn_8022F340(field_0x90 & 3);
                if (((gGameRoot_8053AAF8->field_0x83 >> (slot * 2)) & 3) == 2) {
                    field_0x234 = 0x2A6;
                } else if (fn_8037E694(field_0x90 & 3) == 1) {
                    if (field_0x4 < 8) {
                        field_0x234 = 0x2A6;
                    } else {
                        field_0x234 = 0x2A9;
                    }
                } else if (fn_8037E694(field_0x90 & 3) == 2) {
                    if (field_0x4 < 8) {
                        field_0x234 = 0x2A7;
                    } else {
                        field_0x234 = 0x2A6;
                    }
                } else {
                    if (field_0x4 < 8) {
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
        mPos = *getPlayerPos_8023E724(field_0x90 & 3);
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

// 0x802297FC.
// FIXME(shotgun): 86.05 %, missing/extra + reloc-name
void PlayerEffect_804A6E20::draw() {
    SpriteParams_801F5AE0 params;
    switch ((field_0x90 >> 2) & 0x3F) {
    case 0:
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, field_0x234, NULL, 1.0f, 1.0f);
        }
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x234, 0, 1, -1, true, 0, 0);
        break;
    case 1:
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, field_0x234, NULL, 1.0f, 1.0f);
        }
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x234, 0, 1, -1, false, 0, 0);
        break;
    case 2:
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, field_0x234, NULL, 1.0f, 1.0f);
        }
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x234, 0, 1, -1, true, 0, 0);
        break;
    case 3:
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, field_0x234, NULL, 1.0f, 1.0f);
        }
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x234, 0, 1, -1, true, 0, 0);
        break;
    case 4:
    case 14:
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, field_0x234, NULL, 1.0f, 1.0f);
        }
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x234, 0, 1, -1, true, 0, 0);
        break;
    case 6:
    case 7:
    case 8:
    case 9:
        params.field_0x1c = 2;
        params.field_0x2d = 1;
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, field_0x234, &params, 1.0f, 1.0f);
        }
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x234, 0, 1, -1, true, 0, 0);
        break;
    case 10:
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, field_0x234, NULL, 1.0f, 1.0f);
        }
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x234, 0, 1, -1, true, 0, 0);
        break;
    case 11:
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, field_0x234, NULL, 1.0f, 1.0f);
        }
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x234, 0, 1, -1, true, 0, 0);
        break;
    case 12:
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, field_0x234, NULL, 1.0f, 1.0f);
        }
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x234, 0, 1, -1, true, 0, 0);
        break;
    case 13:
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, field_0x234, NULL, 1.0f, 1.0f);
        }
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x234, 0, 1, -1, true, 0, 0);
        break;
    }
}

// Destructor at 0x8022979C.
PlayerEffect_804A6E20::~PlayerEffect_804A6E20() {}
