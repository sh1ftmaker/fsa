#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameRootSub20.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/RotMatrix.h"
#include "game/SpriteParams.h"
#include "game/ViewLayer.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/act_magic_circle.h"

static const u32 lbl_8046C100[3] = {ACTOR_HNRY, ACTOR_ESRY, ACTOR_NULL};

static inline s32 getTargetLayer(u32 param) {
    s32 layer = (param >> 24) & 7;
    return layer == 0 ? layer : layer + 7;
}

static inline u32 getTargetX(u32 param) {
    return ((param >> 16) & 0x3F) * 8;
}

static inline u32 getTargetY(u32 param) {
    return ((param >> 8) & 0x3F) * 8;
}

MagicCircle_804A7960::~MagicCircle_804A7960() {
    releaseEmitter_801EE434(mEmitter274);
    releaseEmitter_801EE434(mEmitter278);
}

// FIXME: 99.89%, regalloc-only (f2/f31 and f1/f30 swapped in the two destMin fadds)
void MagicCircle_804A7960::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    mLife = 0;
    field_0x230 = 0;
    field_0xb0 = 3;
    field_0x23c = 0;
    field_0x238 = 0;
    field_0x248 = 0x78;
    field_0x234 = 0;
    field_0x251 = 0;
    mEmitter274 = NULL;
    mEmitter278 = NULL;

    JGeometry::TBox2f* destBox = layerBox(getTargetLayer(field_0x90));
    f32 destMinX = destBox->i.x;
    f32 destMinY = destBox->i.y;

    f32 srcMinX;
    f32 srcMinY;
    if (getTargetLayer(field_0x90) != 0) {
        JGeometry::TBox2f* srcBox = layerBox(field_0x4);
        srcMinX = srcBox->i.x;
        srcMinY = srcBox->i.y;
    } else {
        srcMinX = srcMinY = 0.0f;
    }

    if (getTargetX(field_0x90) == 0 && getTargetY(field_0x90) == 0) {
        field_0x268.x = destMinX + (mPos.x - srcMinX);
        field_0x268.y = destMinY + (mPos.y - srcMinY);
        field_0x268.z = 0.0f;
    } else {
        field_0x268.set(8.0f + ((f32)getTargetX(field_0x90) + destMinX),
                        8.0f + ((f32)getTargetY(field_0x90) + destMinY),
                        0.0f);
    }

    field_0x252[0] = 0;
    field_0x252[1] = 0;
    field_0x252[2] = 0;
    field_0x252[3] = 0;
    field_0x258 = 0;
    field_0x250 = 0;
    field_0x260 = 0;
    field_0x262 = 0;
    field_0x263 = 1;
    field_0x264 = 0;
    field_0x24c = (field_0x90 >> 4) & 0xF;
    field_0x25c = field_0x90 & 7;
    if (field_0x25c == 6) {
        s32 playerCount = gGameRoot_8053AAF8->fn_80138A30();
        if (playerCount >= 2) {
            field_0x25c = (s32)((f32)playerCount * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 1;
        } else {
            field_0x25c = 0;
        }
    } else if (field_0x25c == 5) {
        field_0x260 = 1;
    }

    if (field_0x16c == -1) {
        vfunc_0x10();
    }
}

void MagicCircle_804A7960::vfunc_0x10() {
    field_0x250 = 1;
    for (s32 i = 0; i < 4; i++) {
        if (fn_80303750(i)) {
            field_0x250 = 0;
            break;
        }
    }
    if (field_0x250 == 1) {
        field_0x234 = fn_8028B850();
    }
    field_0x240 = (field_0x234 == 1) ? 1.0f : 0.0f;
    field_0x244 = (field_0x234 == 1) ? 0x6E : 0;
}

// Inlining off for fn_8028B850 (docs/INLINING.md, pragma rule 1):
// MagicCircle_804A7960::vfunc_0x10 and MagicCircle_804A7960::update call it out of line.
#pragma auto_inline off
bool MagicCircle_804A7960::fn_8028B850() {
    if (field_0x250 == 0) {
        for (s32 i = 0; i < 4; i++) {
            if (fn_80303750(i)) {
                field_0x230++;
                if (field_0x230 >= 600) {
                    field_0x230 = 600;
                }
                field_0x258 = 0;
                return 0;
            }
        }
    }
    if (field_0x258 >= 3) {
        field_0x250 = 1;
    } else {
        field_0x258++;
        return 0;
    }
    if ((field_0x90 >> 27) != 0) {
        if (!vfunc_0x34() && field_0x24c != 0 && field_0x24c != 2 && field_0x24c != 3) {
            return 0;
        }
    }
    switch (field_0x24c) {
    case 0:
        return 1;
    case 1:
        field_0x230++;
        if (field_0x230 >= 600) {
            field_0x230 = 0;
            return field_0x234 == 0;
        }
        return field_0x234;
    case 2:
        return vfunc_0x34();
    case 3:
        return !vfunc_0x34();
    case 4:
        if (field_0x230 >= 600) {
            return 1;
        }
        field_0x230++;
        return 0;
    case 5:
        if (field_0x230 >= 600) {
            return 0;
        }
        field_0x230++;
        return 1;
    case 6:
    case 7:
    case 8:
    case 9:
        return field_0x23c < field_0x24c - 5;
    default:
        return 1;
    }
}
#pragma auto_inline reset

