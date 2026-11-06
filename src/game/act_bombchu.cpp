#include "game/GameAudio.h"
#include "game/BgRewrite.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/ViewLayer.h"
#include "game/act/act_bombchu.h"
#include "dolphin/mtx.h"
#include <math.h>

// Frames to hold each blast frame (indexed by field_0x250).
static s32 lbl_804AB440[9] = {4, 4, 2, 2, 4, 4, 4, 4, 4};

// The kind: the low four bits of field_0x90 (init keeps only them; every test
// masks again, clrlwi 28).
inline s32 Bombchu_804AB464::kind() const {
    return field_0x90 & 0xF;
}

// The players caught in the blast (a bit per player). Inlined in vfunc_0x54
// and in three update states (the same call pair each time).
inline s32 Bombchu_804AB464::hitPlayers() {
    if (gGameRoot_8053AAF8->field_0x7f == 0 || kind() == 4 || kind() == 3) {
        return fn_801F2B7C(field_0x114, -1, 1);
    }
    return fn_801F2B7C(0, -1, 1);
}

void Bombchu_804AB464::init() {
    field_0x26c = (field_0x90 >> 16) & 3;
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-7.0f, -4.0f, 7.0f, 7.0f);
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x260 = 0;
    field_0x264 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    mLife = 2;
    field_0x118 = 2;
    field_0x114 = 2;
    field_0x280 = 1.0f;
    field_0x27c = 0.0f;
    field_0x278 = (field_0x90 >> 4) & 1;
    field_0x268 = 0;
    field_0xb0 = 0x53308;
    field_0x270 = (field_0x90 >> 8) & 0xF;
    field_0x90 = field_0x90 & 0xF;
    field_0x264 = -1;
    field_0x274 = 0;
    field_0x275 = 1;
    field_0x277 = 1;
    field_0x234 = 0.3f;
    field_0x276 = 1;
    f32 speed = 2.5f;
    if (field_0x270 == 1) {
        speed = 1.25f;
    }
    switch (kind()) {
    case 0:
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        field_0x23c = 300;
        field_0x230 = 0;
        field_0xb0 = 0x73308;
        break;
    case 1:
        field_0x23c = 300;
        field_0x248 = 10;
        field_0x230 = 2;
        field_0xb0 = 0x73308;
        break;
    case 2:
        field_0x23c = 300;
        field_0x248 = 10;
        field_0x230 = 2;
        field_0xb0 = 0x73308;
        field_0x275 = 0;
        break;
    case 4: {
        field_0x23c = 300;
        field_0x248 = 120;
        field_0x230 = 2;
        field_0xb0 = 0x73308;
        s16 angle = 65536.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
        field_0x3c.x = speed * JMath::sincosTable_.sinShort(angle);
        field_0x3c.y = speed * JMath::sincosTable_.cosShort(angle);
        field_0x3c.z = 0.0f;
        field_0x275 = 0;
        break;
    }
    case 3:
        field_0x23c = 300;
        field_0x24c = 10;
        field_0x230 = 2;
        field_0xb0 = 0x73308;
        // The draw is read before field_0x277 is cleared (the original stores it after the load).
        f32 coin = gGameRoot_8053AAF8->mRandom.get_ufloat_1();
        field_0x277 = 0;
        if ((s32)(2.0f * coin) != 0) {
            field_0x3c.x = speed;
        } else {
            field_0x3c.x = -speed;
        }
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        field_0x275 = 0;
        break;
    case 5:
        field_0x23c = 300;
        field_0x90 = 0;
        field_0x230 = 2;
        field_0xb0 = 0x73308;
        fn_8023061C(field_0x26c, false);
        if (fn_8022F9E4(field_0x26c, mSerial, false, false) == true) {
            field_0x230 = 2;
        } else {
            field_0x230 = 1;
        }
        break;
    }
}

