#include "game/GameAudio.h"
#include "game/act/tu_803276C4.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Effect.h"
#include "game/Map.h"
#include "game/Player.h"
#include "dolphin/mtx.h"

// The "TIMG" texture names (by field_0x250).
static const char* const lbl_804AB518[4] = {"said_a.bti", "said_b.bti", "said_c.bti", "said_d.bti"};

// FIXME(shotgun): 85.7 %, missing/extra: the OSIN search loop and the box constants
void cls_804AB528::init() {
    field_0x94 = field_0x90 & 0xF;
    field_0x240 = (field_0x90 >> 4) & 0xF;
    field_0x248 = (field_0x90 >> 8) & 0xF;
    field_0x25a = (field_0x90 >> 12) & 1;
    s32 serial = -1;
    while ((serial = getActorManager_801F856C()->fn_801F78F4(ACTOR_OSIN, serial)) >= 0) {
        if (serial == mSerial) {
            continue;
        }
        cls_804AB528* other = static_cast<cls_804AB528*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
        if (other != NULL && other->field_0x248 == field_0x248) {
            if (!fn_801F6758(&other->mPos, other->field_0x4, 0.0f)) {
                mAlive = false;
            }
        }
    }
    mLife = 10000;
    mPos.y -= 8.0f;
    field_0x260 = mPos;
    field_0x230 = 0;
    field_0x234 = 0;
    field_0x244 = 0;
    field_0x25c = 1.0f;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x24c = 10;
    field_0x250 = 0;
    field_0x258 = 0;
    field_0x259 = 0;
    field_0x254[0] = 0;
    field_0x254[1] = 0;
    field_0x254[2] = 0;
    field_0x254[3] = 0;
    if (field_0x25a == 1) {
        field_0x25c = 0.0f;
    }
    if (field_0x240 == 0) {
        field_0x80.i.x = -15.0f;
        field_0x80.i.y = -7.0f;
        field_0x80.f.x = 15.0f;
        field_0x80.f.y = 7.0f;
        mBox.i.x = -15.0f;
        mBox.i.y = -7.0f;
        mBox.f.x = 15.0f;
        mBox.f.y = 7.0f;
    } else if (field_0x240 == 1) {
        field_0x80.i.x = -31.0f;
        field_0x80.i.y = -15.0f;
        field_0x80.f.x = 31.0f;
        field_0x80.f.y = 15.0f;
        mBox.i.x = -31.0f;
        mBox.i.y = -15.0f;
        mBox.f.x = 31.0f;
        mBox.f.y = 15.0f;
    }
}

