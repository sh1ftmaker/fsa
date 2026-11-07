#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/Camera.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/ViewLayer.h"
#include "game/act/act_spreading_fire.h"

// Offsets of the fire's spread steps, one table per axis (.rodata 0x8046D3B0 / 0x8046D3F0).
static const f32 lbl_8046D3B0[16] = {
    -16.0f, 0.0f, 16.0f, -16.0f, 16.0f, -16.0f, 0.0f, 16.0f,
    -16.0f, 0.0f, 16.0f, -16.0f, 16.0f, -16.0f, 0.0f, 16.0f,
};
static const f32 lbl_8046D3F0[16] = {
    -16.0f, -16.0f, -16.0f, 0.0f, 0.0f, 16.0f, 16.0f, 16.0f,
    -16.0f, -16.0f, -16.0f, 0.0f, 0.0f, 16.0f, 16.0f, 16.0f,
};

// A tile coordinate of the view box (0..0x1FF).
static inline s32 clampViewCoord(s32 v) {
    if (v < 0) {
        return 0;
    }
    if (v >= 0x200) {
        return 0x1FF;
    }
    return v;
}

// The GameView bit that marks a 16-cell block: x and y coordinates packed.
static inline s32 viewBitOf(s32 x, s32 y) {
    return (x & 0x100) | ((y & 0x100) << 1) | (y & 0xF0) | ((x >> 4) & 0xF);
}

// FIXME: 95.43 %. The original tests field_0x250 twice on the same flags (a bne to the
// sprite-skip, then a second bne that is never taken before the view-bit test); no natural
// form gives that dead branch, so the kill check is written as one condition.
void SpreadingFire_804A8A50::init() {
    field_0x250 = (field_0x90 >> 6) & 0xF;
    field_0x98 = (field_0x90 >> 4) & 0x3;
    field_0x258 = field_0x90 & 0xF;
    if (field_0x258 == 0xF) {
        field_0x258 = -1;
    }
    field_0x290 = 0;
    field_0x294 = 0;
    field_0x27c = 0;
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
    mEmitters[2] = NULL;
    mEmitters[3] = NULL;

    s32 layer = field_0x4;
    GameView_804A9950* view = getGameView_802D6A9C();
    const JGeometry::TBox2f* box;
    if (layer < 8) {
        box = &view->mVisibleBox;
    } else {
        box = &view->field_0xdc;
    }
    field_0x25c = *box;
    field_0x270 = (s32)(mPos.x - field_0x25c.i.x);
    field_0x274 = (s32)(mPos.y - field_0x25c.i.y);
    if (field_0x270 < 0) {
        field_0x270 = 0;
    } else if (field_0x270 >= 0x200) {
        field_0x270 = 0x1FF;
    }
    if (field_0x274 < 0) {
        field_0x274 = 0;
    } else if (field_0x274 >= 0x200) {
        field_0x274 = 0x1FF;
    }
    mViewBit = (field_0x270 & 0x100) | ((field_0x274 & 0x100) << 1) | (field_0x274 & 0xF0) |
               ((field_0x270 >> 4) & 0xF);

    if (field_0x250 != 0 || !getGameView_802D6A9C()->fn_802D7384(mViewBit)) {
        mBox.i.x = -6.0f;
        mBox.i.y = -6.0f;
        mBox.f.x = 6.0f;
        mBox.f.y = 6.0f;
        field_0x80.i.x = -8.0f;
        field_0x80.i.y = -8.0f;
        field_0x80.f.x = 8.0f;
        field_0x80.f.y = 8.0f;

        mTile = fn_80226850(&mPos, field_0x4);
        if (field_0x4 >= 8) {
            field_0x240 = 3;
            field_0x280 = 0;
        } else {
            field_0x240 = 0;
            if (field_0x250 == 3) {
                mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x19F, 0, 0, NULL, NULL, true, 1.0f);
                mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x1A0, 0, 0, NULL, NULL, true, 1.0f);
                mEmitters[2] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x1A1, 0, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x54D, 1, 0, NULL, NULL, true, 1.5f);
            } else if (field_0x250 != 6) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x349, 0, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x34A, 0, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x34B, 0, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x54D, 1, 0, NULL, NULL, true, 1.0f);
                field_0x290 = 1;
                field_0x294 = 10;
            }
        }

        s32 nibble = (field_0x90 >> 12) & 0xF;
        if (nibble == 0) {
            field_0x254 = 30;
        } else {
            field_0x254 = nibble * 2;
            if (field_0x254 < 0x15) {
                field_0x27c = (s32)(1000.88f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) & 7;
            }
        }

        field_0x278 = 0;
        if (viewLayerFlag(field_0x4) == 0) {
            if (fn_801EE3C0(mTile) == true) {
                getGameView_802D6A9C()->fn_802D73CC(mViewBit, true);
            }
        }
    } else {
        mAlive = false;
    }
}

