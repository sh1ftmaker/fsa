#include "game/GameAudio.h"
#include "game/EmitterDelete.h"
#include "game/GameView.h"
#include "game/GameGlobals.h"
#include "game/cls_804A8638.h"
#include "game/tu_802EF76C.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/tu_80182914.h"
#include "game/Camera.h"
#include "game/EmitterDelete.h"
#include "JSystem/JParticle/JPAEmitter.h"

// The two emitters that start the rain; their 0x28 float (field_0x2c0[0], [1]) is not stored
// (JPABaseEmitter in include/JSystem/JParticle/JPAEmitter.h has mEmitterRot (s16) at 0x24..0x29
// and mVolumeType at 0x2A, so no float field at 0x28 is identifiable; the two stfs are
// omitted, a behaviour difference until the real field is known).
// FIXME(shotgun): 88.00 %, missing/extra + reloc-name
void cls_804A8638::init() {
    field_0x234 = NULL;
    field_0x238 = NULL;
    mEmitter = NULL;
    field_0x260 = (field_0x90 >> 22) & 7;
    field_0x264 = (field_0x90 >> 21) & 1;
    field_0x268 = (field_0x90 >> 20) & 1;
    field_0x270 = (field_0x90 >> 19) & 1;
    field_0x274 = (field_0x90 >> 18) & 1;
    field_0x278 = (field_0x90 >> 17) & 1;
    field_0x27c = (field_0x90 >> 16) & 1;
    field_0x280 = (field_0x90 >> 15) & 1;
    field_0x288 = (field_0x90 >> 13) & 3;
    field_0x28c = (field_0x90 >> 12) & 1;
    field_0x290 = (field_0x90 >> 11) & 1;
    field_0x294 = (field_0x90 >> 10) & 1;
    field_0x298 = (field_0x90 >> 9) & 1;
    field_0x29c = (field_0x90 >> 8) & 1;
    field_0x2a0 = (field_0x90 >> 7) & 1;
    field_0x2a4 = (field_0x90 >> 6) & 1;
    field_0x2a8 = (field_0x90 >> 5) & 1;
    field_0x2ac = (field_0x90 >> 4) & 1;
    field_0x2b4 = (field_0x90 >> 26) & 1;
    field_0x2b8 = (field_0x90 >> 25) & 1;
    field_0x2d8 = field_0x90 & 0xF;
    if (fn_802DEBA0() == true && field_0x2d8 != 0xF && field_0x2d8 != 0xD) {
        field_0x2d8 = 0;
    }
    field_0x240 = 0;
    field_0x244 = 0;
    switch (field_0x260) {
    case 1:
        gGameRoot_8053AAF8->fn_80138B7C();
        break;
    case 2:
        gAudioMgr_8053AF80->fn_80458FF0(0x1000004, false, 0);
        break;
    case 3:
        gAudioMgr_8053AF80->fn_80458F9C(0x78);
        break;
    }
    if (field_0x264 != 0) {
        field_0x2d0[0] = fn_8016A3AC()->field_0x1ec;
        field_0x2d0[1] = (u8)(gGameRoot2_8053AB10->field_0x2c->field_0xc * 0x2F / 255);
    } else {
        field_0x2d0[0] = fn_8016A3AC()->field_0x1ec;
    }
    fn_8016A3AC()->field_0x1ec = field_0x2d0[0];
    field_0x2d0[2] = fn_8016A3AC()->field_0x1ed;
    if (field_0x268 != 0) {
        field_0x26c = 0x8F;
    }
    fn_8016A3AC()->field_0x1ed = field_0x2d0[2];
    if (field_0x270 != 0) {
        fn_8016A3AC()->field_0x1c7 = 1;
    }
    if (field_0x274 != 0) {
        fn_8016A3AC()->field_0x1c4 = 1;
    }
    field_0x2d0[3] = fn_8016A3AC()->field_0x1f1;
    if (field_0x280 != 0) {
        field_0x284 = 0xFF;
    }
    fn_8016A3AC()->field_0x1f1 = field_0x2d0[3];
    field_0x2d0[4] = fn_8016A3AC()->field_0x1f0;
    if (field_0x2ac != 0) {
        field_0x2b0 = 0xFF;
    }
    fn_8016A3AC()->field_0x1f0 = field_0x2d0[4];
    field_0x2d0[5] = fn_8016A3AC()->field_0x1ef;
    if (field_0x2b8 != 0) {
        field_0x2bc = 0xFF;
    }
    fn_8016A3AC()->field_0x1ef = field_0x2d0[5];
    if (field_0x288 == 0) {
        if (getGameView_802D6A9C()->field_0x131f2 != 0) {
            if (getGameView_802D6A9C()->field_0x4 == 1 && getGameView_802D6A9C()->field_0x8 == 3) {
                field_0x288 = 4;
            } else {
                field_0x288 = 3;
            }
        }
    }
    if (field_0x288 == 2 && getGameView_802D6A9C()->field_0x4 == 1 && getGameView_802D6A9C()->field_0x8 == 1 &&
        vfunc_0x34()) {
        if (getGameView_802D6A9C()->field_0x131f2 != 0) {
            field_0x288 = 3;
        } else {
            field_0x288 = 0;
        }
    }
    if (field_0x288 != 0) {
        field_0x310 = fn_801B7550();
        if (field_0x288 == 2 && (field_0x90 >> 27) != 0 && !vfunc_0x34()) {
            field_0x288 = 1;
        }
        if (field_0x288 == 1) {
            if (((field_0x90 >> 27) != 0 && vfunc_0x34() == true) || getGameView_802D6A9C()->field_0x131f2 != 0) {
                field_0x288 = 2;
            }
        }
        if (field_0x288 == 1) {
            field_0x2f0 = 0x9F;
            field_0x2f4 = 0xAF;
            field_0x2f8 = 0xFF;
            field_0x300 = field_0x2e0;
            field_0x304 = field_0x2e4;
            field_0x308 = field_0x2e8;
            field_0x2c0[0] = 1.0f;
            field_0x2c0[1] = 1.0f;
            field_0x2c0[2] = 0.17f;
            field_0x2c0[3] = 0.5f;
            field_0x24c = 20;
            field_0x250 = 20;
            getGameView_802D6A9C()->fn_802D6BF8(0);
        } else {
            if (field_0x288 == 2) {
                field_0x2e0 = 0x9F;
                field_0x2e4 = 0xAF;
                field_0x2e8 = 0xFF;
            } else {
                field_0x2f0 = 0x9F;
                field_0x2e0 = 0x9F;
                field_0x2f4 = 0xAF;
                field_0x2e4 = 0xAF;
                field_0x2f8 = 0xFF;
                field_0x2e8 = 0xFF;
            }
            field_0x2c0[0] = 10.0f;
            field_0x2c0[1] = 30.0f;
            field_0x2c0[2] = 0.17f;
            field_0x2c0[3] = 0.5f;
            field_0x24c = fn_801CD664(100);
            field_0x250 = 0;
            field_0x234 = spawnEmitter_8013CC50(field_0x4, &field_0x254, 0x3A1, 1, 0, NULL, NULL, true, 1.0f);
            field_0x238 = spawnEmitter_8013CC50(field_0x4, &field_0x254, 0x3A2, 1, 0, NULL, NULL, true, 1.0f);
            getGameView_802D6A9C()->fn_802D6BF8(1);
            vfunc_0x60();
        }
    }
    if (field_0x290 != 0) {
        mEmitter = spawnEmitter_8013CC50(field_0x4, &field_0x254, 0x449, 1, 0, NULL, NULL, true, 1.0f);
    } else if (field_0x294 != 0) {
        mEmitter = spawnEmitter_8013CC50(field_0x4, &field_0x254, 0x58, 1, 0, NULL, NULL, true, 1.0f);
    } else if (mEmitter != NULL) {
        mEmitter->becomeInvalidEmitter();
        mEmitter = NULL;
    }
    JUtility::TColor fog = fn_8018330C();
    field_0x2dc = fog.toUInt32();
    field_0x2e0 = fog.r;
    field_0x2e4 = fog.g;
    field_0x2e8 = fog.b;
    field_0x2ec = fog.a;
    field_0x300 = 0xFF;
    field_0x304 = 0xFF;
    field_0x308 = 0xFF;
    field_0x30c = 0;
    field_0x310 = fn_801B7550();
    field_0x254.x = -gGameRoot2_8053AB10->field_0x24->field_0x4[0][3];
    field_0x254.y = -gGameRoot2_8053AB10->field_0x24->field_0x4[1][3];
    field_0x254.z = 0.0f;
    switch (field_0x2d8) {
    case 0:
        field_0x240 = 9;
        break;
    case 1:
        vfunc_0x54(0xFF, 0x80, 0xFF, 0, 10, 0);
        break;
    case 2:
        vfunc_0x54(0xFF, 0xFF, 0xFF, 0, 0xFF, 0);
        field_0x240 = 0;
        break;
    case 3:
        getGameView_802D6A9C()->fn_802D7440(0);
        vfunc_0x54(0xFF, 0xFF, 0xFF, 0, 0xFF, 0);
        field_0x240 = 0;
        break;
    case 4:
        getGameView_802D6A9C()->fn_802D7440(0);
        if (!getGameView_802D6A9C()->fn_802D75BC(field_0x16c)) {
            vfunc_0x54(0xFF, 0x80, 0xFF, 0, 10, 0);
            field_0x240 = 0xB;
        } else {
            vfunc_0x54(0xFF, 0xFF, 0xFF, 0, 0xFF, 0);
            field_0x240 = 8;
        }
        break;
    case 5:
        vfunc_0x54(0xFF, 0xFF, 0xFF, 0, 0x80, 0);
        break;
    case 6:
        vfunc_0x54(0xFF, 0x80, 0xFF, 0, 10, 0);
        field_0x240 = 0xB;
        break;
    case 7:
        vfunc_0x54(0xFF, 0xFF, 0xFF, 0, 10, 9);
        field_0x248 = 100;
        field_0x334 = 0;
        field_0x338 = 0;
        field_0x314 = 10;
        break;
    case 8:
        vfunc_0x54(0x28, 0x50, 0xFF, 0, 0xFF, 0);
        break;
    case 9:
        vfunc_0x54(0x28, 0x50, 0xFF, 0, 0xFF, 9);
        break;
    case 10:
        vfunc_0x54(0xFF, 0xB4, 0x78, 0, 0x8C, 9);
        break;
    case 11:
        vfunc_0x54(0xA4, 0xC8, 0xDC, 0, 0xFF, 9);
        field_0x240 = 0;
        break;
    case 12:
        vfunc_0x54(0xFF, 0xFF, 0xFF, 0, 0xFF, 0);
        field_0x240 = 0;
        break;
    case 13:
        vfunc_0x54(0xBE, 0xD2, 0xC8, 0, 0xFF, 9);
        break;
    case 14:
        vfunc_0x54(0x96, 0xC8, 0xFF, 20, 0xB4, 9);
        break;
    case 15:
        vfunc_0x54(0xFF, 0xFF, 0xD2, 24, 0xFF, 9);
        break;
    }
    mSoundObj = new ActorSoundObj_804578F4(NULL, NULL, 8);
    JGeometry::TVec3f saved = mPos;
    update();
    mPos = saved;
    field_0x11f = 1;
}