void Bombchu_804AB464::vfunc_0x54() {
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.z = 0.0f;
    mPos.z = 0.0f;
    field_0xb0 = 0;
    mBox.set(-16.0f, -16.0f, 16.0f, 16.0f);
    getGameView_802D6A9C()->fn_802D79E8(field_0x4, 4.0f, 0.0f, false);
    gGameRoot_8053AAF8->fn_80134DE4(1, 0);
    s32 hit = hitPlayers();
    field_0x250 = 0;
    field_0x238 = lbl_804AB440[field_0x250];
    fn_801F2B20(5);
    if (hit > 0) {
        for (int i = 0; i < 4; i++) {
            if ((hit >> i) & 1) {
                fn_80236074(i, 100, 0);
            }
        }
    }
    spawnEmitter_8013CC50(field_0x4, &mPos, 0x59, 1, 1, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(field_0x4, &mPos, 0x5A, 1, 1, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(field_0x4, &mPos, 0x5B, 1, 1, NULL, NULL, true, 1.0f);
    startSound_801F0E34(0x30001, 0);
    s32 code = fn_8022461C(&mPos, field_0x4);
    if (code == 0x39 || code == 0xD || code == 0x3B) {
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x4F3, -1, 1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x4F4, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x4F5, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x4F6, 0, 0, NULL, NULL, true, 1.0f);
        startSound_801F0E34(0x30022, 0);
    }
}

void Bombchu_804AB464::vfunc_0x48() {
    vfunc_0x54();
    JGeometry::TVec3f result;
    gBgRewrite_8053AC98->fn_8029EDB8(&mPos, 0, field_0x4, &result);
    field_0x230 = 3;
}

// FIXME: 99.88 %. Only registers and stack slots differ: in state 2 (layer flag
// clear) the hit mask and the loop index swap r29 / r30, and the inlined
// vfunc_0x48's result TVec3f and the frames table swap slots (0x1C / 0x2C).
void Bombchu_804AB464::update() {
    if (field_0x230 == 1) {
        if (fn_8022F9E4(field_0x26c, mSerial, false, false) == true) {
            field_0x230 = 2;
        }
        return;
    }
    int i;
    JGeometry::TBox2f box(mBox);
    JGeometry::TVec2f pos(mPos.x, mPos.y);
    box.addPos(pos);
    if (field_0xc4) {
        field_0x260 = 0;
        mPos.set(*fn_8023E7B0(field_0xc8));
        field_0x4 = getPlayerLayer_8023DE58(field_0xc8);
        field_0x264 = field_0xc8;
    } else {
        fn_8032506C();
    }
    if (field_0x278 == 1 && field_0x230 != 3) {
        field_0x23c = 300;
    }
    switch (field_0x230) {
    case 0:
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        mPos.z = 0.0f;
        for (i = 0; i < 4; i++) {
            if (fn_8023F264(i) && fn_80239C00(i, &box, field_0x4) && fn_8022F9E4(i, mSerial, true, false)) {
                field_0x258 = 0;
                field_0x230 = 2;
            }
        }
        break;
    case 2:
        if (!viewLayerFlag(field_0x4)) {
            if (!field_0xc4) {
                if (field_0x260 == 0) {
                    field_0x248 = 30;
                    field_0x260 = 1;
                }
                if (field_0x260 == 1) {
                    field_0x3c.z += -0.15f;
                }
            }
            if (mPos.z < 0.0f && field_0x3c.z < 0.0f) {
                mPos.z = 0.0f;
            }
            if (mPos.z == 0.0f && field_0x248 <= 0) {
                s32 hit = hitPlayers();
                if (hit > 0) {
                    for (i = 0; i < 4; i++) {
                        if ((hit >> i) & 1) {
                            fn_80236074(i, 100, 0);
                        }
                    }
                }
                fn_801F2B20(5);
            }
        } else {
            if (field_0xc4) {
                break;
            }
            if (field_0x260 == 0) {
                field_0x248 = 10;
                mPos.y -= mPos.z;
                mPos.z = 0.0f;
                field_0x3c.z = 0.0f;
                field_0x260 = 1;
            }
            faceMoveDirection();
            if (field_0x248 <= 0) {
                s32 hit = hitPlayers();
                if (hit > 0) {
                    for (i = 0; i < 4; i++) {
                        if ((hit >> i) & 1) {
                            fn_80236074(i, 100, 0);
                        }
                    }
                }
            }
            fn_801F2B20(5);
        }
        break;
    case 3: {
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        s32 hit = hitPlayers();
        fn_801F2B20(5);
        if (hit > 0) {
            for (i = 0; i < 4; i++) {
                if ((hit >> i) & 1) {
                    fn_80236074(i, 100, 0);
                }
            }
        }
        if (field_0x238 == 0) {
            field_0x250++;
            if (field_0x250 > 8) {
                mAlive = false;
            }
            field_0x238 = lbl_804AB440[field_0x250];
        }
        break;
    }
    }
    if (!field_0xc4 && field_0x230 != 3 && (field_0x230 == 0 || field_0x230 == 2)) {
        u8 explode = 0;
        if (fn_801A4F5C(mSerial, &box, &pos) != 0) {
            explode = 1;
        }
        if (field_0x248 <= 0 && fn_801F305C()) {
            explode = 2;
        }
        if (explode || mLife == 0) {
            Bombchu_804AB464::vfunc_0x48();
        }
    }
    s32 frames[4] = {0, 3, 2, 1};
    if (field_0x230 != 0 && !field_0xc4 && field_0x230 != 3 && field_0x23c % 4 == 0) {
        if (field_0x23c > 100) {
            field_0x250 ^= 1;
        } else {
            field_0x258 = (field_0x258 + 1) % 4;
            field_0x250 = frames[field_0x258];
        }
    }
    if (field_0x230 != 3 && field_0x23c == 0) {
        Bombchu_804AB464::vfunc_0x48();
    }
    if (field_0x230 != 0 && !field_0xc4) {
        if (field_0x238 > 0) {
            field_0x238--;
        }
        if (field_0x23c > 0) {
            field_0x23c--;
        }
        if (field_0x240 > 0) {
            field_0x240--;
        }
        if (field_0x244 > 0) {
            field_0x244--;
        }
        if (field_0x248 > 0) {
            field_0x248--;
        }
        if (field_0x24c > 0) {
            field_0x24c--;
        }
    }
}

void Bombchu_804AB464::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    bool held = false;
    s32 flags = 7;
    if (mPos.z == 0.0f) {
        s32 code = fn_8022461C(&mPos, field_0x4);
        if (code == 0x39) {
            flags = 0;
        }
    }
    PSMTXIdentity(mtx);
    f32 angle = field_0x27c;
    f32 s = sin(angle);
    f32 c = cos(angle);
    mtx[1][1] = 1.0f;
    mtx[0][0] = c;
    mtx[0][2] = s;
    mtx[2][0] = -s;
    mtx[2][2] = c;
    mtx[0][1] = mtx[1][0] = mtx[1][2] = mtx[2][1] = 0.0f;
    params.mMtx = mtx;
    params.field_0x1d = 1;
    params.field_0x1c = 1;
    if (field_0x230 == 0) {
        params.field_0x1d = 0;
        params.field_0x1c = 0;
    }
    if (field_0xc4) {
        params.field_0x20 = 0;
        params.field_0x1d = fn_8023CB24(field_0xc8) + 1;
        params.field_0x1c = fn_8023CA88(field_0xc8);
        held = true;
        flags = -1;
    }
    if (field_0x230 != 3) {
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x250 + 0x8C, &params, flags, 1.0f, 1.0f);
    }
    if (field_0x230 != 3) {
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x250 + 0x8C, 0, 2, -1, false, 0, 0);
    } else {
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x250 + 0xE8, 0, 2, -1, held, 0, 0);
    }
}

