#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/tu_8044998C.h"
#include "game/act/act_force_gem_winged.h"
#include "game/act/act_zora_fish_herder.h"
#include "JSystem/JMath/JMath.h"

// FIXME(shotgun): 99.74 %, scheduling + reloc-name
void ZoraFishHerding_804A2E30::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    mBox2.set(-10.0f, -10.0f, 10.0f, 10.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x254 = 2;
    field_0x258 = -1;
    field_0x25c = 0;
    field_0x26c = -1;
    field_0x270 = -1;
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x264 = -1;
    field_0x268 = -1;
    field_0x250 = 0;
    if (field_0x250 != 0) {
        field_0x250 = 0;
        field_0x234 = 0;
        field_0x240 = 0;
    }
    mKind = field_0x90 & 0xF;
    if (vfunc_0x34()) {
        fn_801F3668();
        return;
    }
    switch (mKind) {
    case 1:
        if (field_0x250 != 1) {
            field_0x250 = 1;
            field_0x234 = 0;
            field_0x240 = 0;
        }
        field_0x254 = field_0x240 + 12;
        // fall through
    case 0:
        if (vfunc_0x34()) {
            mState = 10;
        } else {
            mState = 0;
        }
        break;
    case 2:
        mState = 13;
        break;
    }
}

// Advances the fish animation: picks the sprite frame (field_0x254) of the
// current mode and steps to the next mode when its loop ends.
// FIXME(shotgun): 87.88 %, missing/extra (the original has a dead cmpwi 4 / beqlr after the mode 5 test)
void ZoraFishHerding_804A2E30::fn_801A7C48() {
    if (field_0x234 != 0) {
        return;
    }
    switch (field_0x250) {
    case 0:
    case 5:
        field_0x254 = field_0x240 + 2;
        field_0x234 = 8;
        field_0x240 = (field_0x240 + 1) & 7;
        if (field_0x240 >= 4 && field_0x250 == 5) {
            field_0x250 = 4;
            field_0x234 = 0;
            field_0x240 = 0;
        }
        break;
    case 1:
        field_0x254 = field_0x240 + 12;
        field_0x234 = 8;
        field_0x240 = (field_0x240 + 1) & 3;
        break;
    case 2:
        field_0x254 = 13 - field_0x240;
        field_0x234 = 8;
        field_0x240 = (field_0x240 + 1) & 3;
        if (field_0x240 == 0) {
            field_0x250 = 0;
        }
        break;
    case 3:
        field_0x254 = field_0x240 + 10;
        field_0x234 = 8;
        field_0x240 = (field_0x240 + 1) & 3;
        if (field_0x240 == 0) {
            field_0x250 = 1;
        }
        break;
    case 4:
        field_0x254 = field_0x240;
        field_0x234 = 15;
        field_0x240 = (field_0x240 + 1) & 1;
        break;
    }
}

ZoraFishHerding_804A2E30::~ZoraFishHerding_804A2E30() {
    if (mKind == 0 || mKind == 1) {
        if (mState == 4 || mState == 5) {
            fn_8022B1C8();
            if (gGameRoot_8053AAF8->field_0x4c != 9) {
                gGameRoot_8053AAF8->fn_80138B7C();
                gAudioMgr_8053AF80->fn_80458508(0);
            }
        }
    }
}