// FIXME: 99.81%, scheduling+regalloc (register choice at 0x90C and x/y evaluation order in dest.set)
void MagicCircle_804A7960::update() {
    bool prevActive = field_0x234;
    field_0x262 = 0;
    field_0x234 = fn_8028B850();
    if (prevActive == 0 && field_0x234 == 1) {
        field_0x263 = 1;
    } else if (prevActive == 1 && field_0x234 == 0) {
        field_0x264 = 1;
    }

    s32 colorIdx = field_0x25c;
    if (colorIdx >= 5) {
        colorIdx = 0;
    }

    if (field_0x234 != 0 || field_0x238 > 0) {
        if (field_0x240 < 1.0f) {
            if (field_0x263 == 1) {
                fn_801F0D20(0x2E);
                field_0x263 = 0;
            }
            field_0x240 += 0.02f;
            if (field_0x240 >= 1.0f) {
                field_0x240 = 1.0f;
                field_0x248 = 10;
                if (((field_0x90 >> 3) & 1) == 1) {
                    fn_801F0D20(3);
                }
            }
        }
        if (field_0x240 >= 1.0f) {
            if (field_0x260 == 1) {
                lbl_8053AED0->fn_8043D67C(0xE7, 0, 0);
                field_0x260 = 0;
            }
            field_0x244++;
            if (field_0x244 >= 0x6E) {
                field_0x244 = 0x6E;
            }
            s32 frame = field_0x244;
            GameRootSub20_801902C0* shared = fn_801902C0();
            if (shared->field_0x5ec < 8) {
                GameRootSub20_801902C0::MagicCircle& entry = shared->mMagicCircles[shared->field_0x5ec];
                entry.mPos.x = mPos.x;
                entry.mPos.y = mPos.y;
                entry.mPos.z = mPos.z;
                entry.field_0xc = colorIdx;
                entry.field_0x10 = frame;
                shared->field_0x5ec++;
            }
            startSound_801F0E34(0x3002D, 0);
            if (mEmitter274 == NULL) {
                mEmitter274 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x2A6, 0, 0, NULL, NULL, true, 1.0f);
                if (mEmitter274 != NULL) {
                    if (field_0x25c == 0 || field_0x25c == 5) {
                        mEmitter274->setGlobalPrmColor(0x5A, 0xF3, 0xE9);
                        mEmitter274->setGlobalEnvColor(0x30, 0xA0, 0x5A);
                    } else if (field_0x25c == 1) {
                        mEmitter274->setGlobalPrmColor(0xA0, 0xFF, 0x78);
                        mEmitter274->setGlobalEnvColor(0x40, 0x80, 0x30);
                    } else if (field_0x25c == 2) {
                        mEmitter274->setGlobalPrmColor(0xFF, 0x78, 0x40);
                        mEmitter274->setGlobalEnvColor(0xFF, 0xA0, 0x40);
                    } else if (field_0x25c == 3) {
                        mEmitter274->setGlobalPrmColor(0x00, 0xA0, 0xC8);
                        mEmitter274->setGlobalEnvColor(0x40, 0x78, 0xA0);
                    } else if (field_0x25c == 4) {
                        mEmitter274->setGlobalPrmColor(0xC8, 0x9C, 0xE5);
                        mEmitter274->setGlobalEnvColor(0x78, 0x30, 0xA0);
                    }
                }
            }
            if (mEmitter278 == NULL) {
                mEmitter278 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x2A7, 0, 0, NULL, NULL, true, 1.0f);
                if (mEmitter278 != NULL) {
                    if (field_0x25c == 0 || field_0x25c == 5) {
                        mEmitter278->setGlobalPrmColor(0xFF, 0xFF, 0xFF);
                        mEmitter278->setGlobalEnvColor(0x5A, 0xB0, 0xB1);
                    } else if (field_0x25c == 1) {
                        mEmitter278->setGlobalPrmColor(0xF2, 0xFF, 0xC4);
                        mEmitter278->setGlobalEnvColor(0x49, 0xC3, 0x20);
                    } else if (field_0x25c == 2) {
                        mEmitter278->setGlobalPrmColor(0xFF, 0xA3, 0xA3);
                        mEmitter278->setGlobalEnvColor(0x98, 0x2A, 0x29);
                    } else if (field_0x25c == 3) {
                        mEmitter278->setGlobalPrmColor(0xA3, 0xC4, 0xFF);
                        mEmitter278->setGlobalEnvColor(0x27, 0x29, 0xA7);
                    } else if (field_0x25c == 4) {
                        mEmitter278->setGlobalPrmColor(0xF1, 0xB4, 0xFF);
                        mEmitter278->setGlobalEnvColor(0x84, 0x28, 0x84);
                    }
                }
            }
        } else {
            return;
        }
    } else {
        if (field_0x244 > 0) {
            field_0x244--;
            s32 frame = field_0x244;
            GameRootSub20_801902C0* shared = fn_801902C0();
            if (shared->field_0x5ec < 8) {
                GameRootSub20_801902C0::MagicCircle& entry = shared->mMagicCircles[shared->field_0x5ec];
                entry.mPos.x = mPos.x;
                entry.mPos.y = mPos.y;
                entry.mPos.z = mPos.z;
                entry.field_0xc = colorIdx;
                entry.field_0x10 = frame;
                shared->field_0x5ec++;
            }
        } else {
            if (field_0x264 == 1) {
                fn_801F0D20(0x2F);
                field_0x264 = 0;
            }
            field_0x240 -= 0.02f;
        }
        if (mEmitter274 != NULL) {
            mEmitter274->becomeInvalidEmitter();
            mEmitter274 = NULL;
        }
        if (mEmitter278 != NULL) {
            mEmitter278->becomeInvalidEmitter();
            mEmitter278 = NULL;
        }
        if (field_0x240 <= 0.0f) {
            field_0x240 = 0.0f;
        }
        if (field_0x244 <= 0) {
            field_0x244 = 0;
        }
        if (field_0x25c == 5 && field_0x24c == 0 && field_0x240 <= 0.0f && field_0x244 <= 0 && field_0x23c > 0) {
            fn_801F3668();
        }
        return;
    }

    JGeometry::TBox2f box(mBox.i, mBox.f);
    box.addPos(mPos.x, mPos.y);

    if (field_0x238 > 0) {
        field_0x238--;
    }
    if (field_0x248 > 0) {
        field_0x248--;
        return;
    }

    for (s32 i = 0; i < 4; i++) {
        if (fn_8022F478(i)) {
            continue;
        }
        if (gGameRoot_8053AAF8->field_0x7e && !fn_8022C8EC(i)) {
            if (getPlayerLayer_8023DE58(i) != getTargetLayer(field_0x90)) {
                continue;
            }
        }
        if (field_0x25c >= 1 && field_0x25c <= 4 && field_0x25c != i + 1) {
            continue;
        }
        if (fn_8023B0F4(i, &box, field_0x4, false, 0.0f) && !field_0x251) {
            if (field_0x252[i] == 1) {
                continue;
            }
            field_0x252[i] = 1;
            s32 warpType = 6;
            if (field_0x25c == 5) {
                warpType = 20;
            }
            if (fn_802317F0(i, getTargetLayer(field_0x90), &mPos, &field_0x268, warpType)) {
                field_0x238 = 180;
                if ((field_0x24c >= 6 && field_0x24c <= 9) || (field_0x25c == 5 && field_0x24c == 0)) {
                    field_0x23c++;
                    if (field_0x23c >= field_0x24c - 5) {
                        field_0x251 = 1;
                    }
                    if (field_0x25c == 5 && field_0x24c == 0) {
                        field_0x251 = 1;
                        field_0x24c = 6;
                    }
                }
            }
        } else {
            box.i.x -= 1.0f;
            box.i.y -= 1.0f;
            box.f.x += 1.0f;
            box.f.y += 1.0f;
            if (getField_0x4() == getPlayerLayer_8023DE58(i) && !fn_8023B0F4(i, &box, field_0x4, false, 100.0f)) {
                field_0x252[i] = 0;
            }
        }
    }

    field_0x262 = 1;
    s32 serial = fn_801F26CC(lbl_8046C100);
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        f32 viewX;
        f32 viewY;
        if (getTargetLayer(field_0x90) == 0) {
            JGeometry::TBox2f* viewBox = layerBox(getTargetLayer(field_0x90));
            viewX = viewBox->i.x;
            viewY = viewBox->i.y;
        } else {
            viewX = viewY = 0.0f;
        }
        if (actor != NULL && (actor->mActorId != ACTOR_FIRE || ((actor->field_0x90 >> 6) & 3) != 1) && !actor->field_0xc4) {
            JGeometry::TVec3f dest;
            dest.set(8.0f + ((f32)getTargetX(field_0x90) + viewX),
                     8.0f + ((f32)getTargetY(field_0x90) + viewY),
                     0.0f);
            actor->fn_801F349C(getTargetLayer(field_0x90), &dest);
        }
    }
}

// FIXME: 93.63%, scheduling+regalloc (setRotScaleMtx inlining and SpriteParams copy)
void MagicCircle_804A7960::draw() {
    if (field_0x240 <= 0.0f) {
        return;
    }
    SpriteParams_801F5AE0 params;
    params.field_0x1d = -1;
    Mtx mtx;
    f32 scale = field_0x240;
    JGeometry::TVec3f rot(0.0f, 2.0f * (3.14f * (1.0f - scale)), 0.0f);
    setRotScaleMtx(mtx, rot, scale);
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    params.mMtx = mtx;
    drawSprite_801F06F0("enemy_gc_sob.bin", field_0x25c + 0x129, &params, -1, 1.0f, 1.0f);
}

