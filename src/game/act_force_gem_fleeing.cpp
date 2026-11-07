#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/GameView.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/RotMatrix.h"
#include "game/act/act_force_gem_fleeing.h"


// Sprite ids per kind (field_0x268), four per state group.
static const s32 lbl_804AB130[] = {0x11, 0x15, 0x19, 0x1D, 0x11, 0x15, 0x19, 0x1D, 0x11, 0x15, 0x19, 0x1D};

// Table indexed by field_0x268 (update, state 1).
static const s32 lbl_804AB160[] = {100, 150, 200, 300};

// FIXME(shotgun): 99.4 %, different
void FleeingForceGem_804AB170::init() {
    field_0x268 = field_0x90 & 3;
    field_0x94 = (field_0x90 >> 16) & 0xFF;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x260 = 0;
    for (int i = 0; i < 5; i++) {
        field_0x240[i] = 0;
    }
    field_0x274 = 0;
    field_0x276 = field_0x274;
    mLife = 10000;
    field_0x264 = -1;
    field_0x98 = 0;
    field_0x288.x = mPos.x;
    field_0x288.y = mPos.y;
    field_0x288.z = mPos.z;
    field_0x260 = 100;
    field_0x238 = 1.0f;
    field_0x23c = 0.0f;
    field_0x278 = 0;
    field_0x270 = field_0x4;
    field_0x234 = 30.0f;
    field_0x25c = (s32)(gGameRoot_8053AAF8->mRandom.get_ufloat_1() * 65535.0f);
    field_0xb0 = 0x2000A;
    field_0x294.z = 0.0f;
    field_0x294.y = 0.0f;
    field_0x294.x = 0.0f;
    field_0x26c = 0;
    switch (field_0x94) {
    case 0:
        field_0x268 = field_0x90 & 3;
        field_0x230 = 0;
        field_0x278 = 1;
        break;
    case 1:
        field_0x268 = field_0x90 & 3;
        field_0x230 = 4;
        break;
    case 2:
        field_0x230 = 8;
        break;
    }
    field_0x27c.x = 0.0f;
    field_0x27c.y = 0.0f;
    field_0x27c.z = 0.0f;
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.z = 0.0f;
    field_0xb8 = 1;
    field_0x80.i.x = -6.0f;
    field_0x80.i.y = -6.0f;
    field_0x80.f.x = 6.0f;
    field_0x80.f.y = 6.0f;
    mBox.i.x = -6.0f;
    mBox.i.y = -6.0f;
    mBox.f.x = 6.0f;
    mBox.f.y = 6.0f;
    if (gGameRoot_8053AAF8->field_0x7f == 0) {
        fn_801F3668();
    } else {
        field_0x194 = 1;
    }
}

// FIXME(shotgun): 89.2 %, missing/extra
bool FleeingForceGem_804AB170::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    s32 tile = (u16)fn_8022461C(pos, layer);
    if (tile == 3) {
        field_0x3c.z = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.x = 0.0f;
        field_0x230 = 5;
        return false;
    }
    if (tile == 0xE) {
        if (field_0x230 != 7) {
            field_0x240[2] = 10;
            field_0x230 = 7;
        }
        return false;
    }
    if (tile == 0xD) {
        if (field_0x230 != 6) {
            field_0x240[2] = 10;
            field_0x230 = 6;
        }
        return false;
    }
    if (tile == 0 || tile == 0x27 || tile == 2) {
        return false;
    }
    if (tile == 0x28 && (s32)mPos.y % 16 > 8) {
        return false;
    }
    if (tile == 0x29 && (s32)mPos.y % 16 < 8) {
        return false;
    }
    if (tile == 0x2C && (s32)mPos.y % 16 > 8) {
        return false;
    }
    if (tile == 0x2D && (s32)mPos.y % 16 < 8) {
        return false;
    }
    if (tile == 0x2A && (s32)mPos.x % 16 > 8) {
        return false;
    }
    if (tile == 0x2B && (s32)mPos.x % 16 < 8) {
        return false;
    }
    if (tile == 0x2E && (s32)mPos.x % 16 > 8) {
        return false;
    }
    if (tile == 0x2F && (s32)mPos.x % 16 < 8) {
        return false;
    }
    return true;
}