// 0x802B5284.
void cls_804A8638::vfunc_0x10() {
    if (field_0x288 != 4) {
        return;
    }
    getGameView_802D6A9C()->fn_802D6BF8(0);
    field_0x288 = 0;
    field_0x2e0 = field_0x300;
    field_0x2e4 = field_0x304;
    field_0x2e8 = field_0x308;
    deleteEmitter(&field_0x234);
    deleteEmitter(&field_0x238);
    u32 color = (field_0x2e0 << 24) | (field_0x2e4 << 16) | (field_0x2e8 << 8) | field_0x2ec;
    fn_8013A884(&color);
}

// FIXME(shotgun): 87.08 %, missing/extra
cls_804A8638::~cls_804A8638() {
    fn_8016A3AC()->field_0x1ee = 0;
    if (field_0x234 != NULL) {
        releaseEmitter_801EE434(field_0x234);
    }
    if (field_0x238 != NULL) {
        releaseEmitter_801EE434(field_0x238);
    }
    if (mEmitter != NULL) {
        releaseEmitter_801EE434(mEmitter);
    }
    delete mSoundObj;
}

// 0x802B3AB4.
void cls_804A8638::draw() {}

// 0x802B3A48.
// FIXME(shotgun): 84.07 %, missing/extra
void cls_804A8638::fn_802B3A48() {
    fn_8016A3AC()->field_0x1ee = 0;
    if (field_0x234 != NULL) {
        releaseEmitter_801EE434(field_0x234);
    }
    if (field_0x238 != NULL) {
        releaseEmitter_801EE434(field_0x238);
    }
    if (mEmitter != NULL) {
        releaseEmitter_801EE434(mEmitter);
    }
    mAlive = false;
}

