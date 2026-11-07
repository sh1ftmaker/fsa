#include "game/GameAudio.h"
#include "game/EmitterDelete.h"
#include "game/GameView.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/Player.h"
#include "game/cls_804A8638.h"
#include "game/tu_802EF76C.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/tu_80182914.h"
#include "game/Camera.h"
#include "game/EmitterDelete.h"
#include "JSystem/JParticle/JPAEmitter.h"

u8 lbl_8053A1A0[8] = {10, 60, 80, 100, 210, 0, 0, 0};

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

// 0x802B3AB8.
// GameRootSub24_8016A3AC's 0x1E0 and 0x1E8 floats are not declared in GameGlobals.h (only 0x1E4 is),
// and JPABaseEmitter's 0x28 float / 0xF4 status word differ from include/JSystem/JParticle/JPAEmitter.h,
// so those stores are omitted or call JPA's methods.
// FIXME(shotgun): 94.06 %, missing/extra
void cls_804A8638::update() {
    bool paused = false;
    if ((gGameRoot_8053AAF8->field_0x7f != 0 &&
         (fn_80236B50(0) && fn_80236B50(1) && fn_80236B50(2) && fn_80236B50(3) ||
          (gGameRoot_8053AAF8->field_0x4c == 0xB || gGameRoot_8053AAF8->field_0x88 == 2))) ||
        lbl_80529DEC.field_0x240 != 0) {
        paused = true;
    }
    if (field_0x264 == 0) {
        if (field_0x2d0[0] != 0) {
            field_0x2d0[0]--;
        }
    } else if (field_0x2d0[0] < field_0x2d0[1]) {
        field_0x2d0[0]++;
    }
    fn_8016A3AC()->field_0x1ec = field_0x2d0[0];
    if (field_0x268 == 0) {
        if (field_0x2d0[2] != 0) {
            field_0x2d0[2]--;
        }
    } else if (field_0x2d0[2] < field_0x26c) {
        field_0x2d0[2]++;
    }
    fn_8016A3AC()->field_0x1ed = field_0x2d0[2];
    if (field_0x270 != 0) {
        fn_8016A3AC()->field_0x1c7 = 1;
    }
    if (field_0x274 != 0) {
        fn_8016A3AC()->field_0x1ee = 0xFF;
        if (gGameRoot_8053AAF8->field_0x4c != 0xB) {
            mSoundObj->startSound(0x3001F);
        }
        fn_8016A3AC()->field_0x1c4 = 1;
    }
    if (field_0x278 != 0) {
        fn_8016A3AC()->field_0x1c8 = 1;
    }
    if (field_0x27c != 0) {
        fn_8016A3AC()->field_0x1c9 = 1;
    }
    if (field_0x280 == 0) {
        if (field_0x2d0[3] != 0) {
            field_0x2d0[3]--;
        }
    } else if (field_0x2d0[3] < field_0x284) {
        field_0x2d0[3]++;
    }
    fn_8016A3AC()->field_0x1f1 = field_0x2d0[3];
    if (field_0x28c != 0) {
        f32 y = mPos.y - 16.0f;
        GameRootSub24_8016A3AC* sub24 = fn_8016A3AC();
        sub24->field_0x1cb[0] = 1;
        sub24->field_0x1e4 = y;
    }
    if (field_0x298 != 0) {
        fn_8016A3AC()->field_0x1cb[1] = 1;
    }
    if (field_0x29c != 0) {
        fn_8016A3AC()->field_0x1cb[3] = 1;
    }
    if (field_0x2a0 != 0) {
        GameRootSub24_8016A3AC* sub24 = fn_8016A3AC();
        sub24->field_0x1cf = 1;
        sub24->field_0x1d0 = 1;
    }
    if (field_0x2a4 != 0) {
        fn_8016A3AC()->field_0x1cb[2] = 1;
    }
    if (field_0x2a8 != 0) {
        fn_8016A3AC()->field_0x1c5 = 1;
    }
    if (field_0x2ac == 0) {
        if (field_0x2d0[4] != 0) {
            field_0x2d0[4]--;
        }
    } else if (field_0x2d0[4] < field_0x2b0) {
        field_0x2d0[4]++;
    }
    fn_8016A3AC()->field_0x1f0 = field_0x2d0[4];
    if (field_0x2b4 != 0) {
        fn_8016A3AC()->field_0x1c6 = 1;
    }
    if (field_0x2b8 == 0) {
        if (field_0x2d0[5] != 0) {
            field_0x2d0[5]--;
        }
    } else if (field_0x2d0[5] < field_0x2bc) {
        field_0x2d0[5]++;
    }
    fn_8016A3AC()->field_0x1ef = field_0x2d0[5];
    field_0x254.x = -gGameRoot2_8053AB10->field_0x24->field_0x4[0][3];
    field_0x254.y = -gGameRoot2_8053AB10->field_0x24->field_0x4[1][3];
    field_0x254.z = 0.0f;
    switch (field_0x2d8) {
    case 0: {
        fn_80128C58((u8)field_0x310);
        u32 color = (field_0x2e0 << 24) | (field_0x2e4 << 16) | (field_0x2e8 << 8) | field_0x2ec;
        fn_8013A884(&color);
        break;
    }
    case 1:
        switch (field_0x240) {
        case 0:
            vfunc_0x58();
            break;
        case 0xC:
            vfunc_0x5C();
            break;
        case 9: {
            fn_80128C58((u8)field_0x310);
            u32 color = (field_0x2e0 << 24) | (field_0x2e4 << 16) | (field_0x2e8 << 8) | field_0x2ec;
            fn_8013A884(&color);
            break;
        }
        }
        break;
    case 2:
        switch (field_0x240) {
        case 0:
            gGameRoot_8053AAF8->fn_80138B7C();
            field_0x240 = 0xC;
            break;
        case 0xC:
            vfunc_0x5C();
            break;
        case 9: {
            fn_80128C58((u8)field_0x310);
            u32 color = (field_0x2e0 << 24) | (field_0x2e4 << 16) | (field_0x2e8 << 8) | field_0x2ec;
            fn_8013A884(&color);
            break;
        }
        }
        break;
    case 3:
        mSoundObj->startSound(0x3001E);
        switch (field_0x240) {
        case 0:
            if (getActorManager_801F856C()->field_0x305c == 1) {
                field_0x248 = 20;
                field_0x240 = 1;
            }
            break;
        case 1:
            field_0x248--;
            if (field_0x248 == 0) {
                field_0x240 = 0xC;
            }
            break;
        case 0xC:
            vfunc_0x5C();
            break;
        case 9: {
            fn_80128C58((u8)field_0x310);
            u32 color = (field_0x2e0 << 24) | (field_0x2e4 << 16) | (field_0x2e8 << 8) | field_0x2ec;
            fn_8013A884(&color);
            break;
        }
        }
        break;
    case 4: {
        mSoundObj->startSound(0x3001E);
        switch (field_0x240) {
        case 0xB:
            vfunc_0x58();
            if (field_0x240 == 9) {
                field_0x334 = 0;
                field_0x338 = 0;
                field_0x240 = 0xC;
            }
            break;
        case 0xC:
            if (getActorManager_801F856C()->field_0x305c == 1) {
                field_0x248 = 20;
                field_0x240 = 1;
            }
            break;
        case 1:
            field_0x248--;
            if (field_0x248 == 0) {
                vfunc_0x54(0xFF, 0xFF, 0xFF, 0, 0xFF, 0);
                field_0x240 = 0;
            }
            break;
        case 0:
            vfunc_0x5C();
            if (field_0x240 == 9) {
                field_0x240 = 2;
                vfunc_0x30(0xFF, true);
            }
            break;
        case 2:
            if (!vfunc_0x34()) {
                vfunc_0x54(0xFF, 0x80, 0xFF, 0, 10, 0);
                field_0x240 = 6;
            }
            break;
        case 6:
            vfunc_0x58();
            field_0x240 = 3;
            gAudioMgr_8053AF80->fn_80458FF0(0x1000004, false, 0);
            break;
        case 3:
            vfunc_0x58();
            if (field_0x240 == 9) {
                field_0x240 = 4;
            }
            break;
        case 4:
            if (getActorManager_801F856C()->field_0x305c == 0) {
                field_0x240 = 5;
            }
            break;
        case 5:
            if (getActorManager_801F856C()->field_0x305c == 1) {
                field_0x248 = 30;
                field_0x240 = 7;
            }
            break;
        case 7:
            field_0x248--;
            if (field_0x248 == 0) {
                vfunc_0x54(0xFF, 0xFF, 0xFF, 0, 0xFF, 0);
                field_0x240 = 8;
                fn_801F34DC();
            }
            break;
        case 8:
            vfunc_0x5C();
            break;
        }
        fn_80128C58((u8)field_0x310);
        u32 color = (field_0x2e0 << 24) | (field_0x2e4 << 16) | (field_0x2e8 << 8) | field_0x2ec;
        fn_8013A884(&color);
        break;
    }
    case 5:
        switch (field_0x240) {
        case 0:
            vfunc_0x58();
            break;
        case 0xC:
            vfunc_0x5C();
            break;
        case 9: {
            fn_80128C58((u8)field_0x310);
            u32 color = (field_0x2e0 << 24) | (field_0x2e4 << 16) | (field_0x2e8 << 8) | field_0x2ec;
            fn_8013A884(&color);
            break;
        }
        }
        break;
    case 6:
        switch (field_0x240) {
        case 0xB: {
            bool found = false;
            s32 end = getActorManager_801F856C()->mActorEnd;
            for (s32 i = 0; i < end; i++) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
                if (actor != NULL && actor->mActorId == 'BPH2') {
                    u32 p = actor->field_0x90;
                    found = true;
                    field_0x324[0] = p >> 27;
                    field_0x324[1] = (p >> 22) & 0x1F;
                    field_0x324[2] = (p >> 17) & 0x1F;
                    field_0x324[3] = (p >> 12) & 0x1F;
                    break;
                }
            }
            if (found) {
                field_0x240 = 0;
            }
            break;
        }
        case 0:
            vfunc_0x58();
            if (field_0x240 == 9) {
                field_0x248 = 100;
                field_0x334 = 0;
                field_0x338 = 0;
                field_0x240 = 0xC;
            }
            break;
        case 0xC: {
            field_0x338 = 0;
            for (s32 i = 0; i < 4; i++) {
                if (getGameView_802D6A9C()->fn_802D800C(field_0x324[i])) {
                    field_0x338++;
                }
            }
            field_0x334 = field_0x338;
            field_0x314 = lbl_8053A1A0[field_0x334];
            if (field_0x310 < field_0x314) {
                field_0x310 += 5;
                if (field_0x310 >= field_0x314) {
                    field_0x310 = field_0x314;
                }
            } else if (field_0x310 > field_0x314) {
                field_0x310 -= 5;
                if (field_0x310 <= field_0x314) {
                    field_0x310 = field_0x314;
                }
            }
            fn_80128C58((u8)field_0x310);
            u32 color = (field_0x2e0 << 24) | (field_0x2e4 << 16) | (field_0x2e8 << 8) | field_0x2ec;
            fn_8013A884(&color);
            break;
        }
        }
        break;
    case 7:
    case 9:
    case 10:
    case 13:
    case 14:
    case 15: {
        fn_80128C58((u8)field_0x310);
        u32 color = (field_0x2e0 << 24) | (field_0x2e4 << 16) | (field_0x2e8 << 8) | field_0x2ec;
        fn_8013A884(&color);
        break;
    }
    case 8:
        break;
    case 11: {
        switch (field_0x240) {
        case 0: {
            if ((field_0x90 >> 27) == 0 || vfunc_0x34()) {
                vfunc_0x54(0xFF, 0xFF, 0xFF, 0, 0xFF, 0);
                field_0x240 = 1;
            }
            fn_80128C58((u8)field_0x310);
            u32 color = (field_0x2e0 << 24) | (field_0x2e4 << 16) | (field_0x2e8 << 8) | field_0x2ec;
            fn_8013A884(&color);
            break;
        }
        case 1:
            vfunc_0x5C();
            if (field_0x240 == 9) {
                field_0x2b8 = 0;
                field_0x240 = 9;
            }
            break;
        case 9:
        default: {
            fn_80128C58((u8)field_0x310);
            u32 color = (field_0x2e0 << 24) | (field_0x2e4 << 16) | (field_0x2e8 << 8) | field_0x2ec;
            fn_8013A884(&color);
            break;
        }
        }
        break;
    }
    case 12: {
        switch (field_0x240) {
        case 0:
            if ((field_0x90 >> 27) == 0 || vfunc_0x34()) {
                field_0x240 = 1;
            }
            break;
        case 1:
            vfunc_0x5C();
            break;
        case 9:
        default: {
            fn_80128C58((u8)field_0x310);
            u32 color = (field_0x2e0 << 24) | (field_0x2e4 << 16) | (field_0x2e8 << 8) | field_0x2ec;
            fn_8013A884(&color);
            break;
        }
        }
        break;
    }
    }

    if (field_0x288 != 0) {
        mPos = *getPlayerPos_8023E724(0);
        if (field_0x288 != 1 || (field_0x90 >> 27) == 0 || vfunc_0x34() == true) {
            if (!paused) {
                field_0x24c--;
                if (field_0x24c == 0) {
                    if (gGameRoot_8053AAF8->field_0x4c != 0xB) {
                        mSoundObj->startSound(0x30031);
                    }
                    spawnEmitter_8013CC50(field_0x4, &field_0x254, 0x488, 1, 0, NULL, NULL, true, 1.0f);
                    field_0x24c = fn_801CD664(0x97) + 50;
                    if (fn_802DEBF8()) {
                        field_0x24c += fn_801CD664(0xDD) + 80;
                    }
                }
            }
        }
    }

    switch (field_0x288) {
    case 1:
        if ((field_0x90 >> 27) != 0 && vfunc_0x34() != true) {
            break;
        }
        if (field_0x250 == 0) {
            field_0x250 = 20;
            if (!paused) {
                if (field_0x234 == NULL) {
                    field_0x234 = spawnEmitter_8013CC50(field_0x4, &field_0x254, 0x3A1, 1, 0, NULL, NULL, true, 1.0f);
                    field_0x238 = spawnEmitter_8013CC50(field_0x4, &field_0x254, 0x3A2, 1, 0, NULL, NULL, true, 1.0f);
                    getGameView_802D6A9C()->fn_802D6BF8(1);
                    vfunc_0x60();
                }
                if (field_0x2c0[0] < 10.0f) {
                    field_0x2c0[0] += field_0x2c0[2];
                    if (field_0x2c0[0] > 10.0f) {
                        field_0x2c0[0] = 10.0f;
                    }
                }
                if (field_0x2c0[1] < 30.0f) {
                    field_0x2c0[1] += field_0x2c0[3];
                    if (field_0x2c0[1] > 30.0f) {
                        field_0x2c0[1] = 30.0f;
                    }
                }
                setEmitterPos_8013CB44(field_0x234, &field_0x254, 1, 1, true);
                setEmitterPos_8013CB44(field_0x238, &field_0x254, 1, 1, true);
            }
        } else if (field_0x250 != 0) {
            field_0x250--;
        }
        switch (field_0x244) {
        case 0: {
            getGameView_802D6A9C()->fn_802D6BF8(1);
            s32 changed = 0;
            if (field_0x2e0 != field_0x2f0) {
                if (field_0x2e0 > field_0x2f0) {
                    field_0x2e0 -= 4;
                    if (field_0x2e0 <= field_0x2f0) {
                        field_0x2e0 = field_0x2f0;
                    }
                } else {
                    field_0x2e0 += 4;
                    if (field_0x2e0 >= field_0x2f0) {
                        field_0x2e0 = field_0x2f0;
                    }
                }
                changed = 1;
            }
            if (field_0x2e4 != field_0x2f4) {
                if (field_0x2e4 > field_0x2f4) {
                    field_0x2e4 -= 4;
                    if (field_0x2e4 <= field_0x2f4) {
                        field_0x2e4 = field_0x2f4;
                    }
                } else {
                    field_0x2e4 += 4;
                    if (field_0x2e4 >= field_0x2f4) {
                        field_0x2e4 = field_0x2f4;
                    }
                }
                changed = 1;
            }
            if (field_0x2e8 != field_0x2f8) {
                if (field_0x2e8 > field_0x2f8) {
                    field_0x2e8 -= 4;
                    if (field_0x2e8 <= field_0x2f8) {
                        field_0x2e8 = field_0x2f8;
                    }
                } else {
                    field_0x2e8 += 4;
                    if (field_0x2e8 >= field_0x2f8) {
                        field_0x2e8 = field_0x2f8;
                    }
                }
                changed = 1;
            }
            if (changed == 0) {
                field_0x244 = 1;
            }
            u32 color = (field_0x2e0 << 24) | (field_0x2e4 << 16) | (field_0x2e8 << 8) | field_0x2ec;
            fn_8013A884(&color);
            break;
        }
        case 1:
            if (field_0x2c0[0] == 10.0f && field_0x2c0[1] == 30.0f) {
                field_0x288 = 2;
            }
            break;
        }
        break;
    case 3:
        if (field_0x250 == 0 && ((field_0x90 >> 27) == 0 || !vfunc_0x34())) {
            field_0x250 = 20;
            if (field_0x2c0[0] > 1.0f) {
                field_0x2c0[0] -= field_0x2c0[2];
                if (field_0x2c0[0] < 1.0f) {
                    field_0x2c0[0] = 1.0f;
                }
            }
            if (field_0x2c0[1] > 1.0f) {
                field_0x2c0[1] -= field_0x2c0[3];
                if (field_0x2c0[1] < 1.0f) {
                    field_0x2c0[1] = 1.0f;
                }
            }
            setEmitterPos_8013CB44(field_0x234, &field_0x254, 1, 1, true);
            setEmitterPos_8013CB44(field_0x238, &field_0x254, 1, 1, true);
        } else if (field_0x250 != 0) {
            field_0x250--;
        }
        if (field_0x244 == 0) {
            field_0x24c = 100;
            s32 changed = 0;
            if (field_0x2e0 != field_0x300) {
                if (field_0x2e0 > field_0x300) {
                    field_0x2e0 -= 4;
                    if (field_0x2e0 <= field_0x300) {
                        field_0x2e0 = field_0x300;
                    }
                } else {
                    field_0x2e0 += 4;
                    if (field_0x2e0 >= field_0x300) {
                        field_0x2e0 = field_0x300;
                    }
                }
                changed = 1;
            }
            if (field_0x2e4 != field_0x304) {
                if (field_0x2e4 > field_0x304) {
                    field_0x2e4 -= 4;
                    if (field_0x2e4 <= field_0x304) {
                        field_0x2e4 = field_0x304;
                    }
                } else {
                    field_0x2e4 += 4;
                    if (field_0x2e4 >= field_0x304) {
                        field_0x2e4 = field_0x304;
                    }
                }
                changed = 1;
            }
            if (field_0x2e8 != field_0x308) {
                if (field_0x2e8 > field_0x308) {
                    field_0x2e8 -= 4;
                    if (field_0x2e8 <= field_0x308) {
                        field_0x2e8 = field_0x308;
                    }
                } else {
                    field_0x2e8 += 4;
                    if (field_0x2e8 >= field_0x308) {
                        field_0x2e8 = field_0x308;
                    }
                }
                changed = 1;
            }
            if (changed == 0) {
                field_0x288 = 0;
                getGameView_802D6A9C()->fn_802D6BF8(0);
                if (field_0x234 != NULL) {
                    field_0x234->becomeInvalidEmitter();
                    field_0x234 = NULL;
                }
                if (field_0x238 != NULL) {
                    field_0x238->becomeInvalidEmitter();
                    field_0x238 = NULL;
                }
                gGameRoot_8053AAF8->fn_80138B7C();
            }
            u32 color = (field_0x2e0 << 24) | (field_0x2e4 << 16) | (field_0x2e8 << 8) | field_0x2ec;
            fn_8013A884(&color);
        }
        break;
    case 2: {
        if ((getGameView_802D6A9C()->field_0x4 != 4 || getGameView_802D6A9C()->field_0x8 != 1) && vfunc_0x34()) {
            field_0x288 = 3;
            field_0x244 = 0;
        }
        field_0x2e0 = 0x9F;
        field_0x2e4 = 0xAF;
        field_0x2e8 = 0xFF;
        getGameView_802D6A9C()->fn_802D6BF8(1);
        if (gGameRoot_8053AAF8->field_0x4c != 0xB) {
            mSoundObj->startSound(0x30030);
        }
        setEmitterPos_8013CB44(field_0x234, &field_0x254, 1, 1, true);
        setEmitterPos_8013CB44(field_0x238, &field_0x254, 1, 1, true);
        u32 color = (field_0x2e0 << 24) | (field_0x2e4 << 16) | (field_0x2e8 << 8) | field_0x2ec;
        fn_8013A884(&color);
        vfunc_0x60();
        break;
    }
    }

    if (field_0x290 != 0 || field_0x294 != 0) {
        if (field_0x294 != 0 && gGameRoot_8053AAF8->field_0x4c != 0xB) {
            mSoundObj->startSound(0x30032);
        }
        setEmitterPos_8013CB44(mEmitter, &field_0x254, 1, 1, true);
    }

    if (field_0x310 == 0xFF) {
        getGameView_802D6A9C()->fn_802D6BE0(0);
    } else {
        getGameView_802D6A9C()->fn_802D6BE0(1);
    }
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