// 0x8031C598.
// FIXME(shotgun): 34.4 %. States 3-10 of the state switch (0x8031CD60-0x8031D8B8) and the end of the function
// (the frame counter after field_0x240++, 0x8031D9B8-0x8031DA8C) are not written.
void FleeingForceGem_804AB170::update() {
    f32 dist = 0.0f;
    u16 angle = 0;
    s32 dir = 0;
    s32 player = 0;
    bool near = false;
    JGeometry::TVec2f pos2(mPos.x, mPos.y);
    field_0x288.x = mPos.x;
    field_0x288.y = mPos.y;
    field_0x288.z = mPos.z;
    JGeometry::TBox2f box = mBox;
    box.addPos(pos2);
    if (field_0x94 != 1 && field_0x240[4] == 0) {
        addCalc2_801F71A4(&field_0x3c.x, field_0x27c.x, 0.1f, 1.0f);
        addCalc2_801F71A4(&field_0x3c.y, field_0x27c.y, 0.1f, 1.0f);
    }
    if (field_0x94 != 0) {
        mPos += field_0x3c;
    }
    if (field_0x94 == 3 && field_0x230 != 7 && field_0x230 != 6) {
        fn_801F2618(NULL, false);
    }
    if (field_0x230 == 0 || field_0x230 == 3 || field_0x230 == 4 || field_0x230 == 10) {
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023A51C(i) && field_0x4 == getPlayerLayer_8023DE58(i)) {
                JGeometry::TVec3f* playerPos = fn_8023A478(i);
                f32 dx = playerPos->x - mPos.x;
                f32 dy = playerPos->y - mPos.y;
                if (dx * dx + dy * dy <= 6400.0f) {
                    near = true;
                    break;
                }
            }
        }
        if (near && field_0x3c.z <= 0.0f && mPos.z <= 0.0f) {
            field_0x3c.z = 5.0f;
            switch (fn_8023A240()) {
            case 0:
                field_0x3c.set(-2.0f, 0.0f, field_0x3c.z);
                break;
            case 1:
                field_0x3c.set(2.0f, 0.0f, field_0x3c.z);
                break;
            case 2:
                field_0x3c.set(0.0f, -2.0f, field_0x3c.z);
                break;
            case 3:
                field_0x3c.set(0.0f, 2.0f, field_0x3c.z);
                break;
            }
            field_0x27c = field_0x3c;
            field_0x94 = 3;
        }
        if (mPos.z > 0.0f) {
            field_0x3c.z -= 0.5f;
        } else if (mPos.z <= 0.0f && field_0x3c.z < 0.0f) {
            mPos.z = 0.0f;
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
            field_0x94 = 0;
            if (field_0x278 == 1) {
                fn_801F0D20(0x42);
                field_0x278 = 0;
            }
        }
        s32 hit = fn_8023AF14(&box, field_0x4, false, mPos.z);
        fn_801EEC98(mSerial, &angle, &dir, &dist, &player, 0, 0);
        if (hit < 0) {
            for (s32 i = 0; i < 4; i++) {
                if (!fn_80230188(i) && !fn_8023EA28(i)) {
                    if (fn_8023B9CC(i, &box, field_0x4, &pos2, true) == 1 ||
                        fn_8023B848(i, &box, field_0x4, &pos2) == 1) {
                        hit = i;
                        break;
                    }
                }
            }
        }
        if (hit >= 0 && !fn_80234C5C(hit) && !fn_8022B7C4(hit) && !fn_80230188(hit) && !fn_80234ECC(hit) &&
            !fn_8023EA28(hit)) {
            if (fn_8022F514(hit) == 1) {
                fn_8022F3DC(hit);
            }
            if (field_0x230 == 3 || field_0x230 == 4) {
                if (fn_8023ED8C(hit) || fn_8023EBB8(hit) || fn_80233DA4(hit)) {
                    field_0x264 = hit;
                    field_0x230 = 1;
                    field_0xb0 = 0;
                } else if (fn_802308E0(hit) == 1) {
                    JGeometry::TVec3f playerPos = *getPlayerPos_8023E724(hit);
                    JGeometry::TVec3f effectPos(playerPos.x, playerPos.y - 12.0f, playerPos.z);
                    if (fn_80233E40(hit)) {
                        fn_80230498(hit, &playerPos, 0x336, 0xFF, -0x80, -0x80, true);
                    } else {
                        fn_80230498(hit, &playerPos, 0xED, 0xFF, -0x80, -0x80, true);
                    }
                    getActorManager_801F856C()->spawnActor_801F9484(
                        'IMUP', field_0x4, &effectPos, (field_0x90 & 3) + 0xA00000 + (hit << 24) + (hit << 26) + 4, -1, -1);
                    fn_801F3668();
                    return;
                }
            }
        }
    }
    if (field_0x17d != 0) {
        field_0x240[4] = 20;
        field_0x270 = field_0x4;
        field_0x27c = field_0x3c;
    }
    if (player < 0 && field_0x240[4] == 0) {
        field_0x27c.x = 0.0f;
        field_0x27c.y = 0.0f;
        if (field_0x230 == 4) {
            field_0x268 = field_0x90 & 3;
            field_0x230 = 3;
        }
        fn_801F2618(NULL, false);
    } else {
        switch (field_0x230) {
        case 0:
            if (dist < 80.0f) {
                field_0x240[1] = 0;
                field_0x230 = 4;
            }
            break;
        case 1:
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
            mPos.z = 0.0f;
            field_0x240[1] = 30;
            field_0x3c.z = 3.0f;
            field_0x230 = 2;
            {
                s32 target = field_0x264;
                if (fn_8022F514(field_0x264) == 1) {
                    target = fn_8022F3DC(field_0x264);
                }
                lbl_8053AED0->fn_8043D874(target, lbl_804AB160[field_0x268], 0);
                if (getPlayerLayer_8023DE58(target) >= 8 && gGameRoot_8053AAF8->field_0x7e == 1) {
                    lbl_8053AED0->fn_8043D67C(0x194 - (field_0x90 & 3), target, 0);
                }
            }
            fn_801F3604();
            // fall through: the original continues with state 2 (0x8031CCB4)
        case 2:
            field_0x3c.z -= 0.5f;
            if (mPos.z <= 0.0f && field_0x3c.z < 0.0f) {
                field_0x3c.z = 0.0f;
                field_0x3c.y = 0.0f;
                field_0x3c.x = 0.0f;
                mPos.z = 0.0f;
            }
            if (fn_8023F264(field_0x264)) {
                mPos.x = getPlayerPos_8023E724(field_0x264)->x;
                mPos.y = getPlayerPos_8023E724(field_0x264)->y - 24.0f;
            } else if (field_0x240[1] == 0) {
                vfunc_0x30(0xFF, true);
                fn_801F3668();
            }
            break;
        // FIXME(shotgun): states 3-10 not written (0x8031CD60-0x8031D8B8).
        }
    }
    if (field_0x3c.squared() > 0.1f) {
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x345, 0, 0, NULL, NULL, true, 1.0f);
    }
    if (field_0x230 != 8 && field_0x230 != 9 && field_0x4 < 8) {
        JGeometry::TVec3f focus(mPos.x, mPos.y - mPos.z, 0.0f);
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&focus);
    }
    field_0x240[0]++;
    // FIXME(shotgun): the frame counter handling after field_0x240++ (0x8031D9B8-0x8031DA8C) is not written.
}

