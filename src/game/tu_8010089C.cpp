#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/SpriteParams.h"
#include "game/tu_80448940.h"
#include "game/Message.h"
#include "game/Unk_8045A080.h"
#include "game/GameView.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "game/Effect.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/act/tu_8010089C.h"

// FIXME(shotgun): 99.68 %, reloc-name; behaviour clean.
void WhackAMoleShop_8049C8EC::init() {
    if (field_0x90 & 0x10000) {
        fn_80100A90();
        return;
    }
    mBox.i.x = -10.0f;
    mBox.i.y = -10.0f;
    mBox.f.x = 10.0f;
    mBox.f.y = 10.0f;
    field_0x247 = 1;
    field_0x246 = 0;
    field_0x245 = 0;
    field_0x238 = -1;
    mPos.x += -5.0f;
    mPos.y += 7.0f;
    field_0x24c = mPos.x;
    field_0x250 = mPos.y;
    field_0x254 = mPos.z;
    field_0x258 = 0.0f;
    field_0x25c[0] = 0;
    field_0x260[0] = 0;
    field_0x268[0] = 0;
    field_0x25c[1] = 0;
    field_0x260[1] = 0;
    field_0x268[1] = 0;
    field_0x25c[2] = 0;
    field_0x260[2] = 0;
    field_0x268[2] = 0;
    field_0x25c[3] = 0;
    field_0x260[3] = 0;
    field_0x268[3] = 0;
    field_0x244 = gGameRoot_8053AAF8->fn_80138A30();
    field_0x248 = (field_0x90 >> 22) & 0x1F;
    if (vfunc_0x34()) {
        field_0x230 = 9;
        field_0x234 = 0;
    } else {
        field_0x230 = 0;
        field_0x234 = 0;
    }
}

// FIXME: 99.87 %, reloc-name; behaviour clean.
void WhackAMoleShop_8049C8EC::update() {
    if (field_0x90 & 0x10000) {
        field_0x245++;
        if (field_0x245 == 8) {
            field_0x246++;
            if (field_0x246 == 2) {
                field_0x246 = 0;
            }
            field_0x245 = 0;
        }
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        field_0x234++;
        if (field_0x234 < 40) {
            if ((field_0x234 & 3) == 0) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x1DA, 0, 1, NULL, NULL, true, 1.0f);
            }
        } else {
            field_0x240 = 1;
            field_0xb0 = 0x1100;
            if (field_0x234 == 160) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x1DA, 0, 1, NULL, NULL, true, 1.0f);
                fn_801F3668();
            }
            fn_801F3D94(1);
        }
    } else {
        fn_80100D28();
    }
}

extern const u8 lbl_80539EB0[2]; // frames per animation step (.sdata of this unit)
extern const s32 lbl_8049C8D8[]; // frames before the next mole, by player count (.data of this unit)

// Points the scripted camera at `pos` unless a move is already running.
static inline void focusCamera(const JGeometry::TVec3f& pos) {
    Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
    if (cam->field_0x7f5 == 0) {
        cam->field_0x7f4 = 1;
        cam->field_0x7f8 = 4;
        cam->field_0x7fc = pos;
        cam->field_0x7f6 = 0;
    }
}

// Picks the facing (0 to 3) from the direction to the player; unchanged when it is 100 or more away.
static inline void faceTowards(WhackAMoleShop_8049C8EC* shop, s32 player) {
    JGeometry::TVec3f dir;
    f32 dist;
    shop->fn_801EE558(&dir, &dist, player + 1);
    if (dist < 100.0f) {
        u32 angle = (u16)(s32)(JMath::atanTable_.atan2_(-dir.x, -dir.y) * 10430.378f);
        if (angle < 0x2000 || angle >= 0xE000) {
            shop->field_0x247 = 3;
        } else if (angle < 0x6000) {
            shop->field_0x247 = 1;
        } else if (angle <= 0xA000) {
            shop->field_0x247 = 2;
        } else {
            shop->field_0x247 = 0;
        }
    }
}