// Destructor at 0x802BD1A8.
SpreadingFire_804A8A50::~SpreadingFire_804A8A50() {
    if (mEmitters[0] != NULL) {
        releaseEmitter_801EE434(mEmitters[0]);
    }
    if (mEmitters[1] != NULL) {
        releaseEmitter_801EE434(mEmitters[1]);
    }
    if (mEmitters[2] != NULL) {
        releaseEmitter_801EE434(mEmitters[2]);
    }
    if (mEmitters[3] != NULL) {
        releaseEmitter_801EE434(mEmitters[3]);
    }
    getGameView_802D6A9C()->fn_802D73CC(mViewBit, false);
}

// FIXME: 90 %. The original copies mPos to a stack TVec3f (psq_st 0x24(r1)) and never reads
// it, and it keeps one more register than we do (r29). Left out: the copy has no source use.
void SpreadingFire_804A8A50::update() {
    s32 damage = field_0x114;
    if (gGameRoot_8053AAF8->field_0x7f == 1) {
        damage = 0;
    }
    if (field_0x250 != 4) {
        if (fn_801F2B7C(damage, -1, 7) == 0) {
            fn_801F2B20(5);
        }
    } else {
        fn_801F2B20(5);
    }

    switch (field_0x240) {
    case 0: {
        field_0x240 = 1;
        mTile = fn_80226850(&mPos, field_0x4);
        if (fn_801EE3C0(mTile) == true) {
            startSound_801F0E34(0x3002F, 0);
            gBgRewrite_8053AC98->fn_802A20F0(&mPos, field_0x258, field_0x4, 0x10000, 1);
        } else if (mTile == 0x3EB) {
            gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0xF);
            startSound_801F0E34(0x30061, 0);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x254, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x255, 0, 0, NULL, NULL, true, 1.0f);
            JGeometry::TVec3f tmp(mPos.x, mPos.y - 16.0f, 0.0f);
            if (fn_80226850(&tmp, field_0x4) == 0x3EB) {
                field_0x284 = mPos;
                mPos.y -= 16.0f;
                field_0x240 = 5;
            } else {
                gBgRewrite_8053AC98->fn_802A3948(&mPos, 0, field_0x4, 0x3E0);
            }
        } else if (mTile == 0x33) {
            startSound_801F0E34(0x30061, 0);
            gBgRewrite_8053AC98->fn_802A3948(&mPos, 0, field_0x4, 0xAD);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x254, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x255, 0, 0, NULL, NULL, true, 1.0f);
        } else if (mTile == 0x34) {
            startSound_801F0E34(0x30061, 0);
            gBgRewrite_8053AC98->fn_802A3948(&mPos, 0, field_0x4, 0x251);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x254, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x255, 0, 0, NULL, NULL, true, 1.0f);
        }
        field_0x27c = (s32)(1000.88f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) & 7;
        field_0x298 = 8;
        break;
    }
    case 1: {
        if (field_0x298 != 0) {
            field_0x298--;
        }
        startSound_801F0E34(0x3002F, 0);
        field_0x254--;
        if (field_0x254 <= 0x14 && (field_0x254 & 1) == 0) {
            if (field_0x278 < 8) {
                s32 step = field_0x27c;
                JGeometry::TVec3f tmp(mPos.x + lbl_8046D3B0[step], mPos.y + lbl_8046D3F0[step],
                                      mPos.z);
                field_0x27c = step + 1;
                field_0x278++;
                u16 tile = fn_80226850(&tmp, field_0x4);
                if (fn_801EE3C0(tile) == true) {
                    if (field_0x250 != 5 && field_0x250 != 6) {
                        field_0x270 = clampViewCoord((s32)(tmp.x - field_0x25c.i.x));
                        field_0x274 = clampViewCoord((s32)(tmp.y - field_0x25c.i.y));
                        GameView_804A9950* view = getGameView_802D6A9C();
                        if (!view->fn_802D7384(viewBitOf(field_0x270, field_0x274))) {
                            s32 owner = field_0x258;
                            if (owner == -1) {
                                getActorManager_801F856C()->spawnActor_801F9484(ACTOR_BLZE, field_0x4, &tmp,
                                                           (field_0x98 << 4) | 0xF, -1, -1);
                            } else {
                                getActorManager_801F856C()->spawnActor_801F9484(ACTOR_BLZE, field_0x4, &tmp,
                                                           owner | (field_0x98 << 4), -1, field_0x110);
                            }
                        }
                    }
                }
                if (field_0x278 == 8) {
                    field_0x240 = 2;
                }
            }
        }
        break;
    }
    case 2:
        field_0x254--;
        if (field_0x254 == 0) {
            vfunc_0x24();
        }
        break;
    case 3:
        field_0x280++;
        if (field_0x280 >= 0x20) {
            if (fn_801EE3C0(mTile) == true) {
                gBgRewrite_8053AC98->fn_802A20F0(&mPos, field_0x258, field_0x4, 0x10000, 1);
            }
            vfunc_0x24();
        }
        break;
    case 4:
        break;
    case 5:
        field_0x254--;
        if (field_0x254 == 0) {
            gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x3E0);
            field_0x240 = 6;
        }
        break;
    case 6:
        for (;;) {
            do {
                mPos.y += 8.0f;
            } while (mPos.y != field_0x284.y);
            gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x3EB);
            s32 layer = field_0x4;
            if (layer >= 8) {
                getActorManager_801F856C()->spawnActor_801F9484(ACTOR_KMRI, layer, &mPos, 0, -1, -1);
            }
            field_0x284.y -= 16.0f;
            mPos.y -= 32.0f;
            if (fn_80226850(&mPos, field_0x4) != 0x3EB) {
                break;
            }
            gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x3E0);
        }
        mPos.y += 16.0f;
        gBgRewrite_8053AC98->fn_802A3948(&mPos, 0, field_0x4, 0x3E0);
        vfunc_0x24();
        break;
    case 7:
        field_0x254--;
        if (field_0x254 == 0) {
            vfunc_0x24();
        }
        break;
    }

    if (field_0x250 != 3) {
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
    }
}