// 0x8031C0B0.
// FIXME(shotgun): 96.0 %, missing/extra
void FleeingForceGem_804AB170::draw() {
    SpriteParams_801F5AE0 params;
    if (field_0x230 == 8 || field_0x230 == 9) {
        return;
    }
    if (field_0x230 == 5 || field_0x230 == 6 || field_0x230 == 7) {
        Mtx mtx;
        PSMTXIdentity(mtx);
        JGeometry::TVec3f rot(0.0f, field_0x23c, 0.0f);
        setRotScaleMtx(mtx, rot, field_0x238);
        params.mMtx = mtx;
        s32 frame = field_0x254 + lbl_804AB130[field_0x268];
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
        }
        return;
    }
    if (field_0x230 != 2) {
        params.field_0x1d = 0;
        params.field_0x30 = 0x11;
        if (mPos.z > 0.0f) {
            params.field_0x1c = 1;
        }
        JGeometry::TVec3f savedPos = mPos;
        mPos += field_0x294;
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x254 + lbl_804AB130[field_0x268], &params, 7, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x254 + lbl_804AB130[field_0x268], 0, 2, -1, 0, 0, 0);
        mPos = savedPos;
    } else {
        if (field_0x264 >= 0 && field_0x264 <= 3) {
            params.field_0x1d = fn_8023CB24(field_0x264) + 1;
            params.field_0x1c = fn_8023CA88(field_0x264);
        }
        params.field_0x2d = 1;
        s32 frame = field_0x254 + lbl_804AB130[field_0x268];
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
        }
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x254 + lbl_804AB130[field_0x268], 0, 2, -1, 0, 0, 0);
    }
}

FleeingForceGem_804AB170::~FleeingForceGem_804AB170() {}