// 0x802B3A3C.
void cls_804A8638::fn_802B3A3C() {
    field_0x2b8 = 0;
}

// 0x802B39E8.
void cls_804A8638::fn_802B39E8() {
    field_0x290 = 0;
    deleteEmitter(&mEmitter);
}

// 0x802B394C.
void cls_804A8638::vfunc_0x54(s32 a, s32 b, s32 c, s32 d, s32 e, s32 state) {
    field_0x240 = state;
    if (state == 9) {
        field_0x2e0 = a;
        field_0x2e4 = b;
        field_0x2e8 = c;
        field_0x2ec = d;
        field_0x310 = e;
        return;
    }
    field_0x2f0 = a;
    field_0x2f4 = b;
    field_0x2f8 = c;
    field_0x2ec = d;
    field_0x314 = e;
    field_0x318 = (field_0x2f0 - field_0x2e0) / 4;
    field_0x31c = (field_0x2f4 - field_0x2e4) / 4;
    field_0x320 = (field_0x2f8 - field_0x2e8) / 4;
    if (field_0x310 >= field_0x314) {
        return;
    }
    field_0x240 = 0xc;
}

// 0x802B3838.
void cls_804A8638::vfunc_0x58() {
    field_0x310 -= 4;
    if (field_0x2e0 > field_0x2f0) {
        field_0x2e0 += field_0x318;
    } else {
        field_0x2e0 = field_0x2f0;
    }
    if (field_0x2e4 > field_0x2f4) {
        field_0x2e4 += field_0x31c;
    } else {
        field_0x2e4 = field_0x2f4;
    }
    if (field_0x2e8 > field_0x2f8) {
        field_0x2e8 += field_0x320;
    } else {
        field_0x2e8 = field_0x2f8;
    }
    if (field_0x310 <= field_0x314) {
        field_0x310 = field_0x314;
        field_0x2e0 = field_0x2f0;
        field_0x2e4 = field_0x2f4;
        field_0x2e8 = field_0x2f8;
        field_0x240 = 9;
    }
    fn_80128C58((u8)field_0x310);
    u32 color = (field_0x2e0 << 24) | (field_0x2e4 << 16) | (field_0x2e8 << 8) | field_0x2ec;
    fn_8013A884(&color);
}

