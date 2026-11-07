#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/Effect.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/ViewLayer.h"
#include "game/act/act_shadow_link_giant_bomb.h"

// A diagonal scale in the sprite's 3x3, every off-diagonal entry stored as 0.
static inline void setScaleMtx(Mtx mtx, f32 x, f32 yz) {
    mtx[0][0] = x;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = yz;
    mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = yz;
}

// Timer table of state 4 (indexed by field_0x238) and the sprite offsets of state 4.
static const u8 lbl_804AC4B8[12] = {3, 6, 6, 3, 3, 3, 6, 0x50, 3, 0, 0, 0};
static const u8 lbl_804AC4C4[12] = {0, 1, 2, 3, 4, 5, 6, 1, 0, 0, 0, 0};

ShadowLinkGiantBomb_804AC4D0::~ShadowLinkGiantBomb_804AC4D0() {
    if (gGameRoot_8053AAF8->field_0x7f == 1) {
        s32 mode = gGameRoot_8053AAF8->field_0x4c;
        if (mode != 9 && mode != 0xB) {
            gGameRoot_8053AAF8->fn_80138B7C();
        }
    }
}

// FIXME: 99.88%. Only the two float literals differ: their relocations are
// lbl_805422E0 and lbl_805422C4 in the original, unnamed (@NNN) in ours; the
// literal split of this unit, not the code.
void ShadowLinkGiantBomb_804AC4D0::init() {
    field_0x240 = (field_0x90 >> 12) & 0xFF;
    field_0x23c = 0;
    field_0x244 = -1;
    if (field_0x240 == 0) {
        field_0x254 = 1;
    } else {
        field_0x254 = 0;
        field_0x240--;
    }
    field_0x248 = (field_0x90 >> 22) & 0x1F;
    if (field_0x90 >> 27) {
        mState = 0;
        field_0x238 = 0;
        mStateTimer = 0;
    } else {
        mState = 1;
        field_0x238 = 0;
        mStateTimer = field_0x90 & 0x7F;
    }
    if (gGameRoot_8053AAF8->field_0x7f == 0) {
        s32 layer = field_0x4;
        if (viewLayerFlag(layer) == 1) {
            mPos.y += 4.0f;
            mSpawnPos = mPos;
        }
    }
    field_0x250 = 1.0f;
    GameView_804A9950* view = getGameView_802D6A9C();
    bool inView = view->fn_802D800C(field_0x248);
    if (!inView && gGameRoot_8053AAF8->field_0x7f == 1) {
        field_0x255 = 1;
    } else {
        field_0x255 = 0;
    }
}