// FIXME(shotgun): 90.82 %, missing/extra + reloc-name; behaviour drift.
void WhackAMoleShop_8049C8EC::draw() {
    if (field_0x90 & 0x10000) {
        SpriteParams_801F5AE0 params;
        if (field_0x244 == 1) {
            params.field_0x0[11] = 6;
        } else if (field_0x244 != 0 && field_0x244 <= 5) {
            params.field_0x0[11] = field_0x244;
        }
        if (field_0x240 != 0) {
            drawSprite_801F06F0("enemy_gc_sob.bin", 0x155 + field_0x246, &params, -1, 1.0f, 1.0f);
        }
    } else {
        mPos.y = field_0x250 + field_0x258;
        drawSprite_801F06F0("enemy_gc_sob.bin", 0x311 + field_0x247 * 2 + field_0x246, NULL, 0, 1.0f, 1.0f);
        if (field_0x230 == 5 || field_0x230 == 6 || field_0x230 == 7 || field_0x230 == 8) {
            s32 time = field_0x23c;
            s32 minutes = time / 60;
            lbl_8053AF18->field_0x30 = minutes;
            lbl_8053AF18->field_0x31 = (u8)((f32)(time - minutes * 60) / 0.6f);
            lbl_8053AF18->field_0x32 |= 0x10;
            for (s32 i = 0; i < field_0x244; i++) {
                lbl_8053AF18->field_0x32 |= (u8)(1 << i);
                lbl_8053AF18->field_0x2c[i] = (u8)field_0x260[i];
            }
        }
    }
}