// FIXME(shotgun): 90.9 %, missing/extra + reloc-name
void cls_804AB528::update() {
    if (field_0x25c < 1.0f) {
        if (vfunc_0x34() == 1 && field_0x25a == 1) {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x503, 1, 0, NULL, NULL, true, (f32)(field_0x240 + 1));
            startSound_801F0E34(0x30003, 0);
            field_0x25a = 0;
        }
        if (field_0x25a == 0) {
            field_0x25c += 0.1f;
            if (field_0x25c >= 1.0f) {
                field_0x25c = 1.0f;
            }
        }
        return;
    }

    JGeometry::TBox2f worldBox;
    worldBox.i.x = mBox.i.x + mPos.x;
    worldBox.i.y = mBox.i.y + mPos.y;
    worldBox.f.x = mBox.f.x + mPos.x;
    worldBox.f.y = mBox.f.y + mPos.y;
    JGeometry::TBox2f box = worldBox;
    box.i.x += 4.0f;
    box.i.y -= 2.0f;
    box.f.x -= 4.0f;

    mPos += field_0x3c;

    if (field_0x94 == 2 || field_0x94 == 3) {
        for (s32 i = 3; i >= 0; i--) {
            if (fn_80230188(i) && field_0x259 == 1) {
                JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
                pos += field_0x3c;
                fn_80230498(i, &pos, fn_80230CFC(i) + 0x3C, 0xFF, -0x80, -0x80, true);
            }
        }
    }

    u8 blocked = 0;
    if (!fn_801F6758(&mPos, field_0x4, -40.0f)) {
        blocked = fn_801F2618(NULL, false);
    }
    field_0x238 = 0;
    fn_80239914(&worldBox, field_0x4, mSerial);

    switch (field_0x94) {
    case 0:
    case 1:
        field_0x254[0] = 0;
        field_0x254[1] = 0;
        field_0x254[2] = 0;
        field_0x254[3] = 0;
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i) && !fn_8023EA28(i) && field_0x254[i] == 0) {
                field_0x254[i] = fn_8023B0F4(i, &box, field_0x4, false, 0.0f);
                if (field_0x254[i] != 0 && getPlayerPos_8023E724(i)->y < mPos.y) {
                    if (fn_8022CAF8(i) == 4) {
                        field_0x238++;
                    } else {
                        field_0x238 += fn_8022E970(i);
                    }
                    for (s32 j = 0; j < 4; j++) {
                        if (field_0x254[j] == 0 && fn_8022CAF8(j) != 4 && i != j && fn_8022E438(i, j) == 1) {
                            field_0x254[j] = 1;
                        }
                    }
                }
            }
        }
        for (s32 i = 0; i < 4; i++) {
            if (field_0x254[i] == 1 && (fn_8023ED8C(i) || fn_8023EBB8(i))) {
                field_0x238++;
            }
        }
        if (field_0x258 == 1) {
            return;
        }
        if (field_0x23c != 0 && field_0x238 < field_0x23c) {
            field_0x238 = field_0x23c;
        }
        if (field_0x238 > 1) {
            if (blocked == 0 && 48.0f * (f32)(field_0x238 - 1) + field_0x260.y > mPos.y) {
                field_0x3c.y = 0.5f;
                field_0x234 = 1;
                if (field_0x94 != 0 && field_0x238 >= field_0x23c) {
                    field_0x23c = field_0x238;
                }
            } else if (blocked == 0 && 48.0f * (f32)(field_0x238 - 1) + field_0x260.y < mPos.y) {
                field_0x3c.y = -0.5f;
                field_0x234 = 0;
            } else {
                field_0x3c.y = 0.0f;
                field_0x234 = 0;
            }
        } else if (field_0x238 <= 1 && field_0x260.y < mPos.y) {
            field_0x3c.y = -0.5f;
            field_0x234 = 0;
        } else {
            field_0x3c.y = 0.0f;
            field_0x234 = 0;
        }
        if (field_0x94 != 0 && field_0x238 >= 4) {
            if (48.0f * (f32)(field_0x238 - 1) + field_0x260.y <= mPos.y || blocked != 0) {
                field_0x258 = 1;
                field_0x3c.y = 0.0f;
                if (blocked == 0) {
                    field_0x234 = 0;
                } else {
                    field_0x234 = 1;
                }
            }
        }
        break;
    case 2:
    case 3:
        switch (field_0x230) {
        case 0:
            if (!getGameView_802D6A9C()->fn_802DCD0C()) {
                for (s32 i = 0; i < 4; i++) {
                    if (fn_80233EDC(i) == 1) {
                        break;
                    }
                    if (fn_8023F264(i) && !fn_8023EA28(i)) {
                        field_0x254[i] = fn_8023B0F4(i, &box, field_0x4, false, 0.0f);
                        if (field_0x254[i] != 0 && getPlayerPos_8023E724(i)->y <= worldBox.i.y) {
                            field_0x238++;
                            if (fn_8023ED8C(i)) {
                                field_0x238++;
                            }
                        }
                    }
                }
            }
            if (field_0x238 >= 4) {
                for (s32 i = 0; i < 4; i++) {
                    fn_80230AD4(i);
                }
                field_0x230 = 1;
                field_0x259 = 1;
                field_0x3c.y = 0.5f;
            }
            break;
        case 1:
            if (mPos.y - field_0x260.y >= 8.0f) {
                if (field_0x94 == 2) {
                    field_0x3c.x = 1.0f;
                } else if (field_0x94 == 3) {
                    field_0x3c.x = -1.0f;
                }
                field_0x3c.y = 0.0f;
                field_0x230 = 2;
                field_0x244 = 240;
            }
            break;
        case 2:
            field_0x244--;
            if (field_0x94 == 2) {
                if (mPos.x - getGameView_802D6A9C()->mVisibleBox.f.x > 40.0f) {
                    s32 row = getGameView_802D6A9C()->field_0x18;
                    getGameView_802D6A9C()->fn_802D9FE8(getGameView_802D6A9C()->field_0x14 + 1, row);
                    gGameRoot_8053AAF8->fn_80136798(0x11, 0);
                }
            } else if (field_0x94 == 3) {
                if (mPos.x - getGameView_802D6A9C()->mVisibleBox.i.x < -40.0f) {
                    s32 row = getGameView_802D6A9C()->field_0x18;
                    getGameView_802D6A9C()->fn_802D9FE8(getGameView_802D6A9C()->field_0x14 - 1, row);
                    gGameRoot_8053AAF8->fn_80136798(0x11, 0);
                }
            }
            if (blocked != 0 || field_0x244 == 0) {
                field_0x260.x = mPos.x;
                field_0x260.y = mPos.y;
                field_0x260.z = mPos.z;
                field_0x3c.x = 0.0f;
                field_0x3c.y = -0.5f;
                field_0x230 = 3;
            }
            break;
        case 3:
            if (field_0x260.y - mPos.y >= 8.0f) {
                field_0x260.x = mPos.x;
                field_0x260.y = mPos.y;
                field_0x260.z = mPos.z;
                field_0x3c.z = 0.0f;
                field_0x3c.y = 0.0f;
                field_0x3c.x = 0.0f;
                for (s32 i = 0; i < 4; i++) {
                    fn_80230224(i, 0);
                }
                fn_80231368();
                if (field_0x94 == 2) {
                    field_0x94 = 3;
                } else if (field_0x94 == 3) {
                    field_0x94 = 2;
                }
                field_0x230 = 4;
            }
            break;
        case 4:
            for (s32 i = 0; i < 4; i++) {
                if (fn_8023F264(i) && !fn_8023EA28(i)) {
                    field_0x254[i] = fn_8023B0F4(i, &box, field_0x4, false, 0.0f);
                    if (field_0x254[i] != 0 && getPlayerPos_8023E724(i)->y <= worldBox.i.y) {
                        field_0x238++;
                        if (fn_8023ED8C(i)) {
                            field_0x238++;
                        }
                    }
                }
            }
            if (field_0x259 == 1 && field_0x238 > 0) {
                field_0x259 = 0;
            } else if (field_0x259 == 0 && field_0x238 <= 0) {
                field_0x230 = 0;
            }
            break;
        }
        break;
    }

    field_0x24c--;
    if (field_0x24c <= 0) {
        field_0x24c = 10;
        field_0x250++;
        field_0x250 &= 3;
    }
}

// FIXME(shotgun): 70.2 %, missing/extra: the Mtx diagonal setup and the sprite call order
void cls_804AB528::draw() {
    if (field_0x25c == 0.0f) {
        return;
    }
    if (getGameView_802D6A9C()->field_0x4 == 7 && getGameView_802D6A9C()->field_0x8 == 2) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', lbl_804AB518[field_0x250]);
        if (field_0x4 < 8) {
            gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, res, NULL);
        }
        return;
    }
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    PSMTXIdentity(mtx);
    f32 scale = (f32)(field_0x240 + 1) * field_0x25c;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][2] = 0.0f;
    mtx[0][0] = scale;
    mtx[1][1] = scale;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = scale;
    params.mMtx = mtx;
    s32 frame = field_0x234 + 0x141;
    if (field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
    }
    fn_803075AC(mSerial, field_0x4, &mPos, frame, 0, 2, -1, field_0x240 != 0, 0, 0);
}

cls_804AB528::~cls_804AB528() {}