// 0x802BC930.
void SpreadingFire_804A8A50::draw() {
    if (field_0x240 != 6) {
        s32 layer = field_0x4;
        if (layer >= 8) {
            fn_803075AC(mSerial, layer, &mPos, (field_0x280 >> 1) + 0x109, false, 2, -1, 0, 0, 0);
        }
    } else {
        drawSprite_801F06F0("enemy_both2_sob.bin", 0x18E, NULL, 0, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, 0x18E, false, 2, -1, 0, 0, 0);
    }
}

// 0x802BC88C.
void SpreadingFire_804A8A50::vfunc_0x54() {
    if (mEmitters[0] != NULL) {
        mEmitters[0]->becomeInvalidEmitter();
        mEmitters[0] = NULL;
    }
    if (mEmitters[1] != NULL) {
        mEmitters[1]->becomeInvalidEmitter();
        mEmitters[1] = NULL;
    }
    if (mEmitters[2] != NULL) {
        mEmitters[2]->becomeInvalidEmitter();
        mEmitters[2] = NULL;
    }
    if (mEmitters[3] != NULL) {
        mEmitters[3]->becomeInvalidEmitter();
        mEmitters[3] = NULL;
    }
}

// 0x802BC83C.
void SpreadingFire_804A8A50::vfunc_0x24() {
    vfunc_0x54();
    mAlive = false;
    getGameView_802D6A9C()->fn_802D73CC(mViewBit, false);
}