bool Bombchu_804AB464::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    s32 serial = mSerial;
    CollisionBoxes_8020D58C* boxes = fn_8020D58C();
    if (boxes->mLists[boxes->field_0x1].fn_8020D6AC(layer, pos, serial, NULL)) {
        return true;
    }
    s32 code = fn_8022461C(pos, layer);
    s32 tile = fn_80226850(pos, layer);
    if (code == 0x20) {
        return true;
    }
    if (tile == 0x230 || tile == 0x231) {
        return false;
    }
    if (code != 0 && code != 4 && code != 5 && code != 6 && code != 7 && code != 2 && code != 0x50 &&
        code != 0xE && code != 0xC && code != 0x27 && code != 0x28 && code != 0x29 && code != 0x2A &&
        code != 0x2B && code != 0x2C && code != 0x2D && code != 0x2E && code != 0x2F && code != 0xD &&
        code != 0x39 && code != 0x3A && code != 0x3B && code != 0x4C && code != 0x4D && code != 0x4E &&
        code != 0x4F && code != 0x24) {
        if (tile == 0x3A) {
            return false;
        }
        if (code == 0x25) {
            return (u8)!getGameView_802D6A9C()->field_0x171;
        }
        if (code == 0x26) {
            return (u8)(getGameView_802D6A9C()->field_0x171 == 1);
        }
        if (code != 0x22 || getGameView_802D6A9C()->field_0x170 == 1) {
            return true;
        }
    }
    return false;
}