// FIXME(shotgun): 89.17 %, missing/extra + reloc-name; behaviour drift.
void WhackAMoleShop_8049C8EC::fn_80100D28() {
    JGeometry::TBox2f box = mBox;
    JGeometry::TVec3f msgPos = mPos;
    msgPos.y -= 50.0f;
    JGeometry::TVec3f focus = mPos;
    box.i.x += mPos.x;
    box.f.x += mPos.x;
    box.i.y += mPos.y;
    box.f.y += mPos.y;
    fn_80239914(&box, field_0x4, mSerial);
    field_0x245++;
    if (field_0x245 == lbl_80539EB0[field_0x246]) {
        field_0x246++;
        if (field_0x246 == 2) {
            field_0x246 = 0;
        }
        field_0x245 = 0;
    }
    switch (field_0x230) {
    case 0:
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        field_0x238 = fn_8022FD8C(mSerial);
        if (field_0x238 >= 0 && fn_802300EC(field_0x238)) {
            u32 msg = 0x31F;
            if (field_0x244 == 1) {
                msg = 0x328;
            }
            if (fn_8038AD24(field_0x238, msg, mSerial, field_0x4, msgPos, 0)) {
                for (s32 i = 0; i < 4; i++) {
                    fn_802308E0(i);
                }
                for (s32 i = 0; i < 4; i++) {
                    field_0x25c[i] = 0;
                    field_0x260[i] = 0;
                }
                field_0x230 = 1;
                faceTowards(this, field_0x238);
            }
        }
        break;
    case 1:
        focusCamera(focus);
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (fn_803865B8(field_0x238)) {
            switch (fn_8037EFB4(field_0x238)) {
            case 0: {
                u32 msg = 0x320;
                if (field_0x244 == 1) {
                    msg = 0x329;
                }
                fn_8038470C(field_0x238, msg);
                for (s32 i = 0; i < 4; i++) {
                    field_0x270[i] = fn_802373EC(i);
                    field_0x280[i] = fn_8023725C(i);
                    fn_802372F8(i, 5, 1);
                }
                field_0x230 = 3;
                field_0x234 = 0;
                break;
            }
            case 1:
                fn_8038470C(field_0x238, 0x321);
                field_0x230 = 2;
                field_0x234 = 0;
                break;
            }
        }
        break;
    case 2:
        focusCamera(focus);
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x238, mSerial)) {
            field_0x230 = 0;
            field_0x234 = 0;
            fn_80230534(false);
        }
        break;
    case 3:
        focusCamera(focus);
        if (field_0x247 == 3) {
            field_0x258 += 1.0f;
        } else {
            field_0x258 -= 1.0f;
        }
        if (field_0x258 >= 20.0f) {
            field_0x258 = 20.0f;
        } else if (field_0x258 <= -20.0f) {
            field_0x258 = -20.0f;
        }
        fn_8022FE80(field_0x238, getPlayerLayer_8023DE58(0), mSerial, false, false);
        if (fn_80388CAC(field_0x238, mSerial)) {
            field_0x230 = 4;
            field_0x234 = 0;
            field_0x23c = 3600;
            fn_80230534(false);
            fn_8022B22C();
        }
        break;
    case 4:
        lbl_8053AED0->fn_8043CB24(3 - field_0x234 / 60, 0xFF, 0, 1, true,
                                  0.5f * (f32)JUTVideo::sManager->getFbWidth(),
                                  0.5f * (f32)JUTVideo::sManager->getEfbHeight(), 3.0f);
        if (field_0x234 % 60 == 0) {
            fn_801F0D20(0x3F);
        }
        field_0x234++;
        if (field_0x234 == 180) {
            field_0x230 = 5;
            field_0x234 = 0;
            fn_801F0D20(0x3A);
            getGameView_802D6A9C()->fn_802D8050(field_0x248, true, 0xFF);
            field_0x240 = gAudioMgr_8053AF80->field_0x58;
            gAudioMgr_8053AF80->fn_80458FF0(0x1000007, true, 0);
        }
        break;
    case 5:
        field_0x23c--;
        if (field_0x23c / 60 <= 10 && field_0x23c % 60 == 59) {
            fn_801F0D20(0x3F);
            gAudioMgr_8053AF80->fn_80458508(1);
        }
        if (field_0x23c / 60 == 10) {
            lbl_8053AED0->fn_8043D67C(0x326, 0, 0);
        }
        field_0x234++;
        if (field_0x234 >= lbl_8049C8D8[field_0x244] && field_0x23c >= 0x78) {
            s32 n = (s32)(9.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
            JGeometry::TVec3f pos;
            pos.x = 80.0f * (f32)(n % 3) + (85.0f + field_0x24c) +
                    (60.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 30.0f);
            pos.y = 80.0f * (f32)(n / 3) + (field_0x250 - 75.0f) +
                    (60.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 30.0f);
            pos.z = 0.0f;
            getActorManager_801F856C()->spawnActor_801F9484('MGYA', field_0x4, &pos, mSerial + 0x10000, -1, -1);
            field_0x234 = 0;
        }
        if (field_0x23c == 0) {
            getActorManager_801F856C()->fn_801F68E0();
            field_0x230 = 6;
            field_0x234 = 0;
            fn_801F0D20(0x3B);
            gAudioMgr_8053AF80->fn_80458508(0);
            getGameView_802D6A9C()->fn_802D8050(field_0x248, false, 0xFF);
            field_0x247 = 2;
        }
        break;
    case 6:
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        field_0x234++;
        if (field_0x234 > 50 && getActorManager_801F856C()->fn_801F68E0() == 1) {
            field_0x238 = fn_8022EA0C(0);
            fn_8022FE80(field_0x238, getPlayerLayer_8023DE58(field_0x238), mSerial, false, false);
            if (fn_8022FD8C(mSerial) >= 0) {
                fn_8038AD24(field_0x238, 0x322, mSerial, field_0x4, msgPos, 2);
                field_0x230 = 7;
                field_0x234 = 0;
                fn_8022B214();
                gGameRoot_8053AAF8->fn_80138B7C();
                field_0x234 = 0;
            }
        }
        break;
    case 7:
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        fn_8022FE80(field_0x238, getPlayerLayer_8023DE58(field_0x238), mSerial, false, false);
        if (fn_803865B8(field_0x238)) {
            if (field_0x234 > 0 && field_0x260[field_0x234 - 1] != 0) {
                lbl_8053AED0->fn_8043D874(field_0x234 - 1, field_0x260[field_0x234 - 1] * 20, 0);
            }
            if (field_0x234 < field_0x244) {
                fn_803894BC(field_0x238, field_0x234 + 1);
                fn_8038938C(field_0x238, field_0x260[field_0x234]);
                fn_80389254(field_0x238, field_0x260[field_0x234] * 20);
                u32 msg;
                if (field_0x260[field_0x234] >= 40) {
                    msg = 0x323;
                } else if (field_0x260[field_0x234] >= 20) {
                    msg = 0x324;
                } else if (field_0x260[field_0x234] >= 1) {
                    msg = 0x325;
                } else {
                    msg = 0x31E;
                }
                fn_8038470C(field_0x238, msg);
            } else {
                fn_8038470C(field_0x238, 0x327);
                field_0x230 = 8;
                field_0x234 = 0;
            }
            field_0x234++;
        }
        break;
    case 8:
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        fn_8022FE80(field_0x238, getPlayerLayer_8023DE58(field_0x238), mSerial, false, false);
        if (fn_803865B8(field_0x238) && fn_80384C58(field_0x238, mSerial)) {
            field_0x230 = 9;
            field_0x234 = 0;
            getActorManager_801F856C()->fn_801F6884();
            for (s32 i = 0; i < 4; i++) {
                if (!fn_8022F514(i)) {
                    fn_802372F8(i, field_0x270[i], field_0x280[i]);
                }
            }
            if (field_0x244 == 1) {
                if (field_0x25c[0] == 1) {
                    fn_802362C4(0, 500);
                }
            } else {
                s32 best = -1;
                for (s32 i = 0; i < 4; i++) {
                    if (best < (u16)field_0x268[i]) {
                        best = (u16)field_0x268[i];
                    }
                }
                if (best != 0) {
                    for (s32 i = 0; i < field_0x244; i++) {
                        getGameView_802D6A9C()->fn_802D7844(best == (u16)field_0x268[i], i);
                    }
                }
            }
            vfunc_0x30(0xFF, true);
        }
        break;
    case 9:
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        field_0x238 = fn_8022FD8C(mSerial);
        if (field_0x238 >= 0 && fn_8038AD24(field_0x238, 0x328, mSerial, field_0x4, msgPos, 0)) {
            field_0x230 = 10;
            field_0x234 = 0;
            faceTowards(this, field_0x238);
        }
        break;
    case 10:
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x238, mSerial)) {
            field_0x230 = 9;
            field_0x234 = 0;
        }
        break;
    }
}