// FIXME(shotgun): 81.69 %, missing/extra + reloc-name
void ZoraFishHerding_804A2E30::fn_801A68DC() {
    JGeometry::TVec3f msgPos = mPos;
    s32 nearPlayer = -1;
    f32 nearDist;
    if (mKind == 1) {
        fn_801EEC98(mSerial, NULL, &nearPlayer, &nearDist, NULL, 0, 0);
    }
    msgPos.y -= 50.0f;
    JGeometry::TVec3f dropPos = mPos;
    JGeometry::TVec3f camPos = mPos;
    JGeometry::TBox2f box = mBox;
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    if (field_0x250 != 1) {
        fn_80239914(&box, field_0x4, mSerial);
    }
    box = mBox2;
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;

    switch (mState) {
    case 0:
        if (mKind == 1) {
            if (nearPlayer < 0 || nearDist > 64.0f) {
                if (field_0x250 != 3 && field_0x250 != 1) {
                    field_0x250 = 3;
                    field_0x234 = 0;
                    field_0x240 = 0;
                }
                break;
            }
            if (nearDist > 48.0f) {
                break;
            }
            if (field_0x250 != 2 && field_0x250 != 0 && field_0x250 != 4) {
                field_0x250 = 2;
                field_0x234 = 0;
                field_0x240 = 0;
            }
        }
        if (field_0x250 != 0) {
            break;
        }
        field_0x264 = gGameRoot_8053AAF8->fn_80138A30();
        if (field_0x264 > 1) {
            for (s32 i = 1; i < field_0x264; i++) {
                s32 layer = getPlayerLayer_8023DE58(i);
                if (getPlayerLayer_8023DE58(0) != layer) {
                    return;
                }
            }
        }
        if (mKind == 1) {
            if (nearPlayer < 0 || nearDist > 64.0f) {
                if (field_0x250 != 3 && field_0x250 != 1) {
                    field_0x250 = 3;
                    field_0x234 = 0;
                    field_0x240 = 0;
                }
                break;
            }
            if (nearDist > 48.0f) {
                break;
            }
            if (field_0x250 != 2 && field_0x250 != 0 && field_0x250 != 4) {
                field_0x250 = 2;
                field_0x234 = 0;
                field_0x240 = 0;
            }
        }
        if (field_0x250 != 0) {
            break;
        }
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        field_0x268 = fn_8022FD8C(mSerial);
        if (field_0x268 >= 0 && fn_802300EC(field_0x268)) {
            u32 msg = mKind == 0 ? 0x41A : 0x422;
            if (fn_8038AD24(field_0x268, msg, mSerial, field_0x4, msgPos, 0x100)) {
                JGeometry::TVec3f delta;
                f32 dist;
                fn_801EE8F4(&delta, &dist, field_0x268 + 1);
                if (dist < 100.0f) {
                    u16 angle = (s32)(JMath::atanTable_.atan2_(-delta.x, -delta.y) * 10430.378f);
                    if (angle < 0x2000 || angle >= 0xE000) {
                        field_0x98 = 3;
                    } else if (angle >= 0x2000 && angle < 0x6000) {
                        field_0x98 = 1;
                    } else if (angle >= 0x6000 && angle <= 0xA000) {
                        field_0x98 = 2;
                    } else {
                        field_0x98 = 0;
                    }
                }
                mState = 2;
            }
        }
        break;
    case 1:
        break;
    case 2: {
        Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
        if (cam->field_0x7f5 == 0) {
            cam->field_0x7f4 = 1;
            cam->field_0x7f8 = 4;
            cam->field_0x7fc = camPos;
            cam->field_0x7f6 = 0;
        }
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (fn_803865B8(field_0x268)) {
            switch (fn_8037EFB4(field_0x268)) {
            case 0:
                fn_8038470C(field_0x268, 0x41B);
                mState = 3;
                break;
            case 1:
                fn_8038470C(field_0x268, 0x41C);
                mState = 12;
                break;
            }
        }
        break;
    }
    case 3: {
        Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
        if (cam->field_0x7f5 == 0) {
            cam->field_0x7f4 = 1;
            cam->field_0x7f8 = 4;
            cam->field_0x7fc = camPos;
            cam->field_0x7f6 = 0;
        }
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x268, mSerial)) {
            fn_80230534(false);
            field_0x258 = 3600;
            fn_801F0D20(0x3A);
            gAudioMgr_8053AF80->fn_80458FF0(0x1000007, true, 0);
            fn_8022B1E0();
            field_0x244 = 0;
            field_0x248 = 0;
            field_0x24c = 0;
            field_0x238 = 0;
            field_0x23c = 0;
            if (field_0x250 != 4) {
                field_0x250 = 4;
                field_0x234 = 0;
                field_0x240 = 0;
            }
            mState = 4;
        }
        break;
    }
    case 4: {
        field_0x258--;
        field_0x270 = -1;
        if ((field_0x258 & 0xF) == 0) {
            ActorBase_804A5770* fishes[0x400];
            field_0x270 = getActorManager_801F856C()->fn_801F7818('FISH', field_0x4, fishes, -1);
        }
        if (field_0x258 < 0 || field_0x270 == 0) {
            if (getActorManager_801F856C()->fn_801F68E0() != 0) {
                for (s32 i = 0; i < 4; i++) {
                    fn_80230C6C(i);
                }
                fn_801F0D20(0x3B);
                gAudioMgr_8053AF80->fn_80458508(0);
                if (field_0x250 != 0) {
                    field_0x250 = 0;
                    field_0x234 = 0;
                    field_0x240 = 0;
                }
                field_0x244 = 0;
                field_0x248 = 0;
                mState = 5;
            }
            break;
        }
        if (field_0x26c < 0 && field_0x250 == 4) {
            mBox.set(-24.0f, 0.0f, 24.0f, 32.0f);
            field_0x26c = fn_801F2718('FISH');
            mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
            if (field_0x26c >= 0) {
                ActorBase_804A5770* fish = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x26c);
                if (fish != NULL) {
                    f32 dy = fish->mPos.y - mPos.y;
                    f32 dx = fish->mPos.x - mPos.x;
                    f32 dist = JGeometry::TUtil<f32>::sqrt(dx * dx + dy * dy);
                    if (dist > 24.0f) {
                        field_0x26c = -1;
                    } else {
                        JGeometry::TVec3f zero(0.0f, 0.0f, 0.0f);
                        fish->field_0x3c = zero;
                    }
                }
            }
        }
        if (field_0x26c >= 0) {
            ActorBase_804A5770* fish = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x26c);
            if (fish != NULL) {
                f32 dy = fish->mPos.y - mPos.y;
                f32 dx = fish->mPos.x - mPos.x;
                f32 dist = JGeometry::TUtil<f32>::sqrt(dx * dx + dy * dy);
                if (dist < 2.0f) {
                    if (field_0x250 != 5) {
                        field_0x250 = 5;
                        field_0x234 = 0;
                        field_0x240 = 0;
                    }
                    startSound_801F0E34(0x4005C, 0);
                    spawnEmitter_8013CC50(field_0x4, &mPos, 0x54E, 0, 0, NULL, NULL, true, 1.0f);
                    field_0x25c++;
                    fish->mAlive = false;
                    field_0x26c = -1;
                } else {
                    JGeometry::TVec3f zero(0.0f, 0.0f, 0.0f);
                    fish->field_0x3c = zero;
                    dropPos = fish->mPos;
                    addCalc2_801F71A4(&dropPos.x, mPos.x, 0.3f, 1.0f);
                    addCalc2_801F71A4(&dropPos.y, mPos.y, 0.3f, 1.0f);
                    fish->mPos = dropPos;
                }
            } else {
                field_0x26c = -1;
            }
        }
        if (field_0x258 / 60 <= 11 && field_0x258 % 60 == 0 && field_0x258 > 60) {
            fn_801F0D20(0x3F);
            gAudioMgr_8053AF80->fn_80458508(1);
        }
        break;
    }
    case 5:
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        field_0x244++;
        if (field_0x244 > 50) {
            field_0x244 = 51;
            s32 layer = getPlayerLayer_8023DE58(0);
            fn_8022FE80(fn_8022EA0C(0), layer, mSerial, false, false);
            if (fn_8022FD8C(mSerial) >= 0) {
                fn_8038AD24(fn_8022EA0C(0), 0x41D, mSerial, field_0x4, msgPos, 0x102);
                mState = 6;
            }
        }
        break;
    case 6: {
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        s32 layer = getPlayerLayer_8023DE58(0);
        fn_8022FE80(fn_8022EA0C(0), layer, mSerial, false, false);
        if (fn_803865B8(fn_8022EA0C(0))) {
            gGameRoot_8053AAF8->fn_80138B7C();
            if (field_0x25c >= 10) {
                fn_8038470C(fn_8022EA0C(0), 0x41E);
            } else {
                fn_8038470C(fn_8022EA0C(0), 0x420);
            }
            mState = 7;
        }
        break;
    }
    case 7: {
        Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
        if (cam->field_0x7f5 == 0) {
            cam->field_0x7f4 = 1;
            cam->field_0x7f8 = 4;
            cam->field_0x7fc = camPos;
            cam->field_0x7f6 = 0;
        }
        s32 layer = getPlayerLayer_8023DE58(0);
        fn_8022FE80(fn_8022EA0C(0), layer, mSerial, false, false);
        if (fn_803865B8(fn_8022EA0C(0))) {
            field_0x244 = 50;
            if (field_0x25c >= 10) {
                ActorBase_804A5770* sayas[0x400];
                s32 n = getActorManager_801F856C()->fn_801F7818('SAYA', field_0x4, sayas, -1);
                JGeometry::TVec3f spots[8];
                s32 spotCount = 0;
                for (s32 i = 0; i < n && spotCount < 7; i++) {
                    if (sayas[i] != NULL && sayas[i]->field_0x90 == 2) {
                        spots[spotCount] = sayas[i]->mPos;
                        spotCount++;
                    }
                }
                if (spotCount > 0) {
                    s32 pick = (s32)((f32)spotCount * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
                    dropPos = spots[pick];
                }
                u32 arg = 0x80000;
                if (field_0x25c > 40) {
                    arg = 0x80003;
                } else if (field_0x25c > 30) {
                    arg = 0x80002;
                } else if (field_0x25c > 20) {
                    arg = 0x80001;
                }
                s32 hnrySerial = getActorManager_801F856C()->spawnActor_801F9484('HNRY', field_0x4, &mPos, arg, -1, -1);
                if (hnrySerial >= 0) {
                    WingedForceGem_804AB098* hnry = static_cast<WingedForceGem_804AB098*>(
                        getActorManager_801F856C()->findActorBySerial_801F666C(hnrySerial));
                    if (hnry != NULL) {
                        hnry->field_0x2a4 = dropPos;
                    }
                }
                field_0x244 = 0;
                if (field_0x250 != 4) {
                    field_0x250 = 4;
                    field_0x234 = 0;
                    field_0x240 = 0;
                }
            }
            mState = 8;
        }
        break;
    }
    case 8:
        field_0x244++;
        if (field_0x244 > 50) {
            if (field_0x25c >= 10) {
                fn_8038470C(fn_8022EA0C(0), 0x41F);
            } else {
                fn_8038470C(fn_8022EA0C(0), 0x421);
            }
            if (field_0x250 != 0) {
                field_0x250 = 0;
                field_0x234 = 0;
                field_0x240 = 0;
            }
            mState = 9;
        }
        break;
    case 9: {
        Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
        if (cam->field_0x7f5 == 0) {
            cam->field_0x7f4 = 1;
            cam->field_0x7f8 = 4;
            cam->field_0x7fc = camPos;
            cam->field_0x7f6 = 0;
        }
        s32 layer = getPlayerLayer_8023DE58(0);
        fn_8022FE80(fn_8022EA0C(0), layer, mSerial, false, false);
        if (fn_803865B8(fn_8022EA0C(0)) && fn_80384C58(fn_8022EA0C(0), mSerial)) {
            getActorManager_801F856C()->fn_801F6884();
            vfunc_0x30(0xFF, true);
            fn_8022B1C8();
            if (field_0x250 != 3 && field_0x250 != 1) {
                field_0x250 = 3;
                field_0x234 = 0;
                field_0x240 = 0;
            }
            if (field_0x270 == 0) {
                getActorManager_801F856C()->fn_801F8544('RUPY', field_0x4, &mPos, 0x230007);
            }
            field_0x258 = -1;
            mState = 10;
        }
        break;
    }
    case 10:
        if (field_0x250 != 3 && field_0x250 != 1) {
            fn_8022FEB8(&box, field_0x4, mSerial, false, false);
            field_0x268 = fn_8022FD8C(mSerial);
            if (field_0x268 >= 0 && fn_802300EC(field_0x268)) {
                u32 msg = field_0x25c < 10 ? 0x421 : 0x41F;
                if (fn_8038AD24(field_0x268, msg, mSerial, field_0x4, msgPos, 0x100)) {
                    JGeometry::TVec3f delta;
                    f32 dist;
                    fn_801EE8F4(&delta, &dist, field_0x268 + 1);
                    if (dist < 100.0f) {
                        u16 angle = (s32)(JMath::atanTable_.atan2_(-delta.x, -delta.y) * 10430.378f);
                        if (angle < 0x2000 || angle >= 0xE000) {
                            field_0x98 = 3;
                        } else if (angle >= 0x2000 && angle < 0x6000) {
                            field_0x98 = 1;
                        } else if (angle >= 0x6000 && angle <= 0xA000) {
                            field_0x98 = 2;
                        } else {
                            field_0x98 = 0;
                        }
                    }
                    mState = 11;
                }
            }
        }
        break;
    case 11: {
        Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
        if (cam->field_0x7f5 == 0) {
            cam->field_0x7f4 = 1;
            cam->field_0x7f8 = 4;
            cam->field_0x7fc = camPos;
            cam->field_0x7f6 = 0;
        }
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x268, mSerial)) {
            mState = 10;
        }
        break;
    }
    case 12: {
        Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
        if (cam->field_0x7f5 == 0) {
            cam->field_0x7f4 = 1;
            cam->field_0x7f8 = 4;
            cam->field_0x7fc = camPos;
            cam->field_0x7f6 = 0;
        }
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x268, mSerial)) {
            fn_80230534(false);
            mState = 0;
        }
        break;
    }
    case 13:
        break;
    }

    if (field_0x234 > 0) {
        field_0x234--;
    }
    if (field_0x238 > 0) {
        field_0x238--;
    }
    if (field_0x23c > 0) {
        field_0x23c--;
    }
    fn_801A7C48();
}

void ZoraFishHerding_804A2E30::update() {
    switch (mKind) {
    case 0:
    case 1:
        fn_801A68DC();
        break;
    }
}

// FIXME(shotgun): 90.56 %, missing/extra
void ZoraFishHerding_804A2E30::draw() {
    switch (mKind) {
    case 0:
    case 1: {
        SpriteParams_801F5AE0 params;
        if (field_0x250 != 1) {
            drawSprite_801F06F0("enemy_gc_sob.bin", field_0x254 + 0x303, &params, -1, 1.0f, 1.0f);
        }
        if (field_0x258 >= 0) {
            lbl_8053AF20->field_0x10 = field_0x25c;
            lbl_8053AF20->field_0x13 |= 1;
            lbl_8053AF20->field_0x11 = field_0x258 / 60;
            lbl_8053AF20->field_0x12 = (f32)(field_0x258 % 60) * 100.0f / 60.0f;
            lbl_8053AF20->field_0x13 |= 2;
        }
        break;
    }
    }
}