void Bombchu_804AB464::fn_8032506C() {
    if (!viewLayerFlag(field_0x4)) {
        JGeometry::TVec3f wall;
        mPos += field_0x3c;
        if (fn_801F2618(&wall, false) == 1) {
            if (wall.x != 0.0f) {
                if (field_0x275) {
                    field_0x3c.x *= -0.7;
                    field_0x3c.y = -field_0x3c.x;
                    field_0x275 = 0;
                } else {
                    field_0x3c.x = -field_0x3c.x;
                }
            } else {
                if (field_0x275) {
                    field_0x3c.y *= -0.7;
                    field_0x3c.x = field_0x3c.y;
                    field_0x275 = 0;
                } else {
                    field_0x3c.y = -field_0x3c.y;
                }
            }
        }
    } else if (field_0x276 == 1) {
        if (field_0x260 != 1) {
            return;
        }
        mPos += JGeometry::TVec3f(field_0x3c.x, 0.0f, 0.0f);
        if (fn_801F2618(NULL, false)) {
            if (field_0x234 > 0.0f) {
                if (field_0x3c.x > 0.0f) {
                    field_0x3c.x = -field_0x3c.x;
                } else {
                    field_0x234 = -field_0x234;
                }
            } else {
                if (field_0x3c.x > 0.0f) {
                    field_0x234 = -field_0x234;
                } else {
                    field_0x3c.x = -field_0x3c.x;
                }
            }
            field_0x3c.y = 0.0f;
            field_0x276 = 0;
        }
        if (field_0x276 == 1) {
            field_0x3c.y += field_0x234;
            mPos += JGeometry::TVec3f(0.0f, field_0x3c.y, 0.0f);
            if (fn_801F2618(NULL, false)) {
                field_0x3c.y = 0.0f;
                field_0x277 = 0;
                return;
            }
            if (field_0x24c <= 0 && !field_0x277) {
                if (field_0x3c.x < 0.0f) {
                    if (field_0x234 > 0.0f) {
                        field_0x3c.x = -field_0x3c.x;
                    }
                    field_0x3c.y = -0.0f;
                    if (field_0x270 == 1) {
                        field_0x234 = 1.25f;
                    } else {
                        field_0x234 = 2.5f;
                    }
                } else {
                    if (field_0x234 < 0.0f) {
                        field_0x3c.x = -field_0x3c.x;
                    }
                    field_0x3c.y = 0.0f;
                    if (field_0x270 == 1) {
                        field_0x234 = -1.25f;
                    } else {
                        field_0x234 = -2.5f;
                    }
                }
                field_0x276 = 0;
            }
        }
    } else {
        if (field_0x260 != 1) {
            return;
        }
        mPos += JGeometry::TVec3f(0.0f, field_0x3c.x, 0.0f);
        if (fn_801F2618(NULL, false)) {
            if (field_0x234 > 0.0f) {
                if (field_0x3c.x > 0.0f) {
                    field_0x3c.x = -field_0x3c.x;
                } else {
                    field_0x234 = -field_0x234;
                }
            } else {
                if (field_0x3c.x > 0.0f) {
                    field_0x234 = -field_0x234;
                } else {
                    field_0x3c.x = -field_0x3c.x;
                }
            }
            field_0x3c.y = 0.0f;
            field_0x276 = 1;
        }
        if (field_0x276 == 0) {
            field_0x3c.y += field_0x234;
            mPos += JGeometry::TVec3f(field_0x3c.y, 0.0f, 0.0f);
            if (fn_801F2618(NULL, false)) {
                field_0x3c.y = 0.0f;
                return;
            }
            if (field_0x3c.x < 0.0f) {
                if (field_0x234 > 0.0f) {
                    field_0x3c.x = -field_0x3c.x;
                }
                field_0x3c.y = 0.0f;
                if (field_0x270 == 1) {
                    field_0x234 = 1.25f;
                } else {
                    field_0x234 = 2.5f;
                }
            } else {
                if (field_0x234 < 0.0f) {
                    field_0x3c.x = -field_0x3c.x;
                }
                field_0x3c.y = 0.0f;
                if (field_0x270 == 1) {
                    field_0x234 = -1.25f;
                } else {
                    field_0x234 = -2.5f;
                }
            }
            field_0x276 = 1;
        }
    }
}

// Turns the sprite to the crawl direction. Inlined into update (state 2);
// the original defined it here, before fn_8032506C: its out-of-line copy was
// dead-stripped, but its 0.0f, pi, 3pi/2 and pi/2 open the unit's .sdata2 pool.
void Bombchu_804AB464::faceMoveDirection() {
    if (field_0x276 == 1) {
        if (field_0x234 > 0.0f) {
            field_0x27c = 0.0f;
        } else {
            field_0x27c = 3.1415927f;
        }
    } else {
        if (field_0x234 > 0.0f) {
            field_0x27c = 4.712389f;
        } else {
            field_0x27c = 1.5707964f;
        }
    }
}

Bombchu_804AB464::~Bombchu_804AB464() {}