void cls_804A8638::vfunc_0x5C() {
    field_0x310 += 4;
    if (field_0x2e0 > field_0x2f0) {
        field_0x2e0 += field_0x318;
    } else {
        field_0x2e0 = field_0x2f0;
    }
    if (field_0x2e4 > field_0x2f4) {
        field_0x2e4 += field_0x31c;
    } else {
        field_0x2e4 = field_0x2f4;
    }
    if (field_0x2e8 > field_0x2f8) {
        field_0x2e8 += field_0x320;
    } else {
        field_0x2e8 = field_0x2f8;
    }
    if (field_0x310 >= field_0x314) {
        field_0x310 = field_0x314;
        field_0x2e0 = field_0x2f0;
        field_0x2e4 = field_0x2f4;
        field_0x2e8 = field_0x2f8;
        field_0x240 = 9;
    }
    fn_80128C58((u8)field_0x310);
    u32 color = (field_0x2e0 << 24) | (field_0x2e4 << 16) | (field_0x2e8 << 8) | field_0x2ec;
    fn_8013A884(&color);
}

// Starts BGM 0x100000E unless one of these is already the current music (id - 0x1000000).
// 0x802B3674.
// FIXME(shotgun): 91.75 %, missing/extra
void cls_804A8638::vfunc_0x60() {
    u32 current = gAudioMgr_8053AF80->field_0x60;
    if (!fn_802DEBF8() && current != 0) {
        u32 id = current - 0x1000000;
        if (id != 0x21 && id != 9 && id != 0x18 && id != 0x19 && id != 1 && id != 0xD && id != 7 && id != 0x2E &&
            id != 0x29 && id != 0x24 && id != 0xE) {
            gAudioMgr_8053AF80->fn_80458FF0(0x100000E, false, 0);
        }
    }
}