// FIXME(shotgun): 94.59 %, missing/extra + reloc-name; behaviour drift.
void WhackAMoleShop_8049C8EC::fn_80100A90() {
    field_0x290 = static_cast<WhackAMoleShop_8049C8EC*>(getActorManager_801F856C()->findActorBySerial_801F666C(field_0x90 & 0xFFFF));
    mBox.i.x = -12.0f;
    mBox.i.y = -12.0f;
    mBox.f.x = 12.0f;
    mBox.f.y = 12.0f;
    mBox2.i.x = -12.0f;
    mBox2.i.y = -12.0f;
    mBox2.f.x = 12.0f;
    mBox2.f.y = 12.0f;
    field_0x234 = 0;
    field_0x246 = 0;
    field_0x245 = 0;
    field_0x240 = 0;
    s32 roll = (s32)(100.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
    s32 players = (u8)gGameRoot_8053AAF8->fn_80138A30();
    if (field_0x290->field_0x23c > 720) {
        if (roll < 10) {
            field_0x244 = 1;
        } else if (players >= 1 && roll < 15) {
            field_0x244 = 2;
        } else if (players >= 2 && roll < 20) {
            field_0x244 = 3;
        } else if (players >= 3 && roll < 25) {
            field_0x244 = 4;
        } else if (players == 4 && roll < 30) {
            field_0x244 = 5;
        } else {
            field_0x244 = 0;
        }
    } else {
        s32 base = 0;
        s32 w2 = 0;
        s32 w1 = 0;
        s32 w4 = 0;
        s32 w3;
        switch (players) {
        case 1:
            w1 = 90;
            base = 10;
            break;
        case 2:
            w1 = 45;
            w2 = 45;
            base = 10;
            break;
        case 3:
            w1 = 30;
            w2 = 30;
            w3 = 30;
            base = 10;
            break;
        case 4:
            w1 = 22;
            w2 = 22;
            w3 = 22;
            w4 = 22;
            base = 12;
            break;
        }
        if (roll < base) {
            field_0x244 = 1;
        } else if (roll < base + w1) {
            field_0x244 = 2;
        } else if (roll < base + w1 + w2) {
            field_0x244 = 3;
        } else if (roll < base + w1 + w2 + w3) {
            field_0x244 = 4;
        } else if (roll < base + w1 + w2 + w3 + w4) {
            field_0x244 = 5;
        }
    }
    spawnEmitter_8013CC50(field_0x4, &mPos, 0x1DA, 0, 1, NULL, NULL, true, 1.0f);
}

// FIXME(shotgun): 96.83 %, missing/extra + reloc-name; behaviour near.
void WhackAMoleShop_8049C8EC::vfunc_0x44() {
    s32 player = field_0x10c;
    if (player < 0) {
        return;
    }
    startSound_801F0E34(0x40099, 0);
    if (fn_8022F514(player)) {
        player = fn_8022F3DC(player);
    }
    if (field_0x244 == 1) {
        JGeometry::TVec3f pos = mPos;
        pos.x += -15.0f;
        spawnEmitter_8013CC50(field_0x4, &pos, 0x59, 1, 1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &pos, 0x5A, 1, 1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &pos, 0x5B, 1, 1, NULL, NULL, true, 1.0f);
        startSound_801F0E34(0x30001, 0);
        JGeometry::TVec2f pos2(pos.x, pos.y);
        fn_80238548(player, 0, &pos2, 0xb);
    } else {
        s32 hits = 1;
        s32 total = 0;
        if (player == field_0x244 - 2) {
            hits = 3;
        } else if (field_0x244 != 0) {
            total = 1;
        }
        field_0x290->field_0x260[player] += hits;
        field_0x290->field_0x268[player] += total;
    }
}

WhackAMoleShop_8049C8EC::~WhackAMoleShop_8049C8EC() {}