// FIXME(shotgun): 98.27 %, scheduling+regalloc + reloc-name
void ShadowLinkGiantBomb_804AC4D0::update() {
    mPos += field_0x3c;
    if (field_0x255 == 1 && mState > 1) {
        cls_804A03A8* sub = gGameRoot2_8053AB10->field_0x30;
        if (sub->field_0x284 == 0) {
            sub->field_0x284 = 1;
            sub->field_0x28c = 0.05f;
        }
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
        f32 y = mPos.y - mPos.z;
        if (camera->field_0x7f5 == 0) {
            camera->field_0x7f4 = 1;
            camera->field_0x7f8 = 4;
            camera->field_0x7fc.x = mPos.x;
            camera->field_0x7fc.y = y;
            camera->field_0x7fc.z = 0.0f;
            camera->field_0x7f6 = 0;
        }
    }

    switch (mState) {
    case 0:
        if (vfunc_0x34()) {
            mState = 1;
            field_0x238 = 0;
            mStateTimer = field_0x90 & 0x7F;
        }
        break;
    case 1:
        if (field_0x255 == 1) {
            if (getActorManager_801F856C()->fn_801F68E0() == 0) {
                break;
            }
            s32 bit = field_0x248;
            if (bit > 0) {
                getGameView_802D6A9C()->fn_802D8050(bit, true, 0xFF);
            }
        }
        if (gGameRoot_8053AAF8->field_0x7f == 1) {
            gAudioMgr_8053AF80->fn_80458FF0(0x100000D, false, 0);
        }
        mState = 2;
        mStateTimer = 60;
        break;
    case 2:
        if (mStateTimer <= 0) {
            mState = 3;
            mStateTimer = 0x73;
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x418, 0, 1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x419, 0, 1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x41A, 0, 1, NULL, NULL, true, 1.0f);
            startSound_801F0E34(0x4004E, 0);
        }
        break;
    case 3:
        if (mStateTimer <= 0) {
            startSound_801F0E34(0x40035, 0);
            mState = 4;
            field_0x3c.z = 3.0f;
            mStateTimer = lbl_804AC4B8[0];
        }
        break;
    case 4:
        if (field_0x3c.z <= 0.0f) {
            mState = 5;
            field_0x3c.z = 0.0f;
            mStateTimer = 60;
        } else {
            field_0x3c.z -= 0.1f;
            if (mStateTimer <= 0 && field_0x238 < 9) {
                field_0x238++;
                mStateTimer = lbl_804AC4B8[field_0x238];
            }
        }
        break;
    case 5:
        if ((f32)mStateTimer <= 0.0f) {
            mState = 6;
            mStateTimer = 0x50;
            JGeometry::TVec3f pos = mPos;
            pos.z = mPos.z + 24.0f;
            field_0x244 = getActorManager_801F856C()->spawnActor_801F9484(
                ACTOR_BBMB, field_0x4, &pos, ((field_0x90 >> 8) & 0x1000) | 0x50, -1, -1);
            startSound_801F0E34(0x50085, 0);
        }
        break;
    case 6:
        if (mStateTimer <= 0) {
            startSound_801F0E34(0x40036, 0);
            mState = 7;
            mStateTimer = 60;
        }
        break;
    case 7:
        if (mStateTimer <= 0) {
            mState = 8;
            mStateTimer = 10;
        }
        if (field_0x255 == 1) {
            ActorBase_804A5770* bomb = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x244);
            if (bomb != NULL && bomb->mPos.z <= 0.0f) {
                getActorManager_801F856C()->fn_801F6884();
                field_0x255 = 0;
            }
        }
        break;
    case 8:
        if (mStateTimer <= 0) {
            mState = 9;
        } else {
            field_0x250 -= 0.1f;
        }
        break;
    case 9:
        if (getActorManager_801F856C()->findActorBySerial_801F666C(field_0x244) == NULL) {
            if (field_0x254 == 0 && field_0x240 <= 0) {
                if (gGameRoot_8053AAF8->field_0x7f == 1) {
                    gGameRoot_8053AAF8->fn_80138B7C();
                }
                mAlive = false;
                break;
            }
            if (field_0x240 > 0) {
                field_0x240--;
            }
            field_0x244 = -1;
            mStateTimer = 300;
            mState = 10;
            if (gGameRoot_8053AAF8->field_0x7f == 1) {
                if (vfunc_0x3C() != 0 && !vfunc_0x34()) {
                    gGameRoot_8053AAF8->fn_80138B7C();
                }
            }
        }
        break;
    case 10:
        if (mStateTimer <= 0) {
            if (vfunc_0x3C() == 0 || (vfunc_0x3C() != 0 && vfunc_0x34() == 1)) {
                if (gGameRoot_8053AAF8->field_0x7f == 1) {
                    if (gAudioMgr_8053AF80->field_0x58 != 0x100000D) {
                        gAudioMgr_8053AF80->fn_80458FF0(0x100000D, false, 0);
                    }
                }
                field_0x250 = 0.0f;
                mState = 11;
                mStateTimer = 60;
            }
        }
        break;
    case 11:
        if (field_0x250 >= 1.0f) {
            field_0x250 = 1.0f;
            mState = 6;
            mStateTimer = 0x50;
            JGeometry::TVec3f pos = mPos;
            pos.z = mPos.z + 24.0f;
            field_0x244 = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_BBMB, field_0x4, &pos, 0x2050, -1, -1);
            startSound_801F0E34(0x50085, 0);
        } else {
            field_0x250 += 0.016666668f;
        }
        break;
    }

    if (mState != 0 && mState != 1 && mState != 9 && mState != 10) {
        JGeometry::TVec3f focus(mPos.x, mPos.y - mPos.z, 0.0f);
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&focus);
    }

    if (mState == 8) {
        field_0x23c = 0x330;
    }
    if (mState == 11) {
        field_0x23c = (((mStateTimer >> 2) + 2) % 4) * 5 + 0x326;
    } else if (mState == 7) {
        if (mStateTimer < 0x2B) {
            field_0x23c = 0x330;
        } else if (mStateTimer < 0x33) {
            field_0x23c = 0x32F;
        } else if (mStateTimer < 0x34) {
            field_0x23c = 0x32E;
        } else {
            field_0x23c = 0x32D;
        }
    } else if (mState == 6) {
        field_0x23c = 0x102;
    } else if (mState == 5) {
        field_0x23c = 0x330;
    } else if (mState == 4) {
        field_0x23c = lbl_804AC4C4[field_0x238] + 0x101;
    }

    if (mStateTimer > 0) {
        mStateTimer--;
    }
}

// FIXME: 99.74%. Only the float literals (1.0f, 2.0f, 3.0f, 0.0f) and the
// "player_sob.bin" string differ: their relocations are lbl_ names in the
// original and @NNN in ours; the literal split, not the code.
void ShadowLinkGiantBomb_804AC4D0::draw() {
    if (mState == 8 || mState == 0xb) {
        SpriteParams_801F5AE0 params;
        Mtx mtx;
        PSMTXIdentity(mtx);
        params.field_0x2e = 0x40;
        if (mState == 8) {
            setScaleMtx(mtx, field_0x250, 3.0f - 2.0f * field_0x250);
        } else {
            setScaleMtx(mtx, field_0x250, 2.0f - field_0x250);
        }
        params.mMtx = mtx;
        drawSprite_801F06F0("player_sob.bin", field_0x23c, &params, 7, 1.0f, 1.0f);
    } else {
        if (mState >= 4 && mState <= 7) {
            drawSprite_801F06F0("player_sob.bin", field_0x23c, NULL, 7, 1.0f, 1.0f);
        }
    }
}
